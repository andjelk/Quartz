
extern void _cdecl initglobal();
#include"deviceproc.hpp"
#include"video.hpp"
#include<string.hpp>
#include<krnlio.hpp>
#include<stdio.hpp>
#include"power.hpp"
#include"pcdevice.hpp"
#include"mmngr.hpp"
#include"storage.hpp"
#include<stdio.hpp>
#include"syscall.hpp"
#include<math.hpp>
_deverr res;
unsigned int PHYSICAL_MEMORY_SIZE = 4096;
unsigned long long VGA_DEV_ATTR[] = { 0xB8000, 16, 80, 25 };
unsigned long long PC_DEV_ATTR[] = { PHYSICAL_MEMORY_SIZE };
#pragma warning(disable:4731)
#define DEFAULT_OUTPUT_DEVICE VGATEXTMODE
void _checkDefOutput()
{
	static extendDevInfo def_out_dev;
	if (getDev(DEFAULT_OUTPUT_DEVICE, &def_out_dev)->devState != CONFIGURED_AND_INSTALLED)
	{
		//stdout=...uh...
		//Nevermind. Just use other output devices, for now devices such as, black hole radiation.
	}
}
#include<unistd.hpp>
void main()
{
	_asm
	{
		xchg bx, bx
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
	extern volatile devinfo *loadeddev;
	memset((void*)loadeddev, 0, sizeof(devinfo)*MAXDEVICES);
	configDev(VGATEXTMODE, videoinit, 0, VGA_DEV_ATTR);
	configDev(PCX86, initPcStdRoutines, 0, PC_DEV_ATTR);
	memset(&res, 0, sizeof(_deverr));
	initdevices(&res);
	refreshStdioStream();
	if (!res.count)
	{
		configDev(PCSTORAGE, (int(*)(extendDevInfo*))(initStorage), deinitStorage, 0);
		initdevices(&res);
	}
	_checkDefOutput();
	puts("Quartz Operating System");
	if (res.count)
	{
		printf("[Fatal Error] >>> Initialization of following %d device/s failed:\n", res.count);
		for (size_t i = 0; i < res.count; i++)
		{
			extendDevInfo *__di = getDev(res.err->__dev, __di);
			if (__di->devName)
				printf("%s (Device-ID:%x) - Reported error code : %x\n", __di->devName, res.err[i].__dev, res.err[i].__err);
			else printf("Device-ID:%x - Reported error code : %x\n", res.err[i].__dev, res.err[i].__err);
		}
	}
	else
	{
		//TODO:
		//Read blocks from floppy

		//Execution

		//Something like RunFile("A:\system\qshell", arg_foo, arg_bar)...

	}
	//Deinitalization routines, kernel exiting.
	powerprocedure(res.count);
}
