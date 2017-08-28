/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              LibC: The Orange C Standard Library Implementation
 *
 *      File    : stdio.h
 *      Purpose : C Standard I/O routines
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);

#ifdef __cplusplus
}
#endif

#endif
