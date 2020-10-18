#ifndef DRIVER_PIT_H
#define DRIVER_PIT_H
#include <stdint.h>

extern uint32_t tick_counter;

void pit_init();
void set_frequency(uint16_t h);

#endif
