#include"storage.hpp"
#include"pcdevice.hpp"
#include"fdc.hpp"
#include"mmngr.hpp"
floppyDrv masterFloppy;
floppyDrv slaveFloppy;
int initStorage()
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
#include"deviceproc.hpp"
#include"video.hpp"
int storWriteByte(int character, FILE * stream)
{
	if (stream->special)
	{
		switch (stream->id)
		{
		case VGATEXTMODE:
			v_term_write(character);
			break;
		default:;
			//Undefined special file.
			return -3;
		}
	}
	else
	{
		//Actual file
		return -2; //Unimplemented
	}
	return 0;
}
int storReadByte(FILE * stream)
{
	if (stream->special)
	{
		switch (stream->id)
		{
		case VGATEXTMODE:
			return ((char*)(stream->buffer))[stream->position];
		default:;
			//Undefined special file.
			return -3;
		}
	}
	else
	{
		//Actual file
		return -2; //Unimplemented
	}
}
int storWriteFile(void * buf, unsigned int count, FILE * stream)
{
	if (stream->special)
	{
		switch (stream->id)
		{
		case VGATEXTMODE:
		{
			unsigned int i = 0;
			for (; i < count; i++)
			{
				v_term_write(((char*)(buf))[i]);
			}
			break;
		}
		default:;
			//Undefined special file.
			return -3;
		}
	}
	else
	{
		//Actual file
		return -2; //Unimplemented
	}
	return count;
}
#include<string.hpp>
int storReadFile(void * buf, unsigned int count, FILE * stream)
{
	if (stream->special)
	{
		switch (stream->id)
		{
		case VGATEXTMODE:
			memcpy(buf, (char*)(stream->buffer)+stream->position, count);
			break;
		default:;
			//Undefined special file.
			return -3;
		}
	}
	else
	{
		//Actual file
		return -2; //Unimplemented
	}
	return count;
}
