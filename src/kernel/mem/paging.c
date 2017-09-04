/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : paging.c
 *      Purpose : kernel virtual memory paging
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
#include <mem/rcrw.h>
#include <misc/panic.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

page_directory_t *kernel_directory = NULL;
page_directory_t *current_directory = 0;

unsigned int *frames;
unsigned int frames_no;

extern unsigned int *placement_address;
extern heap_t *kernel_heap;

#define INDEX_FROM_BIT(bit) ((bit) / 0x20)
#define OFFSET_FROM_BIT(bit) ((bit) % 0x20)

/*
 * ---------------------------------------------------------------------------
 *      Name   : set_frame
 *      Purpose: mark a frame as active in the directory
 *      Args ---
 *        addr: unsigned int
 *          - frame address to mark
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
static void set_frame(unsigned int addr) {
  if (addr < frames_no * 0x1000) {
    unsigned int frame = addr / 0x1000;
    unsigned int idx = INDEX_FROM_BIT(frame);
    unsigned int off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
  }
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : clear_frame
 *      Purpose: mark a frame as inactive in the directory
 *      Args ---
 *        addr: unsigned int
 *          - frame address to mark
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
static void clear_frame(unsigned int addr) {
  unsigned int frame = addr / 0x1000;
  unsigned int idx = INDEX_FROM_BIT(frame);
  unsigned int off = OFFSET_FROM_BIT(frame);
  frames[idx] &= ~(0x1 << off);
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : first_frame
 *      Purpose: find the first free frame in the directory
 *      Args --- void
 *      Returns: unsigned int
 * ---------------------------------------------------------------------------
 */
