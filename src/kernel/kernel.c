#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <boot/multiboot.h>

#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <io/tty.h>
#include <mem/paging.h>

void kernel_early(multiboot_t *mboot, unsigned int magic, unsigned int *ebp) {
  initialize_terminal();
  terminal_writestring("TTY   ... [\x1b[32mDONE\x1b[00m]\nGDT   ... ");
  initialize_gdt();
  terminal_writestring("[\x1b[32mDONE\x1b[00m]\nIDT   ... ");
  initialize_idt();
  terminal_writestring("[\x1b[32mDONE\x1b[00m]\nIRQ   ... ");
  initialize_irq();
  register_isr_handler(8, double_fault);
  __asm__ __volatile__("sti");
  terminal_writestring("[\x1b[32mDONE\x1b[00m]\nPAGE  ... ");
  initialize_paging(mboot->mem_upper + mboot->mem_lower);
  terminal_writestring("[\x1b[32mDONE\x1b[00m]\n0x");
  terminal_writehex(magic);
  terminal_writehex(*ebp);
}

void main(void) {
  terminal_clearscreen();
  printf(
      "______                               _____  _____ \n"
      "|  _  \\                             |  _  |/  ___|\n"
      "| | | |   ___     ___    _ __ ___   | | | |\\ `--. \n"
      "| | | |  / _ \\   / _ \\  | '_ ` _ \\  | | | | `--. \\\n"
      "| |/  | | (_) | | (_) | | | | | | | | \\_/ |/\\__/ /\n"
      "|____/   \\___/   \\___/  |_| |_| |_|  \\___/ \\____/ \n");
}
