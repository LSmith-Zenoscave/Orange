/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : ps2.h
 *      Purpose : kernel ps2 routines
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#ifndef _KERNEL_PS2_H
#define _KERNEL_ps2_H 1

#define PS2_DATA 0x60
#define PS2_COMMAND 0x64

#define PS2_OUTPUT_FULL 0x01

void initialize_ps2(void);

#endif
