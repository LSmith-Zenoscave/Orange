/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : isr.c
 *      Purpose : kernel interrupt and fault handlers
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#include <cpu/isr.h>
#include <misc/panic.h>
#include <misc/utils.h>
#include <stdint.h>
#include <io/tty.h>

static irq_handler_t irq_handlers[256] = {NULL};
static isr_handler_t isr_handlers[256] = {NULL};

/*
 * ---------------------------------------------------------------------------
 *      Name   : register_irq_handler
 *      Purpose: register irq handler
 *      Args ---
 *        n: unsigned char
 *          - IRQ Number
 *        handler: irq_handler_t
 *          - handler to call
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void register_irq_handler(unsigned char n, irq_handler_t handler) {
  irq_handlers[n] = handler;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : unregister_irq_handler
 *      Purpose: unregister irq handler (replace with default handler)
 *      Args ---
 *        n: unsigned char
 *          - IRQ Number
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void unregister_irq_handler(unsigned char n) {
  irq_handlers[n] = default_irq_handler;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : register_isr_handler
 *      Purpose: register isr handler
 *      Args ---
 *        n: unsigned char
 *          - fault IRQ Number
 *        handler: isr_handler_t
 *          - handler to call
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void register_isr_handler(unsigned char n, isr_handler_t handler) {
  isr_handlers[n] = handler;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : unregister_isr_handler
 *      Purpose: unregister isr handler
 *      Args ---
 *        n: unsigned char
 *          - fault IRQ Number
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void unregister_isr_handler(unsigned char n) {
  isr_handlers[n] = default_fault_handler;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : irq_handler
 *      Purpose: stub of the main irq handler
 *      Args ---
 *        registers: registers_t *
 *          - register states at time of irq call
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void irq_handler(registers_t *registers) {
  // void *stack = NULL;
  irq_handler_t handler = irq_handlers[registers->int_no];

  if (pic_isnormalIRQ(registers->int_no)) {
    if (handler) {
      //stack = handler(registers);
      handler(registers);
    }
    pic_EOI(registers->int_no);
  } else {
    pic_EOI_spurious(registers->int_no);
  }
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : fault_handler
 *      Purpose: stub of the main fault handler
 *      Args ---
 *        registers: registers_t *
 *          - register states at time of fault call
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void fault_handler(registers_t *registers) {
  // void *stack = NULL;
  isr_handler_t handler = isr_handlers[registers->int_no];
  if (handler) {
    //stack = handler(registers);
    handler(registers);
  }

  /*
    if (stack)
    {
      taskswitch(stack);
    }
  */
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : pic_isnormalIRQ
 *      Purpose: return whether or not the IRQ number is spurious/normal
 *      Args ---
 *        irqnum: unsigned int
 *          - requested interrupt number
 *      Returns: unsigned char
 * ---------------------------------------------------------------------------
 */
unsigned char pic_isnormalIRQ(unsigned int irqnum) {
  unsigned short isr;
  if (irqnum == 7 || irqnum == 15) {
    isr = pic_get_isr();
  } else {
    return 1;
  }

  if ((irqnum == 7 && isr & 0x80) || isr & 0x08) {
    return 1;
  }

  return 0;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : pic_EOI
 *      Purpose: signal the EOI (end of interrupt) to the PIC
 *      Args ---
 *        irqnum: unsigned int
 *          - ended interrupt number
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void pic_EOI(unsigned int irqnum) {
  if (irqnum < 8) {
    outb(PIC1_COMMAND, (unsigned char)(0x60 | (irqnum & 0x7)));
  } else {
    outb(PIC2_COMMAND, (unsigned char)(0x60 | (irqnum & 0x7)));
    outb(PIC1_COMMAND, 0x62);
  }
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : pic_EOI_spurrious
 *      Purpose: signal the EOI (end of spurious interrupt) to the PIC
 *      Args ---
 *        irqnum: unsigned int
 *          - ended interrupt number
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void pic_EOI_spurious(unsigned int irqnum) {
  if (irqnum == 15) {
    outb(PIC1_COMMAND, PIC_EOI);
  }
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : __pic_get_irq_reg
 *      Purpose: get register values of irqs off PIC1&2
 *      Args ---
 *        ocw3: int
 *          - value of ocw3 command
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
static unsigned short __pic_get_irq_reg(int ocw3) {
  /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
   * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
  outb(PIC1_COMMAND, ocw3);
  outb(PIC2_COMMAND, ocw3);
  return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : pic_get_irr
 *      Purpose: Returns the combined value of the
 *               cascaded PICs irq request register
 *      Args --- void
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
unsigned short pic_get_irr(void) { return __pic_get_irq_reg(PIC_READ_IRR); }

/*
 * ---------------------------------------------------------------------------
 *      Name   : pic_get_irr
 *      Purpose: Returns the combined value of the
 *               cascaded PICs in-service register
 *      Args --- void
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
unsigned short pic_get_isr(void) { return __pic_get_irq_reg(PIC_READ_ISR); }

/*
 * ---------------------------------------------------------------------------
 *      Name   : default_irq_handler
 *      Purpose: default handler fallback for irq
 *      Args ---
 *        registers: registers_t *
 *          - registers at time of the irq call
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
int *default_irq_handler(registers_t *registers) {
  terminal_writestring("Interrupt Number: ");
  terminal_writehex(registers->int_no);
  terminal_writestring("\n");
  PANIC("NO irq handler installed");
  return NULL;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : default_isr_handler
 *      Purpose: default fault handler fallback
 *      Args ---
 *        registers: registers_t *
 *          - registers at time of the fault irq call
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void *default_fault_handler(registers_t *registers) {
  terminal_writestring("Interrupt Number: ");
  terminal_writehex(registers->int_no);
  terminal_writestring("\n");
  PANIC("NO isr handler installed");
  return NULL;
}
