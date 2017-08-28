/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : alloc.h
 *      Purpose : memory heap allocation and usage declarations
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#ifndef _KERNEL_ALLOC_H
#define _KERNEL_ALLOC_H 1

#include <stddef.h>

#define KERNEL_HEAP_START 0x0C000000
#define KERNEL_HEAP_INIT 0x00100000
#define KERNEL_HEAP_END 0x0CFFF000

#define HEAP_INDEX 0x00020000
#define HEAP_MAGIC 0xD00DB00B
#define HEAP_MIN_SIZE 0x00070000

typedef struct heap_header_struct {
  size_t size;
  unsigned char is_free;
  struct heap_header_struct *next;
  struct heap_header_struct *prev;
} __attribute__((packed)) heap_header_t;

typedef struct heap_footer_struct {
  size_t size;
} __attribute__((packed)) heap_footer_t;

typedef struct heap_struct {
  heap_header_t *header;
  void *start;
  void *end;
  void *max;
  unsigned char supervisor;
  unsigned char read_only;
} __attribute__((packed)) heap_t;

heap_t *create_heap(void *, void *, unsigned char, unsigned char);
void *khalloc(size_t size, unsigned int align, heap_t *);
void khfree(void *p, heap_t *heap);

#endif
