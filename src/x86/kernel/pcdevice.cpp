#include"pcdevice.hpp"
#include"mmngr.hpp"
#include"qgdef.def"
//Load descriptor tables
struct gdte
{
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
};
#define TTL_GDT_ENTRIES 3
const int gdts = TTL_GDT_ENTRIES * sizeof(gdte) / sizeof(unsigned);
unsigned gdt[gdts] = {
	/*
	Null descriptor :	Quartz uses this place to allocate GDTR.
	*/
	(gdts*sizeof(unsigned)-1)|(reinterpret_cast<unsigned short>(gdt)<<16), reinterpret_cast<unsigned>(gdt)>>16,
	/*
	Data segment 0x08
	*/
	0x0000FFFF, 0x00CF9200,
	/*
	Code segment 0x10
	*/
	0x0000FFFF, 0x00CF9A00
};
/*constexpr void _set_gdte(unsigned short n, unsigned int base, unsigned int limit, unsigned char access)
{
	gdt[n].base_low = base;
	gdt[n].base_middle = base >> 16;
	gdt[n].base_high = base >> 24;
	gdt[n].limit_low = limit;
	gdt[n].granularity = limit >> 16;
	gdt[n].access = access;
}*/
struct dtr
{
	short size;
	void* offset;
}idtr;
struct idte
{
	unsigned short offset_low;
	unsigned short selector;
	unsigned char reserved;
	unsigned char attributes;
	unsigned short offset_high;
};
#include<stdio.hpp>
void defIdtHandler(unsigned eflags, unsigned cs, unsigned eip, unsigned error)
{
	_asm
	{
		pusha
	}
	puts("[Warning] >>> Default Interrupt Handler Executed.\n");
#ifndef IGNORE_DEFAULT_IH
	_asm
	{
		cli
		hlt
	}
#endif
	_asm
	{
		popa
		iret
	}
}

#ifndef VECTOR_NOT_IMPLEMENTED
vector<idte> itable;
unsigned int lastIdte=256;
#else
idte itable[256];
const unsigned int lastIdte=256;
#endif
//32-bit IDT gate types.
#include"hwio.hpp"
#include<unistd.hpp>
unsigned char getCmosRegister(unsigned r)
{
	outb(0x70, 0x80 | r);
	sleep(25);
	return inb(0x71);
}
void setint(unsigned n, void* handler, unsigned char attrib)
{
	unsigned intHndl = reinterpret_cast<unsigned>(handler);
	idte* tc = (idte*)idtr.offset + n;
	tc->offset_low = (intHndl & 0xFFFF);
	tc->offset_high = (intHndl >> 16) & 0xFFFF;
	tc->selector = 0x10;
	tc->attributes = attrib;
}
#define x86_PIC1_COMMAND 0x20
#define x86_PIC1_STATUS 0x20
#define x86_PIC1_DATA 0x21
#define x86_PIC1_IMR 0x21
#define x86_PIC2_COMMAND 0xA0
#define x86_PIC2_STATUS	0xA0
#define x86_PIC2_DATA 0xA1
#define x86_PIC2_IMR 0xA1

//Erste initializatien worte 
#define x86_PIC_ICW1_MASK_IC4			0x1	//00000001	// Expect ICW 4 bit
#define x86_PIC_ICW1_MASK_SNGL			0x2	//00000010	// Single or Cascaded
#define x86_PIC_ICW1_MASK_ADI			0x4	//00000100	// Call Address Interval
#define x86_PIC_ICW1_MASK_LTIM			0x8	//00001000	// Operation Mode
#define x86_PIC_ICW1_MASK_INIT			0x10//00010000	// Initialization Command

//Vierte initializatien worte 
#define I86_PIC_ICW4_MASK_UPM		0x1	//00000001	// Mode
#define I86_PIC_ICW4_MASK_AEOI		0x2	//00000010	// Automatic EOI
#define I86_PIC_ICW4_MASK_MS		0x4	//00000100	// Selects buffer type
#define I86_PIC_ICW4_MASK_BUF		0x8	//00001000	// Buffered mode
#define I86_PIC_ICW4_MASK_SFNM		0x10	//00010000	// Special fully-nested mode


//sends the same befehl to both PICs 
void sendBothPicBfhl(unsigned char v)
{
	outb(x86_PIC1_COMMAND, v);
	outb(x86_PIC2_COMMAND, v);
};
void sendPicData(unsigned char v1, unsigned char v2)
{
	outb(x86_PIC1_DATA, v1);
	outb(x86_PIC2_DATA, v2);
};

#define	x86_PIT_RCOUNTER0 0x40
#define	x86_PIT_RCOUNTER1 0x41
#define	x86_PIT_RCOUNTER2 0x42
#define	x86_PIT_COMMAND	0x43

