#include"fdc.hpp"
void lba_2_chs(unsigned lba, unsigned short* cyl, unsigned short* head, unsigned short* sector, floppyDrv* f)
{
	*cyl = lba / (2 * f->SectorsPerTrack);
	*head = ((lba % (2 * f->SectorsPerTrack)) / f->SectorsPerTrack);
	*sector = ((lba % (2 * f->SectorsPerTrack)) % f->SectorsPerTrack + 1);
}
int initfDrive(int n, floppyDrv* f)
{
	if (f->type == Floppy1440_3_5)
	{
		f->SectorsPerTrack = 18;
	};

	return 0;
}
