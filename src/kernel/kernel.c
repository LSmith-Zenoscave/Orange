#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <io/tty.h>

void kernel_early(void *mboot, unsigned int magic, unsigned int *ebp)
{
  initialize_terminal();
  terminal_writestring("[TTY]   ... \x1b[32mDONE\n\x1b[00m");
}

void main()
{
}
