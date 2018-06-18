#include<stdio.hpp>
void divide_by_zero_fault(unsigned cs, unsigned eip, unsigned flags)
{
	printf("[Fatal Error] >>> Divide by 0 at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void single_step_trap(unsigned cs, unsigned eip, unsigned flags)
{
	printf("[Fatal Error] >>> Single step at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void nmi_trap(unsigned cs, unsigned eip, unsigned flags)
{
	printf("[Fatal Error] >>> NMI trap at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void breakpoint_trap(unsigned cs, unsigned eip, unsigned flags)
{	
	printf("[Fatal Error] >>> Breakpoint trap at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void overflow_trap(unsigned cs, unsigned eip, unsigned flags)
{	
	printf("[Fatal Error] >>> Overflow trap at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void bounds_check_fault(unsigned cs, unsigned  eip, unsigned flags)
{	
	printf("[Fatal Error] >>> Bounds check fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void invalid_opcode_fault(unsigned cs, unsigned  eip, unsigned flags)
{	
	printf("[Fatal Error] >>> Invalid opcode at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void no_device_fault(unsigned cs, unsigned eip, unsigned flags)
{
	printf("[Fatal Error] >>> Device not found fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void double_fault_abort(unsigned eip, unsigned cs, unsigned flags, unsigned err)
{	
	printf("[Fatal Error] >>> Double fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void invalid_tss_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err)
{	
	printf("[Fatal Error] >>> Invalid TSS at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void no_segment_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err)
{	
	printf("[Fatal Error] >>> Invalid segment at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void stack_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err)
{	
	printf("[Fatal Error] >>> Stack fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void general_protection_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err) {

	
	printf("[Fatal Error] >>> General Protection Fault\n At physical address 0x%x (%x Segment)\n EFLAGS = %x\n Error information : %x", eip, cs, flags, err);
	_asm cli
	_asm hlt
}
void page_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err)
{
	unsigned faultAddr = 0;
	_asm {
		mov eax, cr2
		mov[faultAddr], eax
	}
	printf("[Fatal Error] >>> Page Fault at 0x%x:0x%x refrenced memory at 0x%x",
		cs, eip, faultAddr);
	_asm cli
	_asm hlt
}
void __declspec (naked) fpu_fault(unsigned cs, unsigned  eip, unsigned flags)
{	
	printf("[Fatal Error] >>> FPU Fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void __declspec (naked) alignment_check_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err)
{	
	printf("[Fatal Error] >>> Alignment Check at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void __declspec (naked) machine_check_abort(unsigned cs, unsigned  eip, unsigned flags)
{	
	printf("[Fatal Error] >>> Machine Check at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}
void __declspec (naked) simd_fpu_fault(unsigned cs, unsigned  eip, unsigned flags)
{
	printf("[Fatal Error] >>> FPU SIMD fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm cli
	_asm hlt
}