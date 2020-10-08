#include <stdint.h>

#include <multiboot.h>
#include <desc_table/gdt.h>
#include <mem/paging.h>
#include <driver/serial.h>
#include <debug/qemu_print.h>



void kmain(multiboot_info_t* mbd, uint32_t magic)
{
  (void)mbd;
  (void)magic;

  gdt_init();
  serial_init();
  for (;;);
}
