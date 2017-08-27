#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H 1

#include <cpu/isr.h>

typedef struct page_entry_struct
{
  unsigned int present  :1;
  unsigned int rw       :1;
  unsigned int user     :1;
  unsigned int accessed :1;
  unsigned int dirty    :1;
  unsigned int unused   :7;
  unsigned int frame    :20;
} page_entry_t;

typedef struct page_table_struct
{
  page_entry_t pages[1024];
} page_table_t;

typedef struct page_directory_struct
{
  unsigned int *tables_physical[1024];
  page_table_t *tables[1024];
  void *physical_addr;
} page_directory_t;

void alloc_frame(page_entry_t *page, unsigned char is_kernel, unsigned char is_writeable);
void free_frame(page_entry_t *page);

void initialize_paging(unsigned int memsize);

void switch_page_directory(page_directory_t *new_directory);
page_entry_t *get_page(const unsigned int *address, unsigned char make, page_directory_t *directory);
void *general_protection_fault(registers_t *registers);
void *page_fault(registers_t *registers);

#endif
