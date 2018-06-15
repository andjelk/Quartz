#include "hwio.hpp"

void outb(unsigned short port, unsigned char value)
{
	_asm
	{
		mov dx, ds:[port]
		mov al, ds:[value]
		out dx, al
	}
}

unsigned char inb(unsigned short port)
{
	unsigned char retv;
	_asm
	{
		mov dx, ds:[port]
		in al, dx
		mov ds:[retv], al
	}
	return retv;
}
