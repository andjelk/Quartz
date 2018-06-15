#include<stdio.hpp>
void divide_by_zero_fault(unsigned cs, unsigned eip, unsigned flags)
{
	printf("[Fatal Error] >>> Divide by 0 at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! single step
void single_step_trap(unsigned cs, unsigned eip, unsigned flags)
{
	printf("[Fatal Error] >>> Single step at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! non maskable interrupt trap
void nmi_trap(unsigned cs, unsigned eip, unsigned flags)
{
	printf("[Fatal Error] >>> NMI trap at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! breakpoint hit
void breakpoint_trap(unsigned cs, unsigned eip, unsigned flags) {

	
	printf("[Fatal Error] >>> Breakpoint trap at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! overflow
void overflow_trap(unsigned cs, unsigned eip, unsigned flags) {

	
	printf("[Fatal Error] >>> Overflow trap at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! bounds check
void bounds_check_fault(unsigned cs, unsigned  eip, unsigned flags) {

	
	printf("[Fatal Error] >>> Bounds check fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! invalid opcode / instruction
void invalid_opcode_fault(unsigned cs, unsigned  eip, unsigned flags) {

	
	printf("[Fatal Error] >>> Invalid opcode at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! device not available
void no_device_fault(unsigned cs, unsigned eip, unsigned flags) {

	
	printf("[Fatal Error] >>> Device not found fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! double fault
void double_fault_abort(unsigned eip, unsigned cs, unsigned flags, unsigned err) {

	
	printf("[Fatal Error] >>> Double fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! invalid Task State Segment (TSS)
void invalid_tss_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err) {

	
	printf("[Fatal Error] >>> Invalid TSS at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! segment not present
void no_segment_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err) {

	
	printf("[Fatal Error] >>> Invalid segment at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! stack fault
void stack_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err) {

	
	printf("[Fatal Error] >>> Stack fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! general protection fault
void general_protection_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err) {

	
	printf("[Fatal Error] >>> General Protection Fault at physical address [0x%x:0x%x] EFLAGS [0x%x]",
		cs, eip, flags);
	_asm hlt
}


//! page fault
void page_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err) {

	

	int faultAddr = 0;

	_asm {
		mov eax, cr2
		mov[faultAddr], eax
	}

	printf("[Fatal Error] >>> Page Fault at 0x%x:0x%x refrenced memory at 0x%x",
		cs, eip, faultAddr);
	_asm hlt
}

//! Floating Point Unit (FPU) error
void __declspec (naked) fpu_fault(unsigned cs, unsigned  eip, unsigned flags) {

	
	printf("[Fatal Error] >>> FPU Fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! alignment check
void __declspec (naked) alignment_check_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err) {

	
	printf("[Fatal Error] >>> Alignment Check at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! machine check
void __declspec (naked) machine_check_abort(unsigned cs, unsigned  eip, unsigned flags) {

	
	printf("[Fatal Error] >>> Machine Check at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}

//! Floating Point Unit (FPU) Single Instruction Multiple Data (SIMD) error
void __declspec (naked) simd_fpu_fault(unsigned cs, unsigned  eip, unsigned flags) {

	
	printf("[Fatal Error] >>> FPU SIMD fault at physical address [0x%x:0x%x] EFLAGS [0x%x]", cs, eip, flags);
	_asm hlt
}