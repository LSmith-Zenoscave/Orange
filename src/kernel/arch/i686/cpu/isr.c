#include <cpu/isr.h>
#include <misc/panic.h>
#include <misc/utils.h>
#include <stdint.h>

static irq_handler_t irq_handlers[256] = {NULL};
static isr_handler_t isr_handlers[256] = {NULL};

void register_irq_handler(unsigned char n, irq_handler_t handler) {
  irq_handlers[n] = handler;
}
void unregister_irq_handler(unsigned char n) {
  irq_handlers[n] = default_irq_handler;
}

void register_isr_handler(unsigned char n, isr_handler_t handler) {
  isr_handlers[n] = handler;
}
void unregister_isr_handler(unsigned char n) {
  isr_handlers[n] = default_fault_handler;
}

void irq_handler(registers_t *registers) {
  void *stack = NULL;
  irq_handler_t handler = irq_handlers[registers->int_no];

  if (pic_isnormalIRQ(registers->int_no)) {
    if (handler) {
      stack = handler(registers);
    }
    pic_EOI(registers->int_no);
  } else {
    pic_EOI_spurious(registers->int_no);
  }
}

void fault_handler(registers_t *registers) {
  void *stack = NULL;
  isr_handler_t handler = isr_handlers[registers->int_no];
  if (handler) {
    stack = handler(registers);
  }

  /*
    if (stack)
    {
      taskswitch(stack);
    }
  */
}

uint8_t pic_isnormalIRQ(unsigned int irqnum) {
  unsigned short isr;
  if (irqnum == 7 || irqnum == 15) {
    isr = pic_get_isr();
  } else {
    return 1;
  }

  if (irqnum == 7 && isr & 0x80 || isr & 0x08) {
    return 1;
  }

  return 0;
}

void pic_EOI(unsigned int irqnum) {
  if (irqnum < 8) {
    outb(PIC1_COMMAND, (unsigned char)(0x60 | (irqnum & 0x7)));
  } else {
    outb(PIC2_COMMAND, (unsigned char)(0x60 | (irqnum & 0x7)));
    outb(PIC1_COMMAND, 0x62);
  }
}

void pic_EOI_spurious(unsigned int irqnum) {
  if (irqnum == 15) {
    outb(PIC1_COMMAND, PIC_EOI);
  }
}
/* Helper func */
static unsigned short __pic_get_irq_reg(int ocw3) {
  /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
   * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
  outb(PIC1_COMMAND, ocw3);
  outb(PIC2_COMMAND, ocw3);
  return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

/* Returns the combined value of the cascaded PICs irq request register */
unsigned short pic_get_irr(void) { return __pic_get_irq_reg(PIC_READ_IRR); }

/* Returns the combined value of the cascaded PICs in-service register */
unsigned short pic_get_isr(void) { return __pic_get_irq_reg(PIC_READ_ISR); }

int *default_irq_handler(registers_t *registers) {
  PANIC("NO irq handler installed");
  return NULL;
}

void *default_fault_handler(registers_t *registers) {
  PANIC("NO irq handler installed");
  return NULL;
}
