#include <stdint.h>
#include <debug/qemu_print.h>

void panic(char* msg,char* file, uint32_t line)
{
  asm volatile ("cli\n");
  qemu_puts("\nPANIC: ");qemu_puts(msg);
  qemu_puts("\nFILE: ");qemu_puts(file);
  qemu_puts("\nLINE: ");QEMU_HEX(line);
  for (;;);
}
