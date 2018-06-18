#include"fdc.hpp"
struct chs
{
	unsigned short head;
	unsigned short cylinder;
	unsigned short sector;
};
void lbachs(unsigned lba, chs *buf, floppyDrv* f)
{
	buf->cylinder = lba / (2 * f->SectorsPerTrack);
	buf->head = ((lba % (2 * f->SectorsPerTrack)) / f->SectorsPerTrack);
	buf->sector = ((lba % (2 * f->SectorsPerTrack)) % f->SectorsPerTrack + 1);
}
int initfDrive(int n, floppyDrv* f)
{
	if (f->type == Floppy1440_3_5)
	{
		f->SectorsPerTrack = 18;
	};

	return 0;
}
