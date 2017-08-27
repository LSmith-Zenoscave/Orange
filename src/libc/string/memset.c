#include <string.h>

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
