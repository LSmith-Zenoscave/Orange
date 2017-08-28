/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : multiboot.h
 *      Purpose : multiboot standard specifications
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#ifndef _KERNEL_MULTIBOOT_H
#define _KERNEL_MULTIBOOT_H 1

#if !defined(__cpluscplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

#define MULTIBOOT_MAGIC 0x1BADB002
#define MULTIBOOT_EAX_MAGIC 0x1BADB002
#define MULTIBOOT_FLAG_ALIGN 0x001
#define MULTIBOOT_FLAG_MEMINFO 0x002
#define MULTIBOOT_FLAG_CMDLINE 0x004
#define MULTIBOOT_FLAG_MODS 0x008
#define MULTIBOOT_FLAG_AOUT 0x010
#define MULTIBOOT_FLAG_ELF 0x020
#define MULTIBOOT_FLAG_MMAP 0x040
#define MULTIBOOT_FLAG_CONFIG 0x080
#define MULTIBOOT_FLAG_LOADER 0x100
#define MULTIBOOT_FLAG_APM 0x200
#define MULTIBOOT_FLAG_VBE 0x400

typedef struct multiboot_struct_t {
  unsigned int flags;
  unsigned int mem_lower;
  unsigned int mem_upper;
  unsigned int boot_device;
  unsigned int cmdline;
  unsigned int mods_count;
  unsigned int mods_addr;
  unsigned int num;
  unsigned int size;
  unsigned int addr;
  unsigned int shndx;
  unsigned int mmap_length;
  unsigned int mmap_addr;
  unsigned int drives_length;
  unsigned int drives_addr;
  unsigned int config_table;
  unsigned int boot_loader_name;
  unsigned int apm_table;
  unsigned int vbe_control_info;
  unsigned int vbe_mode_info;
  unsigned int vbe_mode;
  unsigned int vbe_interface_seg;
  unsigned int vbe_interface_off;
  unsigned int vbe_interface_len;
} multiboot_t;

#endif
