#include <port_io.h>
#include <debug/panic.h>
#include <interrupt/generic.h>
#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

#define PIC_EOI 0x20

void pic_init()
{
	
	//ICW 1  Expect IC4|single?|0|level?|init?|000
	outportb(PIC1_COMMAND,0x11);
	outportb(PIC2_COMMAND,0x11);
	
	//ICW 2  Remapping the IRQs
	outportb(PIC1_DATA,IRQ_BASE);
	outportb(PIC2_DATA,IRQ_BASE+IRQ8_CMOS_CLOCK);

	// Send ICW 3 to primary PIC
	// 0x4 = 0100 Second bit (IR Line 2)
	outportb(PIC1_DATA,0x04);

	// Send ICW 3 to secondary PIC
	// 010=> IR line 2
	// write to data register of secondary PIC
	outportb(PIC2_DATA,0x02);

	// Send ICW 4 - Set x86 mode --------------------------------
	// bit 0 enables 80x86 mode
	outportb(PIC1_DATA,0x01);
	outportb(PIC2_DATA,0x01);
 
	
	// Setting the IMR - All interrupts enabled
	outportb(PIC1_DATA,0);
	outportb(PIC2_DATA,0);
}
void irq_ack(uint8_t irq_no)
{
	if (irq_no < IRQ_BASE) PANIC ("Wrong IRQ number!");

	outportb(PIC1_COMMAND,0x20);
	if (irq_no >= IRQ_BASE + IRQ8_CMOS_CLOCK)
		outportb(PIC2_COMMAND,0x20);
}
