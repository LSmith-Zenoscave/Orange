#ifndef _KERNEL_UTIL_H
#define _KERNEL_UTIL_H 1

#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

void outb(unsigned short port, unsigned char value);

void outw(unsigned short port, unsigned short value);

void outl(unsigned short port, unsigned int value);

unsigned char inb(unsigned short port);

unsigned short inw(unsigned short port);

unsigned int inl(unsigned short port);

void io_wait();

unsigned char one_count(unsigned int value);
#endif
