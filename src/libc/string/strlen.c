/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              LibC: The Orange C Standard Library Implementation
 *
 *      File    : strlen.c
 *      Purpose : character string length counter
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */
#include <string.h>

/*
 * ---------------------------------------------------------------------------
 *      Name   : strlen
 *      Purpose: find the length of a null-terminated string
 *      Args ---
 *        s: const char *
 *          - string to find the length
 *      Returns: size_t
 * ---------------------------------------------------------------------------
 */
size_t strlen(const char *s) {
  const char *s2 = s;
  while (*s2++)
    ;
  return s2 - s;
}
