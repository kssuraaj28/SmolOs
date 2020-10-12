#include <stdarg.h>
#include <stdint.h>
#include <driver/serial.h>

void qemu_puts(char* string)
{
  while (*string)
  {
    write_serial(*string);
    string++;
  }
}


void qemu_hex(uint32_t input)
{
	char buffer[11]  = "0x00000000";
	uint8_t pointer = 9;
	for (int i = 0; i < 8; i++)
	{
		int c = input & 0xF;
		if (c < 10)
			c += 0x30;
		else
			c += 55;
		buffer[pointer] = (char)c;
		pointer--;
		input >>= 4;
	}

	//This is to make the thing smaller
	pointer = 2;
	while((buffer[pointer] == '0')&& (pointer < 9)) pointer ++;
	pointer--;buffer[pointer] = 'x';
	pointer--;buffer[pointer] = '0';
	qemu_puts(buffer + pointer);
}