#define		x86_PIT_OCW_BINCOUNT_BINARY	0	//0		//! Use when setting I86_PIT_OCW_MASK_BINCOUNT
#define		x86_PIT_OCW_BINCOUNT_BCD	1	//1
#define		x86_PIT_OCW_MODE_TERMINALCOUNT	0	//0000		//! Use when setting I86_PIT_OCW_MASK_MODE
#define		x86_PIT_OCW_MODE_ONESHOT	0x2	//0010
#define		x86_PIT_OCW_MODE_RATEGEN	0x4	//0100
#define		x86_PIT_OCW_MODE_SQUAREWAVEGEN	0x6	//0110
#define		x86_PIT_OCW_MODE_SOFTWARETRIG	0x8	//1000
#define		x86_PIT_OCW_MODE_HARDWARETRIG	0xA	//1010
#define		x86_PIT_OCW_RL_LATCH		0	//000000	//! Use when setting I86_PIT_OCW_MASK_RL
#define		x86_PIT_OCW_RL_LSBONLY		0x10	//010000
#define		x86_PIT_OCW_RL_MSBONLY		0x20	//100000
#define		x86_PIT_OCW_RL_DATA		0x30	//110000
#define		x86_PIT_OCW_COUNTER_0		0	//00000000	//! Use when setting I86_PIT_OCW_MASK_COUNTER
#define		x86_PIT_OCW_COUNTER_1		0x40	//01000000
#define		x86_PIT_OCW_COUNTER_2		0x80	//10000000


#define		I86_PIC_OCW2_MASK_EOI		0x20		//00100000	//End of Interrupt command
unsigned _tckc[3] = { 0 };
unsigned pit_initCounter(unsigned freq, unsigned counter, unsigned char mode_bin)
{
	unsigned char counter_bin= x86_PIT_OCW_COUNTER_0;
	unsigned char rcounter_bin = x86_PIT_RCOUNTER0;
	switch (counter)
	{
	case 1:
		counter_bin = x86_PIT_OCW_COUNTER_1;
		rcounter_bin = x86_PIT_RCOUNTER1;
		break;
	case 2:
		counter_bin = x86_PIT_OCW_COUNTER_2;
		rcounter_bin = x86_PIT_RCOUNTER2;
		break;
	}
	unsigned short tpf = 1193180 / freq;
	outb(x86_PIT_COMMAND, counter_bin | x86_PIT_OCW_RL_DATA | mode_bin);
	outb(rcounter_bin, tpf);
	outb(rcounter_bin, tpf >> 8);
	_tckc[counter] = 0;
	return counter;
}
_declspec(naked) void pit_irqHandler()
{
	_asm
	{
		add esp, 12
		pushad
	}
	putchar('r');
	_tckc[0]++;
	intend(0);
	_asm
	{
		popad
		iretd
	}
}
inline void intend(unsigned i)
{
	_asm
	{
		mov eax, [i]
		cmp eax, 7
		jg masterPicProc
		mov dx, x86_PIC2_COMMAND
		mov al, I86_PIC_OCW2_MASK_EOI
		out dx, al
		masterPicProc:
		mov dx, x86_PIC1_COMMAND
		mov al, I86_PIC_OCW2_MASK_EOI
		out dx, al
	}
}
#include"exceptionfunc.hpp"
#include<string.hpp>
int load_dts()
{
	/*GDT*/	{
		_asm
		{
			lgdt[gdt]
		}
	}
	/*IDT*/	{
		idtr.offset = itable;
		setint(20, defIdtHandler, DEFIRQATTR);
		for (idte* tc = (idte*)idtr.offset + 21; tc-idtr.offset < lastIdte; tc++)
			memcpy(tc, reinterpret_cast<idte*>(idtr.offset)+20, sizeof(idte));
		setint(0, divide_by_zero_fault, DEFIRQATTR);
		setint(1, single_step_trap, DEFIRQATTR);
		setint(2, nmi_trap, DEFIRQATTR);
		setint(3, breakpoint_trap, DEFIRQATTR);
		setint(4, overflow_trap, DEFIRQATTR);
		setint(5, bounds_check_fault, DEFIRQATTR);
		setint(6, invalid_opcode_fault, DEFIRQATTR);
		setint(7, no_device_fault, DEFIRQATTR);
		setint(8, double_fault_abort, DEFIRQATTR);
		setint(10, invalid_tss_fault, DEFIRQATTR);
		setint(11, no_segment_fault, DEFIRQATTR);
		setint(12, stack_fault, DEFIRQATTR);
		setint(13, general_protection_fault, DEFIRQATTR);
		setint(14, page_fault, DEFIRQATTR);
		setint(16, fpu_fault, DEFIRQATTR);
		setint(17, alignment_check_fault, DEFIRQATTR);
		setint(18, machine_check_abort, DEFIRQATTR);
		setint(19, simd_fpu_fault, DEFIRQATTR);

		setint(PIC1_ADDR + 0x00, pit_irqHandler, DEFIRQATTR);
		idtr.size = lastIdte * 8 - 1;
		_asm lidt[idtr]
	}
	/*PIC*/ {
		sendBothPicBfhl(x86_PIC_ICW1_MASK_INIT | x86_PIC_ICW1_MASK_IC4);
		sendPicData(PIC1_ADDR, PIC2_ADDR);
		sendPicData(4, 2);
		unsigned char icw4 = I86_PIC_ICW4_MASK_UPM;
		sendPicData(icw4, icw4);
	}
	/*PIT*/ {
		pit_initCounter(1000, 0, x86_PIT_OCW_MODE_SQUAREWAVEGEN);
		_asm sti
	}
	return 0;
};
int initPcStdRoutines(unsigned long long* _attr, int* _dev_arg)
{
	int result = load_dts();
	if(!result)result=init_mmngr(_dev_arg[0]);
	else return result;
	return result;
};
