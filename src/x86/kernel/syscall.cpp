#include "syscall.hpp"
#include "video.hpp"
#include "mmngr.hpp"
#include "storage.hpp"
#include "pcdevice.hpp"
#include "deviceproc.hpp"

void* sysCalls[] =
{
	getTicks, kmalloc, kfree, getDev, storWriteByte, storReadByte, storWriteFile, storReadFile, getDevFile
};
/*
Parameters are being passed by registers in following order:
EBX being the first paramater,
ECX the second,
and so on for the EDX, EDI and ESI.
*/
const size_t maxSysCall = sizeof(sysCalls)/sizeof(unsigned)-1;
_declspec(naked) void sysCallIntHandler()
{
	_asm
	{
		push esi
		push edi
		push edx
		push ecx
		push ebx
		cmp eax, [maxSysCall]
		jg goaw
		lea eax, [sysCalls+eax*4]
		call [eax]
		add esp, 20
		goaw:iretd
	}
}
