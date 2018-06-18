#include"krnlio.hpp"
#include"stdarg.hpp"
#include"string.hpp"
#include"stdio.hpp"
#include"stdlib.hpp"

FILE _stdout;
FILE _stdin;
FILE *stdout = &_stdout;
FILE *stdin = &_stdin;
void refreshStdioStream()
{
	_getDevFile(VGATEXTMODE, stdout);
}
inline int _storWriteByte(int character, FILE *stream)
{
	int rv;
	_asm
	{
		mov eax, 4
		mov ebx, [character]
		mov ecx, [stream]
		int 0x90
		mov [rv], eax
	}
	return rv;
}
inline int _storWriteFile(void * buf, unsigned int count, FILE * stream)
{
	int rv;
	_asm
	{
		mov eax, 6
		mov ebx, [buf]
		mov ecx, [count]
		mov edx, [stream]
		int 0x90
		mov [rv], eax
	}
	return rv;
}
int fputc(int character, FILE *stream)
{
	if (character > 255 || character < 0)return EOF;
	int result = _storWriteByte(character, stream);
	if (result)return EOF;
	return character;
}
int fputs(const char *str, FILE *stream)
{
	return _storWriteFile((void*)str, strlen(str), stream);
}
int putchar(int character)
{
	fputc(character, stdout);
	return character;
}
int puts(const char *str)
{
	int r=fputs(str, stdout);
	putchar('\n');
	return r;
}
char str[11]; // 11 = floor(log10(2 ^ 32)) + 2 e.g.null terminator and negativity
int vprintf(const char *format, va_list arg)
{
	char count=0;
	size_t i = 0;
	for (; format[i]; i++)
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
					unsigned char v = va_arg(arg, unsigned char);
					putchar(v);
					count++;
					break;
				}
				case 's':
				{
					char *v = va_arg(arg, char*);
					count+=fputs(v,stdout);
					break;
				}
				case 'd':
				case 'i':
				{
					int v = va_arg(arg, int);
					count += fputs(itoa(v, str, 10),stdout);
					break;
				}
				case 'x':
				{
					unsigned int v = va_arg(arg, unsigned int);
					extern int _itoa(unsigned, char*, unsigned);
					_itoa(v, str, 16);
					count+=fputs(str,stdout);
					break;
				}
				case 'f':
				{
					double v = va_arg(arg, double);
					count+=fputs(itoa(v, str, 10),stdout);
					break;
				}
				}
			}
			else
			{
				return i;
			}
			break;
		default:
			putchar(format[i]);
			count++;
		}
	}
	return count;
}
int printf(const char *format, ...)
{
	va_list list;
	va_start(list,format);
	int ret_ = vprintf(format, list);
	va_end(list);
	return ret_;
}

