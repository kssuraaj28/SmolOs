#include <stdint.h>

static char buffer[11]  = "0x00000000";

char* int2hex(uint32_t input)
{
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
  return (buffer+pointer);
}

