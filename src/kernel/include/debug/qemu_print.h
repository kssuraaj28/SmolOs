#ifndef DEBUG_QEMU_PRINT_H
#define DEBUG_QEMU_PRINT_H
#include <stdint.h>

#include <common.h>

#define QEMU_HEX(x) qemu_puts(int2hex(x))

void qemu_puts(char* string);

#endif 
