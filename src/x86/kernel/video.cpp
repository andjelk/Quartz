#include"video.hpp"
#include"deviceproc.hpp"
#include"hwio.hpp"
unsigned short vtx;
unsigned short vty;
#define cur_vt (videoDeviceFile.position)
unsigned int max_vt;
#define cvtxp (cur_vt%vtx)
#define cvtyp (cur_vt/vtx)
unsigned short bpcol;
volatile unsigned char* VideoBuffer;
bool _moveCursor=false;
void staticCursor(bool v)
{
	if (v != _moveCursor)return;
	if (v)
	{
		outb(0x3D4, 0x0A);
		outb(0x3D5, 0x20);
	}
	else
	{
		outb(0x3D4, 0x0A);
		outb(0x3D5, (inb(0x3D5) & 0xC0) | 12);//inb(0x3D5)&0xC0 due to bits 6-7
		outb(0x3D4, 0x0B);
		outb(0x3D5, (inb(0x3D5) & 0xE0) | 13);
	}
	_moveCursor = !v;
}
void set_v_term_cur(unsigned short index)
{
	if (!_moveCursor)return;
	outb(0x3D4, 14);
	outb(0x3D5, static_cast<unsigned char>(index>>8));
	outb(0x3D4, 15);
	outb(0x3D5, static_cast<unsigned char>(index));
}
FILE videoDeviceFile;
const char devNm[] = "VGA text-mode terminal";
int videoinit(extendDevInfo *ex)
{
	VideoBuffer = reinterpret_cast<volatile unsigned char*>(ex->devAttr[0]);
	bpcol = ex->devAttr[1];
	vtx = ex->devAttr[2];
	vty = ex->devAttr[3];
	max_vt = vtx*vty;
	switch (bpcol)
	{
		case 0x10:
		{
			for (int i = 0; i < max_vt; i++)
			{
				(reinterpret_cast<volatile unsigned short*>(VideoBuffer))[i] = ' ' | (0x0F00);
			}
			break;
		}
	}
	staticCursor(false);
	set_v_term_cur(0);
	ex->devName = devNm;
	videoDeviceFile.id = VGATEXTMODE;
	videoDeviceFile.buffer = (void*)VideoBuffer;
	videoDeviceFile.position = 0;
	videoDeviceFile.special = true;
	videoDeviceFile.parent = &DeviceFile;
	ex->devFile = &videoDeviceFile;
	return 0;
}
void v_term_write_i_ex(char character, unsigned int i)
{
	switch (bpcol)
	{
	case 0x10:
		(reinterpret_cast<volatile unsigned short*>(VideoBuffer))[i] = character | (0x0F00);
		return;
	}
}
void v_term_write_i(char character, unsigned short x, unsigned short y)
{
	unsigned int index = x + y*vtx;
	v_term_write_i_ex(character, index);
}
#include<string.hpp>
void v_term_scroll()
{
	unsigned __bpc = bpcol / 8;
	unsigned __bpl = vtx*__bpc;
	unsigned __cleb = max_vt*__bpc - __bpl;
	cur_vt = 24 * vtx;
	memcpy((void*)(VideoBuffer), (void*)(VideoBuffer + __bpl), __cleb);
	for (unsigned i = max_vt-vtx; i < max_vt; i++)
	{
		(reinterpret_cast<volatile unsigned short*>(VideoBuffer))[i] = ' ' | (0x0F00);
	}
}
void v_term_write(char character)
{
	switch (character)
	{
	case '\n':
		cur_vt = ((cur_vt / vtx)+1)*vtx;
		break;
	case '\t':
		cur_vt += 4;
	case '\b':
		if(cur_vt)cur_vt--;
		//backspace;
		break;
	default:
		v_term_write_i_ex(character, cur_vt++);
	}
	if (cur_vt == max_vt)
	{
		v_term_scroll();
	}
	set_v_term_cur(cur_vt);
}
