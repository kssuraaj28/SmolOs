#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdint.h>

void* memset(void* buf, int value, size_t len);
char* int2hex(uint32_t input);

#endif
