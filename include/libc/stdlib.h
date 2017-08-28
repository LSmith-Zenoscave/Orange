/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              LibC: The Orange C Standard Library Implementation
 *
 *      File    : stdlib.h
 *      Purpose : C Standard Library
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <stddef.h>
#include <sys/cdefs.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifdef NULL
#undef NULL
#define NULL 0
#endif

__attribute__((__noreturn__)) void abort(void);
void *malloc(size_t size);

#ifdef __cplusplus
}
#endif

#endif
