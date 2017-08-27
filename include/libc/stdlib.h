#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif


#ifdef NULL
#undef NULL
#define NULL 0
#endif

  __attribute__((__noreturn__))
  void abort(void);
  void *malloc(size_t size);

#ifdef __cplusplus
}
#endif

#endif
