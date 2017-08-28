/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : alloc.c
 *      Purpose : kernel memory heap allocation
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#include <assert.h>
#include <mem/alloc.h>
#include <mem/mem.h>
#include <misc/panic.h>
#include <stddef.h>
#include <stdio.h>

extern unsigned int end;

heap_t *kernel_heap;

void *placement_address = &end;

heap_header_t *next_free_block(heap_header_t *block);
heap_header_t *next_contig_block(heap_header_t *block);
heap_header_t *prev_free_block(heap_header_t *block);
heap_header_t *prev_contig_block(heap_header_t *block);

/*
 * ---------------------------------------------------------------------------
 *      Name   : create_heap
 *      Purpose: instantiate memory structure of a heap
 *      Args ---
 *        start: void *
 *          - starting address
 *        heap_end: void *
 *          - ending address
 *        supervisor: unsigned char
 *          - supervisor ring bits
 *        ro: unsigned char
 *          - read-only flag
 *      Returns: heap_t *
 * ---------------------------------------------------------------------------
 */
heap_t *create_heap(void *start, void *heap_end, unsigned char supervisor,
                    unsigned char ro) {
  assert(!start & 0xFFF);
  assert(!heap_end & 0xFFF);

  heap_t *heap = (heap_t *)kmalloc(sizeof(heap_t));
  heap->header = (heap_header_t *)start;
  heap->header->size = (unsigned int)(heap_end - start);
  heap->header->is_free = 1;
  heap->header->next = NULL;
  heap->header->prev = NULL;
  heap->start = start;
  heap->end = heap_end;
  heap->supervisor = supervisor;
  heap->read_only = ro;

  return heap;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : khalloc
 *      Purpose: Kernel heap memory allocation
 *      Args ---
 *        size: size_t
 *          - requested size
 *        align: unsigned int
 *          - alignment flag
 *        heap: heap_t *
 *          - heap to allocate from
 *      Returns: void *
 * ---------------------------------------------------------------------------
 */
void *khalloc(size_t size, unsigned int align, heap_t *heap) {
  size_t full_size = size + sizeof(heap_header_t) + sizeof(heap_footer_t);

  assert(heap->header != NULL);

  heap_header_t *head_ptr = heap->header;

  while (head_ptr && head_ptr->next) {
    head_ptr = head_ptr->next;
  }

  while (head_ptr && head_ptr->is_free == 0) {
    if (prev_contig_block(head_ptr) == NULL) {
      break;
    }

    head_ptr = prev_contig_block(head_ptr);
  }

  while (head_ptr && head_ptr->size < full_size) {
    head_ptr = prev_free_block(head_ptr);
  }

  if (head_ptr == NULL || head_ptr->is_free == 0 ||
      head_ptr->size < full_size) {
    PANIC("Out of heap");  // Something is wrong...
    return NULL;
  }

  heap_header_t *new_head =
      (heap_header_t *)((unsigned int)head_ptr + head_ptr->size - full_size);
  head_ptr->size -= full_size;
  new_head->size = full_size;
  new_head->is_free = 0;
  new_head->next = NULL;
  new_head->prev = head_ptr->prev;
  heap_footer_t *new_foot =
      (heap_footer_t *)((unsigned int)new_head + full_size -
                        sizeof(heap_footer_t));
  new_foot->size = full_size;

  heap_footer_t *head_foot =
      (heap_footer_t *)((unsigned int)head_ptr + head_ptr->size -
                        sizeof(heap_footer_t));
  head_foot->size = head_ptr->size;

  return (void *)((unsigned int)new_head + sizeof(heap_header_t));
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : khfree
 *      Purpose: Kernel heap memory free
 *      Args ---
 *        p: void *
 *          - requested address to free
 *        heap: heap_t *
 *          - heap to free from
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void khfree(void *p, heap_t *heap) {
  heap_header_t *head_ptr =
      (heap_header_t *)((unsigned int)p - sizeof(heap_header_t));
  assert((heap_header_t *)heap->start <= head_ptr);

  heap_header_t *next_ptr;
  heap_header_t *prev_ptr;

  if (head_ptr->is_free == 0) {
    head_ptr->is_free = 1;
    next_ptr = next_contig_block(head_ptr);

    if (next_ptr && next_ptr->is_free == 1)  // then coalesce.
    {
      heap_footer_t *next_foot =
          (heap_footer_t *)((unsigned int)next_ptr + next_ptr->size -
                            sizeof(heap_footer_t));

      head_ptr->next = next_ptr->next;
      head_ptr->size += next_ptr->size;
      next_foot->size = head_ptr->size;
    }

    prev_ptr = prev_contig_block(head_ptr);

    if (prev_ptr && prev_ptr->is_free == 1) {
      heap_footer_t *head_foot =
          (heap_footer_t *)((unsigned int)head_ptr + head_ptr->size -
                            sizeof(heap_footer_t));

      prev_ptr->next = head_ptr->next;
      prev_ptr->size += head_ptr->size;
      head_foot->size = prev_ptr->size;
    }
  }
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : next_free_block
 *      Purpose: find next free block
 *      Args ---
 *        block: heap_header_t *
 *          - base heap header
 *      Returns: heap_header_t *
 * ---------------------------------------------------------------------------
 */
heap_header_t *next_free_block(heap_header_t *block) {
  return (block->is_free == 1) ? block->next : NULL;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : next_contig_block
 *      Purpose: find next contiguous block
 *      Args ---
 *        block: heap_header_t *
 *          - base heap header
 *      Returns: heap_header_t *
 * ---------------------------------------------------------------------------
 */
heap_header_t *next_contig_block(heap_header_t *block) {
  return (heap_header_t *)((unsigned int)block + block->size);
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : prev_free_block
 *      Purpose: find previous free block
 *      Args ---
 *        block: heap_header_t *
 *          - base heap header
 *      Returns: heap_header_t *
 * ---------------------------------------------------------------------------
 */
heap_header_t *prev_free_block(heap_header_t *block) {
  return (block->is_free == 1) ? block->prev : NULL;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : prev_contig_block
 *      Purpose: find previous contiguous block
 *      Args ---
 *        block: heap_header_t *
 *          - base heap header
 *      Returns: heap_header_t *
 * ---------------------------------------------------------------------------
 */
heap_header_t *prev_contig_block(heap_header_t *block) {
  heap_footer_t *prev_foot =
      (heap_footer_t *)((unsigned int)block - sizeof(heap_footer_t));
  if (prev_foot < (heap_footer_t *)kernel_heap->start) {
    return NULL;
  }
  return (heap_header_t *)((unsigned int)block - prev_foot->size);
}
