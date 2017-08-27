#include <stdint.h>
#include <string.h>
#include <cpu/idt.h>
#include <cpu/isr.h>
#include <misc/utils.h>
#include <io/tty.h>
#include <misc/panic.h>

extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();
extern void _isr127();

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

idt_entry_t idt[256];
idt_ptr_t idt_ptr;

void *double_fault(registers_t *regs)
{
  unsigned int err_code = regs->err_code;
  terminal_writestring("Double Fault! Error-Code: 0x");
  terminal_writehex(err_code);
  terminal_writestring("\n");
  PANIC("Double Fault");
  return NULL;
}

void initialize_idt()
{
  idt_ptr.limit = (256 * sizeof(idt_entry_t)) - 1;
  idt_ptr.base = (unsigned int *) &idt;
  memset((void *)idt_ptr.base, 0, 256 * sizeof(idt_entry_t));

  fill_idt_entry(0, 8, _isr0, 0xe, 0);
	fill_idt_entry(1, 8, _isr1, 0xe, 0);
	fill_idt_entry(2, 8, _isr2, 0xe, 0);
	fill_idt_entry(3, 8, _isr3, 0xe, 0);
	fill_idt_entry(4, 8, _isr4, 0xe, 0);
	fill_idt_entry(5, 8, _isr5, 0xe, 0);
	fill_idt_entry(6, 8, _isr6, 0xe, 0);
	fill_idt_entry(7, 8, _isr7, 0xe, 0);
	fill_idt_entry(8, 8, _isr8, 0xe, 0);
	fill_idt_entry(9, 8, _isr9, 0xe, 0);
	fill_idt_entry(10, 8, _isr10, 0xe, 0);
	fill_idt_entry(11, 8, _isr11, 0xe, 0);
	fill_idt_entry(12, 8, _isr12, 0xe, 0);
	fill_idt_entry(13, 8, _isr13, 0xe, 0);
	fill_idt_entry(14, 8, _isr14, 0xe, 0);
	fill_idt_entry(15, 8, _isr15, 0xe, 0);
	fill_idt_entry(16, 8, _isr16, 0xe, 0);
	fill_idt_entry(17, 8, _isr17, 0xe, 0);
	fill_idt_entry(18, 8, _isr18, 0xe, 0);
	fill_idt_entry(19, 8, _isr19, 0xe, 0);
	fill_idt_entry(20, 8, _isr20, 0xe, 0);
	fill_idt_entry(21, 8, _isr21, 0xe, 0);
	fill_idt_entry(22, 8, _isr22, 0xe, 0);
	fill_idt_entry(23, 8, _isr23, 0xe, 0);
	fill_idt_entry(24, 8, _isr24, 0xe, 0);
	fill_idt_entry(25, 8, _isr25, 0xe, 0);
	fill_idt_entry(26, 8, _isr26, 0xe, 0);
	fill_idt_entry(27, 8, _isr27, 0xe, 0);
	fill_idt_entry(28, 8, _isr28, 0xe, 0);
	fill_idt_entry(29, 8, _isr29, 0xe, 0);
	fill_idt_entry(30, 8, _isr30, 0xe, 0);
	fill_idt_entry(31, 8, _isr31, 0xe, 0);
	fill_idt_entry(127, 8, _isr127, 0xe, 0);

  idt_flush(&idt_ptr);
}

static void fill_idt_entry(int entry, unsigned short selector, void *offset, unsigned char gate, unsigned char priv)
{
  idt[entry].base_low = (unsigned short)((unsigned int)offset & 0xFFFF);
  idt[entry].base_high = (unsigned short)(((unsigned int)offset >> 16) & 0xFFFF);

  idt[entry].selector = selector;
  idt[entry].zero = 0;

  idt[entry].flags = (unsigned char)(0x80 | ((priv & 0x03) << 5) | gate);
}

void pic_remap(unsigned char offset_from, unsigned char offset_to)
{
  unsigned char mask1, mask2;

  mask1 = inb(PIC1_DATA);
  mask2 = inb(PIC2_DATA);

  outb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);
  io_wait();
  outb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
  io_wait();
  outb(PIC1_DATA, offset_from);
  io_wait();
  outb(PIC2_DATA, offset_to);
  io_wait();
  outb(PIC1_DATA, 4);
  io_wait();
  outb(PIC2_DATA, 2);
  io_wait();
  outb(PIC1_DATA, ICW4_8086);
  io_wait();
  outb(PIC2_DATA, ICW4_8086);
  io_wait();
  outb(PIC1_DATA, mask1);
  io_wait();
  outb(PIC2_DATA, mask2);
  io_wait();
}
