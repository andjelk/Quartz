#include"stdlib.hpp"
#include"string.hpp"

int _itoa(unsigned int value, char* buf, unsigned base)
{
	unsigned i = 0;
	while (value)
	{
		/*
		65003
		3
		*/
		unsigned t = value % base;
		if (t > 9)
		{
			t += 'A' - 10;
		}
		else
		{
			t += '0';
		}
		buf[i++] = (char)t;
		value /= base;
		_asm
		{
			push eax
			push ecx
			mov ecx, [t]
			mov eax, [buf]
			xchg bx, bx
			pop ecx
			pop eax
		}
	}
	buf[i] = 0;
	_asm xchg bx, bx
	strrev(buf);
	return i;
}
const char* itoa(int value, char* buf, unsigned base)
{
	if (value < 0)
	{
		int r = _itoa(-value, buf + 1, base);
		*buf = '-';
	}
	else if (!value)
	{
		buf[0] = '0';
		buf[1] = 0;
	}
	else _itoa(value, buf, base);
	return buf;
}

