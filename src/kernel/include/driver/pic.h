#ifndef DRIVER_PIC_H
#define DRIVER_PIC_H

void pic_init();
void send_EOI_master();
void send_EOI_slave();

#endif
