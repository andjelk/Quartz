#pragma once
#include<stdio.hpp>
int initStorage();
int deinitStorage();
int storWriteByte(int character, FILE *stream);
int storWriteFile(void * buf, unsigned int count, FILE * stream);
int storReadByte(FILE * stream);
int storReadFile(void * buf, unsigned int count, FILE * stream);