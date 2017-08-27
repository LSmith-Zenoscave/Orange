#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H 1

#if !defined(__cplusplus)
#include <stdbool.h>
#endif

#include <stddef.h>
#include <stdint.h>
#include <cpu/isr.h>

typedef struct idt_entry_struct
{
  unsigned short base_low;
  unsigned short selector;
  unsigned char zero;
  unsigned char flags;
  unsigned short base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr_struct
{
  unsigned short limit;
  unsigned int *base;
} __attribute__((packed)) idt_ptr_t;

void initialize_idt();
extern void idt_flush();
static void fill_idt_entry(int, unsigned short, void *, unsigned char, unsigned char);

void initialize_irq();
void pic_remap(unsigned char, unsigned char);

void *double_fault(registers_t *);

#endif
