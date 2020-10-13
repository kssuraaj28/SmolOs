#ifndef DRIVER_VGATEXT_H
#define DRIVER_VGATEXT_H

enum vga_color {
         BLACK = 0,
         BLUE = 1,
         GREEN = 2,
         CYAN = 3,
         RED = 4,
         MAGENTA = 5,
         BROWN = 6,
         LIGHT_GREY = 7,
         DARK_GREY = 8,
         LIGHT_BLUE = 9,
         LIGHT_GREEN = 10,
         LIGHT_CYAN = 11,
         LIGHT_RED = 12,
         LIGHT_MAGENTA = 13,
         LIGHT_BROWN = 14,
         WHITE = 15,
 };      

extern char _vga_text_memory[];

void vga_init();
void set_fg_color(enum vga_color c);
void set_bg_color(enum vga_color c);
void vga_clrscr();
void vga_puts(char* Message);
void vga_putc(char x);
void set_cursor(uint16_t offset);
uint16_t get_cursor();
#endif
