#include <stdint.h>

#include <port_io.h>
#include <mem/paging.h>
#include <driver/vgatext.h>

#define ROW 80
#define COL 25
#define VGA_TEXT 0xB8000

char _vga_text_memory[80*25*2] __attribute__((aligned(PAGE_SIZE)));

static char current_color = 0xf8; 

#define CRT_INDEX_PORT 0x3D4
#define CRT_DATA_PORT 0x3D5
#define CURSOR_INDEX_LO 0x0F
#define CURSOR_INDEX_HI 0x0E
void set_cursor(uint16_t offset)
{
  uint8_t lo = offset & 0xFF;
  uint8_t hi = offset >> 8;

  outportb(CRT_INDEX_PORT,CURSOR_INDEX_LO);
  outportb(CRT_DATA_PORT,lo);

  outportb(CRT_INDEX_PORT,CURSOR_INDEX_HI);
  outportb(CRT_DATA_PORT,hi);
}

uint16_t get_cursor()
{
  uint8_t lo,hi;
  outportb(CRT_INDEX_PORT,CURSOR_INDEX_LO);
  lo = inportb(CRT_DATA_PORT);

  outportb(CRT_INDEX_PORT,CURSOR_INDEX_HI);
  hi = inportb(CRT_DATA_PORT);

  return (uint16_t)hi<<8|lo;
}

void set_fg_color(enum vga_color c)
{
	current_color &= 0xf0;
	current_color |= c;
}
void set_bg_color(enum vga_color c)
{
	current_color &= 0x0f;
	current_color |= (c << 4);
}

void vga_init()
{
//  FREE_K(_vga_text_memory);
  MAP_K(_vga_text_memory,0xB8000);
  vga_clrscr();
}
void vga_clrscr() 
{
	char* cursor = _vga_text_memory;
	while((uint32_t)(cursor-_vga_text_memory)<2*ROW*COL)
		{
			*cursor = 0;
			cursor[1] = current_color; cursor+=2;
		}
	set_cursor(0);
}
void vga_puts(char* Message)
{
	uint32_t pointer = get_cursor();
	pointer <<= 1;
	char* vga_cursor = (char*) _vga_text_memory;
	vga_cursor += pointer;
	while (*Message)
	{
		char c = *Message;
		switch (c)
		{
		 case '\n':
			pointer = (uint32_t)(vga_cursor - _vga_text_memory);
			pointer -= (pointer % (2 * ROW));
			pointer += (2 * ROW);
			if (pointer == 2 * ROW * COL) //This is to control overflow
				pointer = 0;
			vga_cursor = (char *)(_vga_text_memory + pointer);
			break;
		case '\t':
			pointer=(uint32_t)(vga_cursor-_vga_text_memory);
			pointer+=16;
			vga_cursor=(char*)(_vga_text_memory+pointer);
			break;
		default:
			*vga_cursor = *Message;
			vga_cursor++;
			*vga_cursor = current_color;
			vga_cursor++;
		}
		Message++;
	}
	//Resetting the pointer
	vga_cursor -= (uint32_t)_vga_text_memory;
	pointer = (uint32_t)vga_cursor;
	pointer >>= 1;
	set_cursor(pointer);
}

void vga_putc(char x) 
{
	uint32_t pointer = get_cursor();
	char* vga_cursor = _vga_text_memory;
	switch (x){
	case '\b':
		if (pointer)
		{
			pointer--;
			vga_cursor += (pointer << 1);
			*vga_cursor = 0;
			vga_cursor[1] = vga_cursor[3];
			set_cursor(pointer);
		}
		break;
	case '\n':
			 pointer=pointer-(pointer%ROW)+ROW;
			 if(pointer==ROW*COL)
			 	pointer=0;
			 set_cursor(pointer);
		break;
	default:
		vga_cursor += (pointer << 1);
		vga_cursor[1] = (char)current_color;
		*vga_cursor = x;
		set_cursor(pointer + 1);
	}
}
