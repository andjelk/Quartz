#pragma once
#include"deviceproc.hpp"
int initPcStdRoutines(extendDevInfo *ex);
unsigned char getCmosRegister(unsigned r);
#define _IRQ_PARAM (unsigned eflags, unsigned cs, unsigned eip, unsigned error)
typedef void(*inthandler)_IRQ_PARAM;
void setint(unsigned n, void* handler, unsigned char attrib);
#define PRESENTIDTE 0b10000000
extern inline void intend(unsigned);
extern unsigned _tckc[3];
unsigned getTicks();
enum GateType
{
	GateTask = 0x15,
	GateInterrupt = 0xE,
	GateTrap = 0xF
};
#define DEFIRQATTR (GateInterrupt|PRESENTIDTE)
