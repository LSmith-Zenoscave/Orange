#ifndef _KERNEL_PANIC_H
#define _KERNEL_PANIC_H 1

#define PANIC(msg) panic((msg), __FILE__, __LINE__);
void panic(const char *message, const char *file, unsigned int line);

#endif
