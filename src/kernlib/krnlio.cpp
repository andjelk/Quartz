#include"krnlio.hpp"

unsigned long long _getDevAttr(unsigned dev, unsigned n)
{
	unsigned long long _ret;
	_asm
	{
		mov eax, 4
		mov ebx, [dev]
		mov ecx, [n]
		int 0x90
		mov dword ptr[_ret], eax
		mov dword ptr[_ret+4], edx
	}
	return _ret;
}
