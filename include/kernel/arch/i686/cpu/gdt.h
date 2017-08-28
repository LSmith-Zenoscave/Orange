/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : gdt.h
 *      Purpose : Global Descriptor Table (GDT) declarations
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H 1

#if !defined(__cplusplus)
#include <stdbool.h>
#endif

#include <stddef.h>

typedef struct gdt_entry_struct {
  unsigned short limit_low;
  unsigned short base_low;
  unsigned char base_middle;
  unsigned char access;
  unsigned char granularity;
  unsigned char base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_source_struct {
  unsigned int *base;
  unsigned int limit;
  unsigned char type;
} __attribute((packed)) gdt_t;

typedef struct gdt_ptr_struct {
  unsigned short limit;
  unsigned int *base;
} __attribute__((packed)) gdt_ptr_t;

void initialize_gdt();
extern void gdt_flush(unsigned int *);
static void fill_gdt_entry(unsigned int, gdt_t, bool);

#endif
