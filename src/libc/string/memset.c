/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              LibC: The Orange C Standard Library Implementation
 *
 *      File    : memset.c
 *      Purpose : contiguous memory setting
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */
#include <string.h>

/*
 * ---------------------------------------------------------------------------
 *      Name   : memset
 *      Purpose: set a contiguous chunk of memory to a given value
 *      Args ---
 *        buffer: void *
 *          - memory destination
 *        value: int
 *          - value to use
 *        size: size_t
 *          - length of memory to set
 *      Returns: void *
 * ---------------------------------------------------------------------------
 */
void *memset(void *buffer, int value, size_t size)
{
  if(size)
  {
    char *d = buffer;
    do {
      *d++ = value;
    } while(--size);
  }
  return buffer;
}
