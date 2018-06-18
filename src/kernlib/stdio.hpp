#pragma once
#include"va_list.hpp"
struct FILE
{
	unsigned int id;
	bool special=false;
	FILE *parent;
	void *buffer; //Used only with special devices
	unsigned int position;
};
extern FILE *stdin;
extern FILE *stdout;
extern void refreshStdioStream();
#define EOF -1
int fputc(int character, FILE *stream);
#define putc(character, stream) fputc(character, stream);
int fputs(const char *str, FILE *stream);
int puts(const char* str);
int putchar(int character);
int vprintf(const char* format, va_list arg);
int printf(const char* format, ...);

