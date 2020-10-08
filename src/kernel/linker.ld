OUTPUT_FORMAT (elf32-i386)
ENTRY(_start)
 
SECTIONS
{
__kernel_pma = 0x100000;
__kernel_vma = 0xC0100000;

	. = __kernel_vma;
 
	.text ALIGN(4K) :  AT (ADDR(.text) - (__kernel_vma - __kernel_pma))
	{
		*(.multiboot)
		*(.text)
	}
 
	.data ALIGN(4K) : AT (ADDR(.data) - (__kernel_vma - __kernel_pma))
	{
		*(.rodata)
		*(.data)
	}
 
	.bss ALIGN(4K) : AT (ADDR(.bss) - (__kernel_vma - __kernel_pma))
	{
		*(COMMON)
		*(.bss)
	}

  /DISCARD/     : {     
        *(.eh_frame);
        *(.comment);
    }
}
