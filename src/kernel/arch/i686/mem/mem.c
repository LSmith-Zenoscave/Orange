/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : mem.c
 *      Purpose : kernel memory (de)allocation routines
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#include <assert.h>
#include <io/tty.h>
#include <mem/alloc.h>
#include <mem/mem.h>
#include <mem/paging.h>
#include <stddef.h>
#include <stdint.h>

extern void *end;

extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern void *placement_address;
extern heap_t *kernel_heap;

/*
 * ---------------------------------------------------------------------------
 *      Name   : _kmalloc
 *      Purpose: internal kernel memory allocator implementation
 *      Args ---
 *        size: size_t
 *          - size to allocate
 *        align: unsigned char
 *          - page alignment flag
 *        phys: void **
 *          - allocation's physical address pointer
 *      Returns: void *
 * ---------------------------------------------------------------------------
 */
void *_kmalloc(size_t size, unsigned char align, void **phys) {
  if (kernel_heap) {
    void *addr = khalloc(size, (uint8_t)align, kernel_heap);
    if (phys) {
      *phys = map_to_physical(addr);
    }
    return addr;
  }

  if (align && ((unsigned int)placement_address & 0xFFF)) {
    placement_address =
        (void *)((unsigned int)placement_address + 0x1000 & ~0xFFF);
  }

  if (phys) {
    *phys = placement_address;
  }

  void *addr = placement_address;
  placement_address += size;
  return addr;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : kmalloc
 *      Purpose: kernel malloc
 *      Args ---
 *        size: size_t
 *          - size to allocate
 *      Returns: void *
 * ---------------------------------------------------------------------------
 */
void *kmalloc(size_t size) { return _kmalloc(size, 0, NULL); }

/*
 * ---------------------------------------------------------------------------
 *      Name   : kmalloc_a
 *      Purpose: aligned kernel malloc
 *      Args ---
 *        size: size_t
 *          - size to allocate
 *      Returns: void *
 * ---------------------------------------------------------------------------
 */
void *kmalloc_a(size_t size) { return _kmalloc(size, 1, NULL); }

/*
 * ---------------------------------------------------------------------------
 *      Name   : kmalloc_p
 *      Purpose: physical address kernel malloc
 *      Args ---
 *        size: size_t
 *          - size to allocate
 *        phys: void **
 *          - allocation's physical address pointer
 *      Returns: void *
 * ---------------------------------------------------------------------------
 */
void *kmalloc_p(size_t size, void **phys) { return _kmalloc(size, 0, phys); }

/*
 * ---------------------------------------------------------------------------
 *      Name   : kmalloc_ap
 *      Purpose: aligned physical address kernel malloc
 *      Args ---
 *        size: size_t
 *          - size to allocate
 *        phys: void **
 *          - allocation's physical address pointer
 *      Returns: void *
 * ---------------------------------------------------------------------------
 */
void *kmalloc_ap(size_t size, void **phys) { return _kmalloc(size, 1, phys); }

/*
 * ---------------------------------------------------------------------------
 *      Name   : kfree
 *      Purpose: kernel free
 *      Args ---
 *        p: void *
 *          - Memory address to free
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void kfree(void *p) {
  if (kernel_heap) khfree(p, kernel_heap);
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : map_to_physical
 *      Purpose: map a virtual memory address to a physical one
 *      Args ---
 *        virt: void *
 *          - Virtual Memory address to map
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void *map_to_physical(void *virt) {
  unsigned int remaining = (unsigned int)virt % 0x1000;
  unsigned int frameidx = (unsigned int)virt / 0x1000;
  unsigned int tableidx = frameidx / 0x1000;
  frameidx &= 0x3ff;

  if (current_directory->tables[tableidx]) {
    return (void *)(current_directory->tables[tableidx]->pages[frameidx].frame *
                        0x1000 +
                    remaining);
  }

  return NULL;
}
