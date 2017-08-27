#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <cpu/gdt.h>
#include <cpu/tss.h>

static struct {
  gdt_entry_t entries[6];
  gdt_ptr_t   pointer;
  tss_entry_t tss;
} gdt __attribute__((used));

void initialize_gdt()
{

  gdt_ptr_t *gdt_ptr = &gdt.pointer;
  gdt_ptr->limit = sizeof gdt.entries - 1;
  gdt_ptr->base = (unsigned int *)&gdt.entries[0];

  fill_gdt_entry(0, (gdt_t){.base = 0, .limit = 0, .type = 0}, false);            // Null Segment
  fill_gdt_entry(1, (gdt_t){.base = 0, .limit = 0xFFFFFFFF, .type = 0x9A}, false); // Code Segment
  fill_gdt_entry(2, (gdt_t){.base = 0, .limit = 0xFFFFFFFF, .type = 0x92}, false); // Data Segment
  fill_gdt_entry(3, (gdt_t){.base = 0, .limit = 0xFFFFFFFF, .type =0xFA}, false); // User-Mode Code Segment
  fill_gdt_entry(4, (gdt_t){.base = 0, .limit = 0xFFFFFFFF, .type =0xF2}, false); // User-Mode Data segment
  write_tss(5, 0x10, 0x0);
  gdt_flush((unsigned int *)gdt_ptr);
  tss_flush();

}

static void fill_gdt_entry(unsigned int index, gdt_t source, bool is_tss)
{

  gdt.entries[index].granularity = 0x40;

  if (source.limit > 65536)
  {
    source.limit >>= 12;
    gdt.entries[index].granularity |= 0x80;
  }

  if (is_tss)
  {
    gdt.entries[index].granularity = 0x00;
  }

  // Set limit & granularity
  gdt.entries[index].limit_low = (unsigned short)(source.limit & 0xFFFF);
  gdt.entries[index].granularity |= (source.limit >> 16) & 0x0F;

  // Set Base
  gdt.entries[index].base_low = (unsigned short)((unsigned int)source.base & 0xFFFF);
  gdt.entries[index].base_middle = (unsigned char)(((unsigned int)source.base >> 16) & 0xFF);
  gdt.entries[index].base_high = (unsigned char)(((unsigned int)source.base >> 24) & 0xFF);

  // Set Access Type
  gdt.entries[index].access = source.type;

}

static void write_tss(unsigned int index, unsigned short ss0, unsigned int esp0)
{
  tss_entry_t *tss = &gdt.tss;
  unsigned int *base = (unsigned int *)tss;
  unsigned int *limit = base + sizeof *tss;

  fill_gdt_entry(index, (gdt_t){.base=base, .limit=(unsigned int)limit, .type=0xE9}, true);

  memset((void *)tss, 0x0, sizeof *tss);
  tss->ss0 = ss0;
  tss->esp0 = esp0;
  tss->cs = 0x0b;
  tss->ss = 0x13;
  tss->ds = 0x13;
  tss->es = 0x13;
  tss->fs = 0x13;
  tss->gs = 0x13;

  tss->iomap_base = sizeof *tss;
}
