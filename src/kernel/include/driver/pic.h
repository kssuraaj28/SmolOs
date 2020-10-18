#ifndef DRIVER_PIC_H
#define DRIVER_PIC_H

#include <stdint.h>

void pic_init();
void irq_ack(uint8_t irq_no);

#endif
