#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <io/tty.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>


void kernel_early(void *mboot, unsigned int magic, unsigned int *ebp)
{
  initialize_terminal();
  terminal_writestring("TTY   ... [\x1b[32mDONE\x1b[00m]\nGDT   ... ");
  initialize_gdt();
  terminal_writestring("[\x1b[32mDONE\x1b[00m]\nIDT   ... ");
  initialize_idt();
  terminal_writestring("[\x1b[32mDONE\x1b[00m]\n");
}

void main()
{
}
