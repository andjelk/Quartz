%ifndef FAT_12
%define FAT_12
FileDirSeg dw 0
FileDir dw 0x3900
;DirEntryN dw 224
FileBuf dw 0x0800
FileName dw 0
FindFile:
    xor dx, dx
    mov si, WORD [FileDir] ;ds:si = 0:FileDir
    mov di, WORD [FileName];es:di = 0:(string location)
    mov ax, WORD [FileDirSeg]
    mov ds, ax
    findloop:
    mov cx, 11
    push si
    push di
    mov al, BYTE [si]
    test al, al
    jz nfound
    repz cmpsb
    jz found
    pop di
    pop si
    inc dx
    add si, 0x20
    ;cmp dx, [DirEntryN];224
    ;jnz findloop
    jmp findloop
    ;Not found
    nfound:
    xor ax, ax
    mov ds, ax
    mov si, bsErrorMsg
    call print
    cli
    hlt
    found:
    pop di
    pop si
    push ds
    xor ax, ax
    mov ds, ax
    mov ax, [FileBuf]
    mov WORD [bufloc], ax
    ;Entry is in DS:SI
    add si, 26 ;First sector
    pop ds
    mov ax, WORD [si] ;AX contains the sector
    xor cx, cx
    mov ds, cx
    LoadToMem:
    mov bx, ax
    add bx, 31
    push ax
    push 1
    push bx
    call ReadSectors
    pop ax
    add WORD [bufloc], 0x20
    ;Now find the next sector.
    xor dx, dx
    mov cx, 2
    div cx
    test dx, dx
    jz even_entry
    ;odd entry
    mov dx, 3
    mul dx
    mov bx, ax
    mov ax, word [bx+0x1501]
    shr ax, 4
    cmp ax, 0xFF0
    jl LoadToMem
    jmp DoneLoad
    even_entry:
    mov dx, 3
    mul dx
    mov bx, ax
    mov ax, word [bx+0x1500]
    and ax, 0x0FFF
    cmp ax, 0xFF0
    jl LoadToMem
    DoneLoad:
    ret
%endif
