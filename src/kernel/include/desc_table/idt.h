#ifndef DESC_TABLE_IDT_H
#define DESC_TABLE_IDT_H

#include <stdint.h>

#define MAX_INTERRUPT 256
#define IDT_DESC_TRAP 0x01	//00000001
#define IDT_DESC_BIT16 0x06	//00000110
#define IDT_DESC_BIT32 0x0E	//00001110
#define IDT_DESC_RING1 0x40	//01000000
#define IDT_DESC_RING2 0x20	//00100000
#define IDT_DESC_RING3 0x60	//01100000
#define IDT_DESC_PRESENT 0x80	//10000000

typedef uint32_t reg32_t;
typedef uint32_t seg16_t;

typedef struct __attribute__ ((__packed__)) idtr {
	uint16_t		limit;
	uint32_t		base;
}idtr_t;

typedef struct __attribute__ ((__packed__)) idt_descriptor {
uint16_t		baseLo;
uint16_t		sel;
uint8_t			reserved;
uint8_t			flags;
uint16_t		baseHi;
}idt_descriptor_t;


extern idt_descriptor_t _idt[];
extern idtr_t _idtr;

void install_idt_ir(uint32_t index,uint16_t flags, uint16_t sel, uint32_t* handler_address);
void idt_init();
#endif
