#ifndef MEM_PFA_H
#define MEM_PFA_H
#include <multiboot.h>
#include <stdint.h>

extern uint32_t _kernel_size;

void pfa_init(multiboot_info_t *mbd);
uint32_t first_free_block();
uint32_t alloc_page_frame();
void free_page_frame(uint32_t address);
#endif
