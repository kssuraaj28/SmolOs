#include <desc_table/idt.h>
#include <interrupt/generic.h>
#include <driver/pic.h>
#include <debug/panic.h>
#include <debug/qemu_print.h>


void isr0(); void isr1(); void isr2(); void isr3(); void isr4(); void isr5(); void isr6(); void isr7(); void isr8(); 
void isr9(); void isr10(); void isr11(); void isr12(); void isr13(); void isr14(); void isr15(); void isr16(); 
void isr17(); void isr18(); void isr19(); void isr20(); void isr21(); void isr22(); void isr23(); void isr24();
void isr25(); void isr26(); void isr27(); void isr28(); void isr29(); void isr30(); void isr31(); void isr32(); 
void isr33(); void isr34(); void isr35(); void isr36(); void isr37(); void isr38(); void isr39(); void isr40(); 
void isr41(); void isr42(); void isr43(); void isr44(); void isr45(); void isr46(); void isr47(); 
void isr128();

specific_isr_t _spec_isr_table[MAX_INTERRUPT] = {0};

void interrupt_init()
{
	idt_init();
	install_idt_ir(0,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr0);
	install_idt_ir(1,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr1);
	install_idt_ir(2,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr2);
	install_idt_ir(3,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr3);
	install_idt_ir(4,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr4);
	install_idt_ir(5,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr5);
	install_idt_ir(6,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr6);
	install_idt_ir(7,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr7);
	install_idt_ir(8,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr8);
	install_idt_ir(10,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr10);
	install_idt_ir(11,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr11);
	install_idt_ir(12,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr12);
	install_idt_ir(13,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr13);
	install_idt_ir(14,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr14);
	install_idt_ir(16,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr16);
	
	install_idt_ir(32,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr32);
	install_idt_ir(33,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr33);
	install_idt_ir(34,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr34);
	install_idt_ir(35,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr35);
	install_idt_ir(36,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr36);
	install_idt_ir(37,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr37);

	install_idt_ir(46,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr46);
	install_idt_ir(47,IDT_DESC_BIT32|IDT_DESC_PRESENT,0x08,(uint32_t*)isr47);
	
	install_idt_ir(0x80,IDT_DESC_BIT32|IDT_DESC_PRESENT|IDT_DESC_RING3|IDT_DESC_TRAP,0x08,(uint32_t*)isr128); //This is syscall

	pic_init();
	asm volatile ("sti \n");
}

void generic_interrupt_handler(interrupt_frame_t input)
{
	if(_spec_isr_table[input.vector_number]) _spec_isr_table[input.vector_number](&input);
	if(input.vector_number < IRQ_BASE) 
	{
		qemu_puts("\nException number:"); QEMU_HEX(input.vector_number);
		qemu_puts("\nError code:"); QEMU_HEX(input.error_code);
		qemu_puts("\nProgram counter:"); QEMU_HEX(input.cs);qemu_puts(":");QEMU_HEX(input.eip);
		PANIC ("No specific interrupt handler");
	}
}
void register_interrupt_handler(uint8_t num, specific_isr_t handler) 
{
    _spec_isr_table[num] = handler;
}
