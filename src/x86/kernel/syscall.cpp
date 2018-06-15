#include "syscall.hpp"
#include "video.hpp"
#include "mmngr.hpp"
#include "deviceproc.hpp"
void* sysCalls[] =
{
	setchvt, svtcur, k_phys_malloc, k_phys_free, getDevAttr, devinstallstate
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
