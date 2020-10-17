#ifndef _INTERRUPT_GENERIC_H
#define _INTERRUPT_GENERIC_H
#include<stdint.h>

typedef uint32_t reg32_t;
typedef uint32_t seg16_t;

/** Interrupt frame*/
typedef struct interrupt_frame {
	seg16_t gs;
	seg16_t fs;
	seg16_t es;
	seg16_t ds;

	reg32_t edi;
	reg32_t esi;
	reg32_t ebp;
	reg32_t esp; //Ignore this
	reg32_t ebx;
	reg32_t edx;
	reg32_t ecx;
	reg32_t eax;

	uint32_t vector_number;
	uint32_t error_code;
	reg32_t eip;
	seg16_t cs;
	uint32_t flag;
}interrupt_frame_t;


#define IRQ_BASE                0x20
#define IRQ0_Timer              0x00
#define IRQ1_Keyboard           0x01
#define IRQ2_CASCADE            0x02
#define IRQ3_SERIAL_PORT2       0x03
#define IRQ4_SERIAL_PORT1       0x04
#define IRQ5_RESERVED           0x05
#define IRQ6_DISKETTE_DRIVE     0x06
#define IRQ7_PARALLEL_PORT      0x07
#define IRQ8_CMOS_CLOCK         0x08
#define IRQ9_CGA                0x09
#define IRQ10_RESERVED          0x0A
#define IRQ11_RESERVED          0x0B
#define IRQ12_AUXILIARY         0x0C
#define IRQ13_FPU               0x0D
#define IRQ14_HARD_DISK         0x0E
#define IRQ15_RESERVED          0x0F

typedef void (*specific_isr_t) (interrupt_frame_t *);
extern specific_isr_t _spec_isr_table[];

void interrupt_init();
#endif

