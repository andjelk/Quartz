%ifndef _floppy
%include "fat_boot_sector.asm"
%define _floppy
sector db 0
head db 0
track db 0
bufloc dw 0x0050
stacktemp dw 0
;PROCEDURE
;Reads sectors. (Real Mode)
;Two entries for stack:
;1st - Sector count
;2st - First sector given in LBA
ReadSectors:
    pop ax
    mov WORD [stacktemp], ax
    pop ax
    xor dx, dx
    div WORD [bpbSectorsPerTrack]
    inc dl
    mov BYTE [sector], dl
    xor dx, dx
    div WORD [bpbHeadsPerCylinder]
    mov BYTE [head], dl
    mov BYTE [track], al
    pop ax
    mov ah, 0x02
    push ax
    read_try:
    xor ah, ah
    xor dl, dl
    int 0x13
    jc read_try
    pop ax
    xor dl, dl
    mov bx, WORD [bufloc] ;Loading at 0:bufloc
    mov es, bx
    xor bx, bx
    xor dx, dx
    push ax
    mov ch, BYTE [track]
    mov cl, BYTE [sector]
    mov dh, BYTE [head]
    int 0x13
    jc read_try
    mov es, bx
    pop ax
    push word [stacktemp]
    ret
%endif
