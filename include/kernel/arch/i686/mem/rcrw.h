#ifndef _KERNEL_RWCR_H
#define _KERNEL_RWCR_H 1

#if !defined(__cplusplus)
#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>

extern unsigned int read_cr0();
extern void 	write_cr0(unsigned int);
extern unsigned int read_cr2();
extern unsigned int	read_cr3();
extern void 	write_cr3(void *);
extern unsigned int	read_cr4();
extern void 	write_cr4(unsigned int);


#endif
