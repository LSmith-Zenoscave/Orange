#include <stdio.h>

#if defined(__is_os2_kernel)
#include <io/tty.h>
#endif

int putchar(int ic)
{
#if defined(__is_os2_kernel)
  char c = (char) ic;
	terminal_write(&c, sizeof(c));
#else
  // TODO: implement a write system call.
#endif
  return ic;
}
