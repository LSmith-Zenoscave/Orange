/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              LibC: The Orange C Standard Library Implementation
 *
 *      File    : putchar.c
 *      Purpose : character placement
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */
#include <stdio.h>

#if defined(__is_orange_kernel)
#include <io/tty.h>
#endif

/*
 * ---------------------------------------------------------------------------
 *      Name   : putchar
 *      Purpose: place a character to the tty (or screen)
 *      Args ---
 *        ic: int
 *          - character to place
 *      Returns: int
 * ---------------------------------------------------------------------------
 */
int putchar(int ic) {
#if defined(__is_orange_kernel)
  char c = (char)ic;
  terminal_write(&c, sizeof(c));
#else
// TODO: implement a write system call.
#endif
  return ic;
}
