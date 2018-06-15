#include "power.hpp"
extern void _cdecl exitglobal();
#include"deviceproc.hpp"
void poweroff()
{
	//Not implemented.
	_asm
	{
		cli
		hlt; HLT with IF = 0
	}
}
void powerprocedure(bool halt)
{
	deinitdevices();
	if (halt)
	{
		_asm
		{
			cli
			hlt;HLT with IF=0
		}
	}
	else
	{
		exitglobal();
		poweroff();
	}
}
