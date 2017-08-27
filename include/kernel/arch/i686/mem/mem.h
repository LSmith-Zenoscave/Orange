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
