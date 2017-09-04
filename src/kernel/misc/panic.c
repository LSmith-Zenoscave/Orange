/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : panic.c
 *      Purpose : kernel panic routine
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#include <misc/panic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/*
 * ---------------------------------------------------------------------------
 *      Name   : panic
 *      Purpose: halt and catch fire
 *      Args ---
 *        message: const char *
 *          - message to print
 *        file: const char *
 *          - source file that caused the panic
 *        line: unsigned int
 *          - line number of the panic source
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void panic(const char *message, const char *file, unsigned int line) {
  __asm__ __volatile__("cli"); // Disable interrupts.
  printf("\n%s:%d: %s", file, line, message);
  for (;;)
    ;
}
