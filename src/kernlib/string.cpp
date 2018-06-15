#include "string.hpp"

void* memset(void* pnt, int value, size_t num)
{
	_asm
	{
		mov ecx, [num]
		mov eax, [value]
		mov edi, [pnt]
		rep stos al
	}
	return pnt;
}
void* memcpy(void* destination, const void* source, size_t num)
{
	_asm
	{
		mov ecx, [num]
		mov esi, [source]
		mov edi, [destination]
		rep movs
	}
	return destination;
}
size_t strlen(const char* str)
{
	size_t len=0;
	while (*(str++))len++;
	return len;
}//ab\0
 //12

char * strrev(char * str)
{
	int strl = strlen(str);
	int i = 0, j = strl - 1;
	char temp=str[i];
	while(j>i)
	{
		_asm xchg bx ,bx
		str[i] = str[j];
		str[j--] = temp;
		temp = str[++i];
	}
	return str;
}
