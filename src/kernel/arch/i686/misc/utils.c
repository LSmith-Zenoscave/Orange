/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : utils.c
 *      Purpose : Miscellaneous Architecture specific utilities
 *
 *      Notes   : Only works with basic vga 80x25 console as of now
 *      Author  : Luke Smith
 * ===========================================================================
 */

#include <misc/utils.h>
#include <stddef.h>
#include <stdint.h>

/*
 * ---------------------------------------------------------------------------
 *      Name   : outb
 *      Purpose: output a byte over an I/O port
 *      Args ---
 *        port: unsigned short
 *          - port to use
 *        val: unsigned char
 *          - value to use
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void outb(unsigned short port, unsigned char val) {
  __asm__ __volatile__("outb %b0, %w1" : : "a"(val), "Nd"(port));
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : outw
 *      Purpose: output a word over an I/O port
 *      Args ---
 *        port: unsigned short
 *          - port to use
 *        val: unsigned sort
 *          - value to use
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void outw(unsigned short port, unsigned short val) {
  __asm__ __volatile__("outw %w0, %w1" : : "a"(val), "Nd"(port));
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : outl
 *      Purpose: output a byte double word over an I/O port
 *      Args ---
 *        port: unsigned short
 *          - port to use
 *        val: unsigned int
 *          - value to use
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void outl(unsigned short port, unsigned int val) {
  __asm__ __volatile__("outl %0, %w1" : : "a"(val), "Nd"(port));
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : inb
 *      Purpose: receive a byte from an I/O port
 *      Args ---
 *        port: unsigned short
 *          - port to use
 *      Returns: unsigned char
 * ---------------------------------------------------------------------------
 */
unsigned char inb(unsigned short port) {
  unsigned char ret;
  __asm__ __volatile__("inb %w1, %b0" : "=a"(ret) : "dN"(port));
  return ret;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : inw
 *      Purpose: receive a word from an I/O port
 *      Args ---
 *        port: unsigned short
 *          - port to use
 *      Returns: unsigned short
 * ---------------------------------------------------------------------------
 */
unsigned short inw(unsigned short port) {
  unsigned short ret;
  __asm__ __volatile__("inw %w1, %w0" : "=a"(ret) : "dN"(port));
  return ret;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : inl
 *      Purpose: receive a double word from an I/O port
 *      Args ---
 *        port: unsigned short
 *          - port to use
 *      Returns: unsigned int
 * ---------------------------------------------------------------------------
 */
unsigned int inl(unsigned short port) {
  unsigned int ret;
  __asm__ __volatile__("inl %w1, %0" : "=a"(ret) : "dN"(port));
  return ret;
}

/*
 * ---------------------------------------------------------------------------
 *      Name   : io_wait
 *      Purpose: wait for i/o to finish
 *      Args --- void
 *      Returns: void
 * ---------------------------------------------------------------------------
 */
void io_wait() { __asm__ __volatile__("outb %%al, $0x80" : : "a"(0)); }
