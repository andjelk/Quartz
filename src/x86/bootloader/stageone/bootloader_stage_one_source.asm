bits	16
org		0x7c00
jmp	main
times 0x0b-($-$$) db 0
%include "fat_boot_sector.asm"
%include "real_mode_floppy.asm"
%include "real_mode_print.asm"
%include "real_mode_fat_12.asm"
bsAdditional           DB "Standard Thontelix Quartz Operating System Bootloader", 0x0a, 0x0d, "bootsec starting execution > ", 0
bsErrorMsg             DB "Boot file 'qloader' can not be found.", 0
dFile db "QLOADER    "
main: ;Start
    xchg bx, bx
    cli
    xor ax, ax
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov sp, 0x7C00; The first instructions will not be executed anyway after this...
    sti
    mov si, bsAdditional
    call print
    mov WORD [bufloc], 0x0150
    push 32
    push 1
    call ReadSectors
    mov ax, dFile
    mov WORD [FileName], ax
    call FindFile
    xor ax, ax
    mov bx, ax
    mov cx, ax
    mov dx, ax ;Clearing registers. It does not really matter anyway, but it takes just 4 clocks.
    jmp 0:0x8000
times 510-($-$$) db 0
dw 0xAA55
