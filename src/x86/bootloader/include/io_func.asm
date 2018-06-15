%ifndef IO_FUNC
%define IO_FUNC
;BH=2 Input Buffer
;BH=1 Output Buffer
waitinkey:
in al,0x64
test al,10b
jnz waitinkey
ret
waitoutkey:
in al,0x64
test al,01b
jz waitoutkey
ret
%endif
