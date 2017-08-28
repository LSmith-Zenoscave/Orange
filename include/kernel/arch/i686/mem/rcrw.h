/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : rcrw.h
 *      Purpose : Control Register I/O declarations
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#ifndef _KERNEL_RWCR_H
#define _KERNEL_RWCR_H 1

#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

extern unsigned int read_cr0();
extern void write_cr0(unsigned int);
extern unsigned int read_cr2();
extern unsigned int read_cr3();
extern void write_cr3(void *);
extern unsigned int read_cr4();
extern void write_cr4(unsigned int);

#endif
