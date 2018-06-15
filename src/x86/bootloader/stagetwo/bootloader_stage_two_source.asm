bits 16
org 0x8000
jmp main
%include "real_mode_print.asm"
%include "io_func.asm"
%include "real_mode_floppy.asm"
bsErrorMsg db "System file 'system\kernel' can not be found."
%include "real_mode_fat_12.asm"
dMessage db "qloader starting execution > ", 0
dFolder db "SYSTEM     "
dFile db "KERNEL     "
;GDT Table
s_gdt_data:
;null descriptor (supposed to be, explanation below)
;As CPU never accesses this descriptor, then this 8 bytes can be used for literally anything.
;And is there a better place to put a pointer to it itself, the GDT?
	dw 23
	dd s_gdt_data
	dw 0
;data descriptor
	dw 0FFFFh
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0
;code descriptor
	dw 0FFFFh
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0
main:
    xchg bx, bx
    mov si, dMessage
    call print
    ;Find kernel
    mov WORD [FileBuf], 0x1000
    mov WORD [FileName], dFolder
    call FindFile
    mov WORD [FileDirSeg], 0x1000
    mov WORD [FileDir], 0
    mov WORD [FileName], dFile
    call FindFile

    ;Prepare for protected mode
    cli
    lgdt [s_gdt_data]
    mov eax, cr0
    or eax, 1b
    mov cr0, eax
    jmp 0x10:pmain
bits 32
pmain:
    mov dx, 0x08
    mov ds, dx
    mov es, dx
    mov gs, dx
    mov fs, dx
    mov ss, dx
    mov sp, 0x7c00
    ;A20
    call waitinkey
    mov al, 0xAD
    out 0x64, al
    call waitinkey
    mov al, 0xD0
    out 0x64, al;Send command to initialize receiving
    call waitoutkey
    in al, 0x60;Receive data
    or al, 10b
    push eax
    call waitinkey
    mov al, 0xD1
    out 0x64, al;Send command to initalize sending
    call waitinkey
    pop eax
    out 0x60, al;Send data with second bit set ( A20 )
    call waitinkey
    mov al, 0xAE
    out 0x64, al
    call waitinkey; Blyatifully waiting for this to finish
    ;	PE execution routine
    mov ebx, 0x10028 ;Base=0x10000 + 0x18=Size of IMAGE_FILE_HEADER + 0x10=Offset to the entry point
    add ebx, [0x1003C] ;Base+0x3C=LFA_NEW
    mov ebp, 0x10000
    add ebp, dword [ebx]
    jmp dword [ebp]
    ;	ELF execution routine
    ;mov edi, 0x10004 ;Base=0x10000 + 0x4=p_offset
    ;add edi, dword ptr[0x1001C] ;Base=0x10000 + 0x1C=Program header table position
    ;xor al, al
    ;mov ecx, dword ptr[0x10028]
    ;rep stosb Clear p_memsz
    ;jmp dword [0x10018] ;Or just jump to the entry point?
    hlt
