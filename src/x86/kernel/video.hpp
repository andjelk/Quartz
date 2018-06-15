#pragma once
extern unsigned short vtx;
extern unsigned short vty;
extern inline void setchvt(unsigned char ch, unsigned x, unsigned y);
void svtcur(unsigned char x, unsigned char y);
int videoinit(unsigned long long*, int*);
