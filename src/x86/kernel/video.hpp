#pragma once
#include"deviceproc.hpp"
extern unsigned short vtx;
extern unsigned short vty;
extern bool _moveCursor;
extern int videoinit(extendDevInfo*);
extern void v_term_write_i(char character, unsigned short x, unsigned short y);
extern void v_term_write(char character);
extern void staticCursor(bool);
