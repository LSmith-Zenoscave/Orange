#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <misc/panic.h>
#include <io/tty.h>

void panic(const char *message, const char *file, unsigned int line)
{
  __asm__ __volatile__ ("cli"); // Disable interrupts.
  printf("\n%s:$s: %s",file, line, message);
  for(;;);
}
