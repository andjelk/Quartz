%ifndef _print
%define _print
print:
    xor bx, bx
    print_loop:
    lodsb
    test al, al
    je print_end
    mov ah, 0x0e
    int 0x10
    jmp print_loop
    print_end : ret
%endif

