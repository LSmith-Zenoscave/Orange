/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : mem.h
 *      Purpose : memory (de)allocation routine declarations
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#ifndef _KERNEL_MEM_H
#define _KERNEL_MEM_H 1

#include <stddef.h>

void *_kmalloc(size_t size, unsigned char align, void **physical);
void *kmalloc(size_t size);
void *kmalloc_a(size_t size);
void *kmalloc_p(size_t size, void **physical);
void *kmalloc_ap(size_t size, void **physical);

void kfree(void *p);
void *map_to_physical(void *);

#endif
