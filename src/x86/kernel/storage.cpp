#include"storage.hpp"
#include"pcdevice.hpp"
#include"fdc.hpp"
#include"mmngr.hpp"
floppyDrv masterFloppy;
floppyDrv slaveFloppy;
int initStorage(unsigned long long*, int*)
{
	//Floppy drives
	unsigned char RegisterFloppy = getCmosRegister(0x10);
	slaveFloppy.type = static_cast<FloppyType>(RegisterFloppy & 0xF);
	masterFloppy.type = static_cast<FloppyType>(RegisterFloppy >> 4);
	if (masterFloppy.type!=noFloppyDrive)
	{
		initfDrive(0, &masterFloppy);
	}
	if (slaveFloppy.type != noFloppyDrive)
	{
		initfDrive(0, &slaveFloppy);
	}
	return 0;
}

int deinitStorage()
{
	//Turn off floppy motor
	return 0;
}
