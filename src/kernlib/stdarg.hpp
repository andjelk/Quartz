#pragma once
#include"va_list.hpp"
#define	va_size(TYPE)((sizeof(TYPE) + sizeof(int) - 1)&~(sizeof(int) - 1))
#define	va_start(AP, LASTARG)(AP=((va_list)&(LASTARG) + va_size(LASTARG)))
#define va_end(AP)
#define va_arg(AP, TYPE)(AP+=va_size(TYPE), *((TYPE *)(AP - va_size(TYPE))))
