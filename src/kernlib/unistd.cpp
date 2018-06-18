#include "unistd.hpp"

bool waitEnd;
/*
	Sleeps for one hundredth of a second
*/
void sleep(size_t t)
{
	_asm
	{
		xchg bx, bx
		xor eax, eax
		int 0x90
		mov edx, eax
		mov ecx, [t]
		add ecx, edx
		_loop:
		cmp ecx, edx
		jle outfc
		xor eax, eax
		push ecx
		hlt
		int 0x90
		mov edx, eax
		pop ecx
		jmp _loop
		outfc:
	}
}
