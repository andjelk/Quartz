
.model FLAT
PUBLIC _initFpu
.stack
.data
	fpuConfig dw 37Ah
.code
_initFpu PROC
	push eax
	mov eax, cr4
	or eax, 200h
	mov cr4, eax
	pop eax
	fldcw ds:[fpuConfig]
	ret
_initFpu ENDP
END