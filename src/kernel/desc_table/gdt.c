#include <stdint.h>
#include <desc_table/gdt.h>

gdt_descriptor_t _gdt[GDT_DESCRIPTOR_COUNT];
gdtr_t _gdtr;

void gdt_set_descriptor(uint16_t i, uint32_t base, uint32_t limit, uint8_t access, uint8_t grand)
{ 	
	if (i > GDT_DESCRIPTOR_COUNT)
		return;

	_gdt[i].baseLo	= (uint16_t)(base & 0xffff);
	_gdt[i].baseMid	= (uint8_t)((base >> 16) & 0xff);
	_gdt[i].baseHi	= (uint8_t)((base >> 24) & 0xff);
	_gdt[i].limit	= (uint16_t)(limit & 0xffff);

	_gdt[i].flags = access;
	_gdt[i].grand = (uint8_t)((limit >> 16) & 0x0f);
	_gdt[i].grand |= grand & 0xf0;
}

void gdt_init() 
{

	_gdtr.limit = (sizeof (struct gdt_descriptor) * GDT_DESCRIPTOR_COUNT)-1;
	_gdtr.base = (uint32_t)&_gdt[0];

	gdt_set_descriptor(0, 0, 0, 0, 0);

	gdt_set_descriptor (1,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_EXEC_CODE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	gdt_set_descriptor (2,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	gdt_set_descriptor (3,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_EXEC_CODE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY|I86_GDT_DESC_DPL,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

	gdt_set_descriptor (4,0,0xffffffff,
		I86_GDT_DESC_READWRITE|I86_GDT_DESC_CODEDATA|I86_GDT_DESC_MEMORY|I86_GDT_DESC_DPL,
		I86_GDT_GRAND_4K | I86_GDT_GRAND_32BIT | I86_GDT_GRAND_LIMITHI_MASK);

  asm volatile (
      "lgdt (%0)\n"
      "mov $0x10,%%bx\n"
      "mov %%bx,%%ds\n"
      "mov %%bx,%%ss\n"
      "mov %%bx,%%es\n"
      "mov %%bx,%%fs\n"
      "mov %%bx,%%gs\n"
      "ljmp $0x08, $.dummy%= \n"
      ".dummy%= :\n"
      ::"r"(&_gdtr):"ebx");
}
