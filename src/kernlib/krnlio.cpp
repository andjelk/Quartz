#include"krnlio.hpp"
#include"stdio.hpp"
FILE *_getDevFile(unsigned dev, FILE *buf)
{
	FILE *_ret;
	_asm
	{
		mov eax, 8
		mov ebx, [dev]
		mov ecx, [buf]
		int 0x90
		mov dword ptr[_ret], eax
	}
	return _ret;
}
