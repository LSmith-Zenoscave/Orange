#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <mem/alloc.h>
#include <mem/mem.h>
#include <mem/paging.h>
#include <mem/rcrw.h>
#include <misc/panic.h>
#include <io/tty.h>

page_directory_t *kernel_directory = NULL;
page_directory_t *current_directory = 0;

unsigned int *frames;
unsigned int frames_no;

extern void *placement_address;
extern heap_t *kernel_heap;

#define INDEX_FROM_BIT(bit) ((bit) / 0x20)
#define OFFSET_FROM_BIT(bit) ((bit) % 0x20)

static void set_frame(unsigned int addr)
{
  if (addr < frames_no * 0x1000)
  {
    unsigned int frame = addr / 0x1000;
    unsigned int idx = INDEX_FROM_BIT(frame);
    unsigned int off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
  }
}

static void clear_frame(unsigned int addr)
{
  unsigned int frame = addr / 0x1000;
  unsigned int idx = INDEX_FROM_BIT(frame);
  unsigned int off = OFFSET_FROM_BIT(frame);
  frames[idx] &= ~(0x1 << off);
}

static unsigned int test_frame(unsigned int addr)
{
  unsigned int frame = addr / 0x1000;
  unsigned int idx = INDEX_FROM_BIT(frame);
  unsigned int off = OFFSET_FROM_BIT(frame);
  return frames[idx] & (0x1 << off);
}

static unsigned int first_frame()
{
  for(unsigned int frame_table = 0; frame_table < INDEX_FROM_BIT(frames_no); frame_table++)
  {
    if (frames[frame_table] != 0xFFFFFFFF)
    {
      // at least one frame is free in the table
      for(unsigned int frame_index = 0; frame_index < 0x20; frame_index++)
      {
        if ( !(frames[frame_table] & (0x1 << frame_index)))
        {
          return frame_table * 0x20 + frame_index;
        }
      }
    }
  }
  return 0xFFFFFFFF;
}

void alloc_frame(page_entry_t *page, unsigned char is_kernel, unsigned char is_writeable)
{
  if (page->frame == 0) {

    unsigned int frame = first_frame();
    if(frame == 0xFFFFFFFF)
    {
      PANIC("No free frame");
      return;
    }

    set_frame(frame * 0x1000);
    page->frame = frame;

  }

  page->present = 1;
  page->rw = (is_writeable) ? 1 : 0;
  page->user = (is_kernel) ? 0 : 1;
}

void direct_frame(page_entry_t *page, unsigned char is_kernel, unsigned int is_writeable, void *addr)
{
  page->present = 1;
  page->rw  = (is_writeable) ? 1 : 0;
  page->user = (is_kernel) ? 0 : 1;
  page->frame = (unsigned int)addr / 0x1000;

  set_frame((unsigned int)addr);
}

void free_frame(page_entry_t *page)
{
  if (page->frame != 0)
  {
    clear_frame(page->frame);
    page->frame = 0;
  }
}

void initialize_paging(unsigned int memsize)
{
  frames_no = memsize / 4;
  frames = (unsigned int *)kmalloc(INDEX_FROM_BIT(frames_no) * sizeof(unsigned int));

  assert (frames);

  memset(frames, 0, INDEX_FROM_BIT(frames_no));

  void *physical;
  kernel_directory = (page_directory_t *)kmalloc_ap(sizeof(page_directory_t), &physical);
  memset(kernel_directory, 0, sizeof(page_directory_t));
  current_directory = kernel_directory;

#if 1
  get_page(0, 1, kernel_directory)->present = 0;
  set_frame(0);
  for(void * idx = (void *)0x1000; idx < placement_address+0x3000; idx += 0x1000)
#else
  for(void * idx = (void *)0; idx < placement_address+0x3000; idx += 0x1000)
#endif
  {
    direct_frame(get_page(idx, 1, kernel_directory), 1, 0, idx);
  }

  kernel_directory->physical_addr = (void *)kernel_directory->tables_physical;

  void *heap_start = (void *)KERNEL_HEAP_START;
  if (heap_start <= placement_address + 0x3000)
  {
    heap_start = placement_address + 0x3000;
  }

  for (void * idx = placement_address + 0x3000; idx < heap_start; idx += 0x1000)
  {
    alloc_frame(get_page(idx, 1, kernel_directory), 1, 0);
  }

  for (void * idx = heap_start; idx <= (void *)KERNEL_HEAP_END; idx += 0x1000)
  {
    alloc_frame(get_page(idx, 1, kernel_directory), 0, 0);
  }

  register_isr_handler(13, general_protection_fault);
  register_isr_handler(14, page_fault);

  kernel_heap = create_heap((void *)KERNEL_HEAP_START, (void *)KERNEL_HEAP_END, 0, 0);
  switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *directory)
{
  write_cr3(&directory->tables_physical);
  write_cr4(read_cr4() | 0x00000010); // set PSE;
  write_cr0(read_cr0() | 0xB0000000); // set PG;
}

page_entry_t *get_page(const unsigned int *address, unsigned char make, page_directory_t* directory)
{
  unsigned int frame = (unsigned int)address / 0x1000;
  unsigned int table_idx = frame / 1024;
  if(directory->tables[table_idx])
  {
    return &directory->tables[table_idx]->pages[frame % 1024];
  }
  if (make)
  {
    void *tmp;
    directory->tables[table_idx] = (page_table_t *)kmalloc_ap(sizeof(page_table_t), &tmp);
    memset(directory->tables[table_idx], 0, sizeof(page_table_t));
    tmp = (void *)((unsigned int)tmp | 0x7);
    directory->tables_physical[table_idx] = tmp;
    return &directory->tables[table_idx]->pages[frame % 1024];
  }

  return NULL;
}

void *general_protection_fault(registers_t *registers)
{
  terminal_writestring("Protection fault");
  if(registers->err_code)
  {
    unsigned int external = registers->err_code & 0x1;
    unsigned int table = (registers->err_code & 0x6) >> 1;
    unsigned int index = (registers->err_code & 0xF8) >> 3;
    if (external) {
      terminal_writestring(" of external origin");
    }
    terminal_writestring("!");
    if (table & 0x1) {
      terminal_writestring(" From IDT @ index 0x");
    }
    else if (table & 0x2) {
      terminal_writestring(" From LDT @ index 0x");
    }
    else {
      terminal_writestring(" From GDT @ index 0x");
    }
    terminal_writehex(index);
    terminal_writestring("\n");
  }
  else {
    terminal_writestring("!\n");
  }
  PANIC("General Protection Fault");
  return NULL;
}

void *page_fault(registers_t *regs)
{
  unsigned int err_code = regs->err_code;
  bool present   = !(err_code & 0x1); // Page not present
  unsigned int rw = err_code & 0x2;		   // Write operation?
  unsigned int us = err_code & 0x4;		   // Processor was in user-mode?
  unsigned int reserved = err_code & 0x8;	 // Overwritten CPU-reserved bits of page entry?
  unsigned int id = err_code & 0x10;		  // Caused by an instruction fetch?
  terminal_writestring("Page fault! ( ");
  if (present)	terminal_writestring("present ");
  if (rw)			terminal_writestring("read-only ");
  if (us)			terminal_writestring("user-mode ");
  if (reserved)   terminal_writestring("reserved ");
  terminal_writestring(") at 0x");
  terminal_writehex(read_cr2());
  terminal_writestring("\n");
  PANIC("Page fault");
  return NULL;
}
