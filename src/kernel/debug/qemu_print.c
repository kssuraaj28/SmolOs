#include <stdarg.h>
#include <stdint.h>
#include <driver/serial.h>

void qemu_puts(char* string)
{
  while (*string)
  {
    write_serial(*string);
    string++;
  }
}


