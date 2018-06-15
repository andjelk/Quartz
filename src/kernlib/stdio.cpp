#include"krnlio.hpp"
#include"stdarg.hpp"
#include"string.hpp"
#include"stdlib.hpp"
unsigned int cvtxp=0;
unsigned int cvtyp=0;

unsigned int stdout = VGATEXTMODE;
unsigned int stdin;
inline void _setchvt(unsigned char _ch, unsigned x, unsigned y)
{
	_asm
	{
		xor eax, eax
		mov bl, [_ch]
		mov ecx, [x]
		mov edx, [y]
		int 0x90
	}
}
inline void _svtcur(unsigned x, unsigned y)
{
	_asm
	{
		mov eax, 1
		mov ebx, [x]
		mov ecx, [y]
		int 0x90
	}
}
void vtnewline()
{
	cvtxp = 0;
	cvtyp++;
	if (cvtyp == _getDevAttr(stdout, 3))
	{
		cvtyp = 24;
		//Scroll;
	}
}
inline void putcht(unsigned char character)
{
	switch (character)
	{
	case '\n':
		vtnewline();
		break;
	case '\b':
		//backspace;
		break;
	default:
		_setchvt(character, cvtxp++, cvtyp);
		if (cvtxp > _getDevAttr(stdout, 2))vtnewline();
	}
}
int putchar(int character)
{
	putcht((unsigned char)character);
	_svtcur(cvtxp, cvtyp);
	return character;
}
int puts(const char* str)
{
	int i = 0;
	while (str[i])putcht(str[i++]);
	_svtcur(cvtxp, cvtyp);
	return i;
}
char str[11]; // 11 = floor(log10(2 ^ 32)) + 2 e.g.null terminator and negativity
void printf(const char* format, ...)
{
	va_list list;
	va_start(list,format);
	_asm
	{
		_asm xchg bx, bx
		mov eax, [list]
	}
	for (size_t i = 0; format[i]; i++)
	{
		switch (format[i])
		{
			case '%':
				i++;
				if (format[i])
				{
					switch (format[i])
					{
						case 'c':
						{
							unsigned char v = va_arg(list, unsigned char);
							putchar(v);
							break;
						}
						case 's':
						{
							int *v = va_arg(list, int*);
							puts((char*)v);
							break;
						}
						case 'd':
						case 'i':
						{
							int v = va_arg(list, int);
							puts(itoa(v, str, 10));
							break;
						}
						case 'x':
						{
							unsigned int v = va_arg(list, unsigned int);
							_asm
							{
								mov eax, [list]
								mov ebx, [v]
								xchg bx, bx
							}
							extern int _itoa(unsigned, char*, unsigned);
							_itoa(v, str, 16);
							puts(str);
							break;
						}
						case 'f':
						{
							double v = va_arg(list, double);
							puts(itoa(v, str, 10));
							break;
						}
					}
				}
				else return;
				break;
			default:
			putchar(format[i]);
		}
	}
}

