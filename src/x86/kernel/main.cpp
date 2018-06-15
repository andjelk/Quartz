
extern void _cdecl initglobal();
#include"deviceproc.hpp"
#include"video.hpp"
#include<string.hpp>
#include<stdio.hpp>
#include"power.hpp"
#include"pcdevice.hpp"
#include"mmngr.hpp"
#include"storage.hpp"
#include"syscall.hpp"
#include<math.hpp>
_deverr res;
#define PHYSICAL_MEMORY_SIZE 4096
int PCX86_PAR[1] = { PHYSICAL_MEMORY_SIZE };
unsigned long long VGA_DEV_ATTR[] = { 0xB8000, 0x10, 80, 25 };
#pragma warning(disable:4731)
#define DEFAULT_OUTPUT_DEVICE VGATEXTMODE
void main()
{
	_asm
	{
		mov ax, 0x08
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax
		mov esp, 0x7c00
		mov ebp, esp
	}
	initglobal();
	extern devinfo *loadeddev;
	memset(loadeddev, 0, sizeof(devinfo)*MAXDEVICES);
	installdev(PCX86, initPcStdRoutines, 0, PCX86_PAR, 0);
	installdev(VGATEXTMODE, videoinit, 0, 0, VGA_DEV_ATTR);
	installdev(PCSTORAGE, initStorage, deinitStorage, 0, 0);
	//installdev(FAULTDEVICE, faultdev, 0, 0);
	memset(&res, 0, sizeof(_deverr));
	initdevices(&res);
	setint(0x90, sysCallIntHandler, DEFIRQATTR);
	if (devinstallstate(DEFAULT_OUTPUT_DEVICE) != CONFIGURED_AND_INSTALLED)
	{
		//stdout=...uh...
		//Nevermind. Just use other output devices, for now devices such as, black hole radiation.
	}
	puts("Quartz Operating System\n");
	if (res.count)//Check if there is errors
	{
		printf("[Fatal Error] >>> Initialization of following %d device/s failed:\n", res.count);
		for (size_t i = 0; i < res.count; i++)
		{
			printf("Device : %x - Reported error code : %d\n", res.err[i].__dev, res.err[i].__err);
		}
	}
	//TODO:
	//Something with printf() or va_list is really bad, e.g. corrupts stack.
	//Floating point results to invalid opcode.
	//Execution
	//Something like RunFile("A:\system\qshell", arg_foo, arg_bar)...
	//Deinitalization routines, kernel exiting.
	//_asm int 85
inf:
	extern unsigned cvtxp, cvtyp;
	cvtxp = 10;
	cvtyp = 10;
	printf("Current tick : %d", _tckc[0]);
	goto inf;
	powerprocedure(res.count);
}