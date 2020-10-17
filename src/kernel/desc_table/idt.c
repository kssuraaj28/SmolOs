#include <common.h>
#include <debug/panic.h>
#include <desc_table/idt.h>


idt_descriptor_t _idt[MAX_INTERRUPT];
idtr_t _idtr;

static void dummy_handler()
{
	PANIC ("Undefined interrupt");
}

void install_idt_ir(uint32_t index,uint16_t flags, uint16_t sel, uint32_t* handler_address)
{
	if (index >=MAX_INTERRUPT) return;

	_idt[index].baseLo = (uint32_t)handler_address & 0xffff;
	_idt[index].baseHi = ((uint32_t)handler_address >> 16) & 0xffff;
	_idt[index].reserved = 0;
	_idt[index].flags = flags;
	_idt[index].sel = sel;
}

void idt_init()
{
	_idtr.base = (uint32_t)_idt;
	_idtr.limit = (sizeof (idt_descriptor_t) * MAX_INTERRUPT) -1 ;
	
	memset (_idt,0,sizeof(_idt));

	for (int i=0;i<MAX_INTERRUPT;i++)
		install_idt_ir(i,IDT_DESC_BIT32 | IDT_DESC_PRESENT, 0x08, (uint32_t*) dummy_handler);
	asm volatile ("lidt %[idtr] \n"::[idtr]"m"(_idtr));
}
