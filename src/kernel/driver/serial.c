#include <driver/serial.h>
#include <port_io.h>

#include <stdint.h>

#define COM_1_IO 0x3F8

void serial_init() {
   outportb(COM_1_IO + 1, 0x00);    // Disable all interrupts
   outportb(COM_1_IO + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outportb(COM_1_IO + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outportb(COM_1_IO + 1, 0x00);    //                  (hi byte)
   outportb(COM_1_IO + 3, 0x03);    // 8 bits, no parity, one stop bit
   outportb(COM_1_IO + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outportb(COM_1_IO + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

static int is_transmit_empty() {
   return inportb(COM_1_IO + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
   outportb(COM_1_IO,a);
}

static int serial_received() {
   return inportb(COM_1_IO + 5) & 1;
}

char read_serial() {
   while (serial_received() == 0);
   return inportb(COM_1_IO);
}
