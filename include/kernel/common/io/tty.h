#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H 1

#if !defined(__cplusplus)
#include <stdbool.h>
#endif

#include <stddef.h>
#include <stdint.h>

void initialize_terminal();
void terminal_clearscreen();
void terminal_putchar(char c);
void terminal_write(const char *data, size_t size);
void terminal_writestring(const char *data);
void terminal_writehex(const unsigned int data);
#endif
