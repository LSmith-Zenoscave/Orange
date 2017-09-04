/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              LibC: The Orange C Standard Library Implementation
 *
 *      File    : abort.c
 *      Purpose : system abnormal exit
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */
#include <stdlib.h>

/*
 * ---------------------------------------------------------------------------
 *      Name   : abort
 *      Purpose: Abort current execution
 *      Args --- void
 *      Returns: size_t
 * ---------------------------------------------------------------------------
 */
void abort(void) {
  while(1)
    asm ("hlt");
}
