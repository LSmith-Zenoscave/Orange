/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : panic.h
 *      Purpose : kernel panic routines
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#ifndef _KERNEL_PANIC_H
#define _KERNEL_PANIC_H 1

#define PANIC(msg) panic((msg), __FILE__, __LINE__);
void panic(const char *message, const char *file, unsigned int line);

#endif
