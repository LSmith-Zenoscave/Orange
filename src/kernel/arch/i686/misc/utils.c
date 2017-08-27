#include <stddef.h>
#include <stdint.h>
#include <misc/utils.h>

void outb(unsigned short port, unsigned char val)
{
  __asm__ __volatile__ ("outb %b0, %w1" : : "a"(val), "Nd"(port));
}

void outw(unsigned short port, unsigned short val)
{
  __asm__ __volatile__ ("outw %w0, %w1" : : "a"(val), "Nd"(port));
}

void outl(unsigned short port, unsigned int val)
{
  __asm__ __volatile__ ("outl %0, %w1" : : "a"(val), "Nd"(port));
}

unsigned char inb(unsigned short port)
{
  unsigned char ret;
  __asm__ __volatile__ ("inb %w1, %b0" : "=a" (ret) : "dN" (port));
  return ret;
}

unsigned short inw(unsigned short port)
{
  unsigned short ret;
  __asm__ __volatile__ ("inw %w1, %w0" : "=a" (ret) : "dN" (port));
  return ret;
}

unsigned int inl(unsigned short port)
{
  unsigned int ret;
  __asm__ __volatile__ ("inl %w1, %0" : "=a" (ret) : "dN" (port));
  return ret;
}

void io_wait()
{
  __asm__ __volatile__ ( "outb %%al, $0x80" : : "a"(0) );
}

unsigned char one_count(unsigned int value)
{
  int cnt = 0;
  while(value)
  {
    cnt += value & 0x1;
    value >>= 1;
  }
  return cnt;
}
