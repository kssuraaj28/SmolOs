#include <stdint.h>

#include <common.h>
#include <multiboot.h>
#include <desc_table/gdt.h>
#include <mem/pfa.h>
#include <mem/paging.h>
#include <driver/serial.h>
#include <driver/vgatext.h>
#include <driver/pit.h>
#include <driver/pci.h>
#include <interrupt/generic.h>
#include <debug/qemu_print.h>
#include <debug/panic.h>



void kmain(multiboot_info_t* mbd, uint32_t magic)
{
  (void)magic;
  serial_init();
  gdt_init();
  pfa_init(mbd);
  paging_init();
  vga_init();
  pit_init();
  interrupt_init();
  for(;;);
//  PANIC("We are done");
}
