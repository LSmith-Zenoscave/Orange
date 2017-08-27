/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              LibC: The Orange C Standard Library Implementation
 *
 *      File    : assert.h
 *      Purpose : basic assertion utility
 *
 *      Notes   : aborts on failure if DEBUGing is available
 *      Author  : Luke Smith
 * ===========================================================================
 */

#ifndef _ASSERT_H
#define _ASSERT_H 1

#include <stdio.h>
#include <stdlib.h>

#ifdef NDEBUG
#define assert(e) ((void)0)
#else
#define assert(e)  \
    ((void) ((e) ? 0 : __assert (#e, __FILE__, __LINE__)))
#define __assert(e, file, line) \
    ((void)printf ("%s:%u: failed assertion `%s'\n", file, line, e), abort())
#endif

#endif
