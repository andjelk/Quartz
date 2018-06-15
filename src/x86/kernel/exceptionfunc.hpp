#pragma once
extern void _cdecl divide_by_zero_fault(unsigned a, unsigned b, unsigned c);

//! single step
extern void _cdecl single_step_trap(unsigned cs,
	unsigned eip, unsigned eflags);

//! non maskable interrupt trap
extern void _cdecl nmi_trap(unsigned cs,
	unsigned eip, unsigned eflags);

//! breakpoint hit
extern void _cdecl breakpoint_trap(unsigned cs,
	unsigned eip, unsigned eflags);

//! overflow
extern void _cdecl overflow_trap(unsigned cs,
	unsigned eip, unsigned eflags);

//! bounds check
extern void _cdecl bounds_check_fault(unsigned cs,
	unsigned eip, unsigned eflags);

//! invalid opcode / instruction
extern void _cdecl invalid_opcode_fault(unsigned cs,
	unsigned eip, unsigned eflags);

//! device not available
extern void _cdecl no_device_fault(unsigned cs,
	unsigned eip, unsigned eflags);

//! double fault
extern void _cdecl double_fault_abort(unsigned eip, unsigned cs, unsigned flags, unsigned err);

//! invalid Task State Segment (TSS)
extern void _cdecl invalid_tss_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err);

//! segment not present
extern void _cdecl no_segment_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err);

//! stack fault
extern void _cdecl stack_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err);

//! general protection fault
extern void _cdecl general_protection_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err);

//! page fault
extern void _cdecl page_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err);

//! Floating Point Unit (FPU) error
extern void _cdecl fpu_fault(unsigned cs,
	unsigned eip, unsigned eflags);

//! alignment check
extern void _cdecl alignment_check_fault(unsigned eip, unsigned cs, unsigned flags, unsigned err);

//! machine check
extern void _cdecl machine_check_abort(unsigned cs, unsigned  eip, unsigned flags);

//! Floating Point Unit (FPU) Single Instruction Multiple Data (SIMD) error
extern void _cdecl simd_fpu_fault(unsigned cs,
	unsigned eip, unsigned eflags);
