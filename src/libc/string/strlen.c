#include <string.h>

size_t strlen(const char* s)
{
  const char* s2 = s;
  while ( *s2++ );
  return s2 - s;
}
