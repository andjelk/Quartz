#pragma once
#include"stdio.hpp"
//Devices

//Paging, descriptor tables, etc.
#define PCX86 0xF0000010
//VGA Terminal
#define VGATEXTMODE 0xF0000015
//Test non-conditional error device.
#define FAULTDEVICE 0xF0000020
//Storage.
#define PCSTORAGE 0xF00000025


FILE *_getDevFile(unsigned dev, FILE *buf);