static unsigned int first_frame(void) {
  for (unsigned int frame_table = 0; frame_table < INDEX_FROM_BIT(frames_no);
       frame_table++) {
    if (frames[frame_table] != 0xFFFFFFFF) {
      // at least one frame is free in the table
      for (unsigned int frame_index = 0; frame_index < 0x20; frame_index++) {
        if (!(frames[frame_table] & (0x1 << frame_index))) {
          return frame_table * 0x20 + frame_index;
        }
      }
    }
  }
  return 0xFFFFFFFF;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : alloc_frame
 *      Purpose: add a frame to the page
 *      Args ---
 *        page: page_entry_t *
 *          - base page to add the frame to
 *        is_kernel: unsigned char
 *          - kernel ownership flag
 *        is_writable: unsigned char
 *          - read-write flag
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void alloc_frame(page_entry_t *page, unsigned char is_kernel,
                 unsigned char is_writable) {
  if (page->frame == 0) {
    unsigned int frame = first_frame();
    if (frame == 0xFFFFFFFF) {
      PANIC("No free frame");
      return;
    }

    set_frame(frame * 0x1000);
    page->frame = frame;
  }

  page->present = 1;
  page->rw = (is_writable) ? 1 : 0;
  page->user = (is_kernel) ? 0 : 1;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : adirect_frame
 *      Purpose: add a frame to the page directly
 *      Args ---
 *        page: page_entry_t *
 *          - base page to add the frame to
 *        is_kernel: unsigned char
 *          - kernel ownership flag
 *        is_writable: unsigned char
 *          - read-write flag
 *        addr: void *
 *          - memory frame address
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void direct_frame(page_entry_t *page, unsigned char is_kernel,
                  unsigned int is_writable, void *addr) {
  page->present = 1;
  page->rw = (is_writable) ? 1 : 0;
  page->user = (is_kernel) ? 0 : 1;
  page->frame = (unsigned int)addr / 0x1000;

  set_frame((unsigned int)addr);
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : free_frame
 *      Purpose: remove a frame from the page directly
 *      Args ---
 *        page: page_entry_t *
 *          - base page to remove
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void free_frame(page_entry_t *page) {
  if (page->frame != 0) {
    clear_frame(page->frame);
    page->frame = 0;
  }
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : initialize_paging
 *      Purpose: create kernel paging layout and start handlers
 *      Args ---
 *        memsze: unsigned int
 *          - available memory
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void initialize_paging(unsigned int memsize) {
  frames_no = memsize / 4;
  frames =
      (unsigned int *)kmalloc(INDEX_FROM_BIT(frames_no) * sizeof(unsigned int));

  assert(frames);

  memset(frames, 0, INDEX_FROM_BIT(frames_no));

  void *physical;
  kernel_directory =
      (page_directory_t *)kmalloc_ap(sizeof(page_directory_t), &physical);
  memset(kernel_directory, 0, sizeof(page_directory_t));
  current_directory = kernel_directory;

#if 1
  get_page(0, 1, kernel_directory)->present = 0;
  set_frame(0);
  for (unsigned int *idx = (unsigned int *)0x1000; idx < placement_address + 0x3000;
       idx += 0x1000)
#else
  for (unsigned int *idx = 0; idx < placement_address + 0x3000; idx += 0x1000)
#endif
  {
    direct_frame(get_page(idx, 1, kernel_directory), 1, 0, idx);
  }

  kernel_directory->physical_addr = kernel_directory->tables_physical;

  unsigned int *heap_start = (unsigned int *)KERNEL_HEAP_START;
  if (heap_start <= placement_address + 0x3000) {
    heap_start = placement_address + 0x3000;
  }

  for (unsigned int *idx = placement_address + 0x3000; idx < heap_start;
       idx += 0x1000) {
    alloc_frame(get_page(idx, 1, kernel_directory), 1, 0);
  }

  for (unsigned int *idx = heap_start; idx <= (unsigned int *)KERNEL_HEAP_END; idx += 0x1000) {
    alloc_frame(get_page(idx, 1, kernel_directory), 0, 0);
  }

  asm("cli");

  register_isr_handler(13, general_protection_fault);
  register_isr_handler(14, page_fault);

  asm("sti");

  kernel_heap =
      create_heap((unsigned int *)KERNEL_HEAP_START, (unsigned int *)KERNEL_HEAP_END, 0, 0);
  switch_page_directory(kernel_directory);
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : switch_page_directory
 *      Purpose: switch current page directory
 *      Args ---
 *        directory: page_directory_t *
 *          - new directory to use
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void switch_page_directory(page_directory_t *directory) {
  write_cr3(&directory->tables_physical);
  write_cr4(read_cr4() | 0x00000010);  // set PSE;
  write_cr0(read_cr0() | 0xB0000000);  // set PG;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : get_page
 *      Purpose: get or create a page in a directory
 *      Args ---
 *        address: unsigned int *
 *          - address to retrieve
 *        make: unsigned char
 *          - Creation on missing flag
 *        directory: page_directory_t *
 *          - directory to search
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
page_entry_t *get_page(const unsigned int *address, unsigned char make,
                       page_directory_t *directory) {
  unsigned int frame = (unsigned int)address / 0x1000;
  unsigned int table_idx = frame / 1024;
  if (directory->tables[table_idx]) {
    return &directory->tables[table_idx]->pages[frame % 1024];
  }
  if (make) {
    void *tmp;
    directory->tables[table_idx] =
        (page_table_t *)kmalloc_ap(sizeof(page_table_t), &tmp);
    memset(directory->tables[table_idx], 0, sizeof(page_table_t));
    tmp = (void *)((unsigned int)tmp | 0x7);
    directory->tables_physical[table_idx] = tmp;
    return &directory->tables[table_idx]->pages[frame % 1024];
  }

  return NULL;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : general_protection_fault
 *      Purpose: Protection fault handler
 *      Args ---
 *        registers: registers_t *
 *          - register states at time of the call
 *      Returns: void *
 * ---------------------------------------------------------------------------
 */
void *general_protection_fault(registers_t *registers) {
  terminal_writestring("Protection fault");
  if (registers->err_code) {
    unsigned int external = registers->err_code & 0x1;
    unsigned int table = (registers->err_code & 0x6) >> 1;
    unsigned int index = (registers->err_code & 0xF8) >> 3;
    if (external) {
      terminal_writestring(" of external origin");
    }
    terminal_writestring("!");
    if (table & 0x1) {
      terminal_writestring(" From IDT @ index 0x");
    } else if (table & 0x2) {
      terminal_writestring(" From LDT @ index 0x");
    } else {
      terminal_writestring(" From GDT @ index 0x");
    }
    terminal_writehex(index);
    terminal_writestring("\n");
  } else {
    terminal_writestring("!\n");
  }
  PANIC("General Protection Fault");
  return NULL;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : page_fault
 *      Purpose: Page fault handler
 *      Args ---
 *        registers: registers_t *
 *          - register states at time of the call
 *      Returns: void *
 * ---------------------------------------------------------------------------
 */
void *page_fault(registers_t *regs) {
  unsigned int err_code = regs->err_code;
  bool present = !(err_code & 0x1);  // Page not present
  unsigned int rw = err_code & 0x2;  // Write operation?
  unsigned int us = err_code & 0x4;  // Processor was in user-mode?
  unsigned int reserved =
      err_code & 0x8;  // Overwritten CPU-reserved bits of page entry?
  //unsigned int id = err_code & 0x10;  // Caused by an instruction fetch?
  terminal_writestring("Page fault! ( ");
  if (present) terminal_writestring("present ");
  if (rw) terminal_writestring("read-only ");
  if (us) terminal_writestring("user-mode ");
  if (reserved) terminal_writestring("reserved ");
  terminal_writestring(") at 0x");
  terminal_writehex(read_cr2());
  terminal_writestring("\n");
  PANIC("Page fault");
  return NULL;
}
