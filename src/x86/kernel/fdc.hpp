#pragma once
enum FloppyType
{
	noFloppyDrive = 0,
	Floppy360__5_25 = 1,
	Floppy1200_5_25 = 2,
	Floppy720__3_5 = 3,
	Floppy1440_3_5 = 4,
	Floppy2880_3_5 = 5
};
struct floppyDrv
{
	FloppyType type;
	unsigned short SectorsPerTrack;
};

enum FloppyRegisters
{
	STATUS_REGISTER_A = 0x3F0, // read-only
	STATUS_REGISTER_B = 0x3F1, // read-only
	DIGITAL_OUTPUT_REGISTER = 0x3F2,
	TAPE_DRIVE_REGISTER = 0x3F3,
	MAIN_STATUS_REGISTER = 0x3F4, // read-only
	DATARATE_SELECT_REGISTER = 0x3F4, // write-only
	DATA_FIFO = 0x3F5,
	DIGITAL_INPUT_REGISTER = 0x3F7, // read-only
	CONFIGURATION_CONTROL_REGISTER = 0x3F7  // write-only
};
int initfDrive(int n, floppyDrv* f);

