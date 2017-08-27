#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <mem/mem.h>
#include <mem/paging.h>
#include <mem/alloc.h>
#include <io/tty.h>

extern void *end;

extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern void *placement_address;
extern heap_t *kernel_heap;

void *_kmalloc(size_t size, unsigned char align, void **phys)
{
  if (kernel_heap)
  {
    void *addr = khalloc(size, (uint8_t)align, kernel_heap);
    if (phys)
    {
      *phys = map_to_physical(addr);
    }
    return addr;
  }

  if(align && ((unsigned int)placement_address & 0xFFF))
  {
    placement_address = (void *)((unsigned int)placement_address + 0x1000 & ~0xFFF);
  }

  if(phys)
  {
    *phys = placement_address;
  }

  void *addr = placement_address;
  placement_address += size;
  return addr;
}


void *kmalloc(size_t size) { return _kmalloc(size, 0, NULL); }
void *kmalloc_a(size_t size) { return _kmalloc(size, 1, NULL); }
void *kmalloc_p(size_t size, void **phys) { return _kmalloc(size, 0, phys); }
void *kmalloc_ap(size_t size, void **phys) { return _kmalloc(size, 1, phys); }
void kfree(void * p) { if(kernel_heap) khfree(p, kernel_heap); }

void *map_to_physical(void *virt)
{
  unsigned int remaining = (unsigned int)virt % 0x1000;
  unsigned int frameidx = (unsigned int)virt / 0x1000;
  unsigned int tableidx = frameidx / 0x1000;
  frameidx &= 0x3ff;

  if(current_directory->tables[tableidx])
  {
    return (void *)(current_directory->tables[tableidx]->pages[frameidx].frame * 0x1000 + remaining);
  }

  return NULL;
}
