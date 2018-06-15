#include"video.hpp"
#include"hwio.hpp"
unsigned short vtx;
unsigned short vty;
unsigned long long bpcol;
volatile unsigned char* VideoBuffer;
inline void setchvt(unsigned char chr, unsigned x, unsigned y)
{
	unsigned int val = x + y*vtx;
	switch (bpcol)
	{
	case 0x10:
		(reinterpret_cast<volatile unsigned short*>(VideoBuffer))[val] = chr | (0x0F00);
		return;
	}
}
void svtcur(unsigned char x, unsigned char y)
{
	const unsigned short val = x + y*vtx;
	outb(0x3D4, 14);
	outb(0x3D5, static_cast<unsigned char>(val>>8));
	outb(0x3D4, 15);
	outb(0x3D5, static_cast<unsigned char>(val));
}
int videoinit(unsigned long long* attr, int*)
{
	VideoBuffer = reinterpret_cast<volatile unsigned char*>(attr[0]);
	bpcol = attr[1];
	vtx = attr[2];
	vty = attr[3];
	switch (bpcol)
	{
		case 0x10:
		{
			const size_t total = vtx * vty;
			for (int i = 0; i < total; i++)
			{
				(reinterpret_cast<volatile unsigned short*>(VideoBuffer))[i] = ' ' | (0x0F00);
			}
			break;
		}
	}
	outb(0x3D4, 0x0A);
	outb(0x3D5,(inb(0x3D5)&0xC0)|12);//inb(0x3D5)&0xC0 due to bits 6-7
	outb(0x3D4, 0x0B);
	outb(0x3D5,(inb(0x3D5)&0xE0)|13);
	svtcur(0, 0);
	return 0;
}
