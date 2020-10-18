#include <stdint.h>
#include <port_io.h>
#include <driver/pic.h>
#include <interrupt/generic.h>

#define INPUT_CLOCK_FREQUENCY 1193180
#define TIMER_CONTROL_PORT 0x43
#define TIMER_DATA_PORT 0x40

volatile uint32_t tick_counter = 0;

void pit_handler(interrupt_frame_t* frame)
{
	tick_counter ++;
	irq_ack(frame -> vector_number);
}
void set_frequency(uint16_t h)
{
    uint16_t divisor = INPUT_CLOCK_FREQUENCY / h;
    // Init, Square Wave Mode, non-BCD, first transfer LSB then MSB
    outportb(TIMER_CONTROL_PORT, 0x36);
    outportb(TIMER_DATA_PORT, divisor & 0xFF);
    outportb(TIMER_DATA_PORT, (divisor >> 8) & 0xFF);

}

void pit_init()
{
	set_frequency(100);
	register_interrupt_handler(IRQ_BASE + IRQ0_TIMER, pit_handler);
}
