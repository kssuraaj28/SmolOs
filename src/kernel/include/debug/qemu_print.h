#ifndef DEBUG_QEMU_PRINT_H
#define DEBUG_QEMU_PRINT_H
#include <stdint.h>

void qemu_puts(char* string);
void qemu_hex(uint32_t);

#endif 
