#pragma once
//Devices

//VGA in text mode.
#define VGATEXTMODE 0xF0000000
//Test non-conditional error device.
#define FAULTDEVICE 0xF0000001
//Memory manager, paging, etc.
#define PCX86 0xF0000002
//Storage.
#define PCSTORAGE 0xF0000003
unsigned long long _getDevAttr(unsigned dev, unsigned n);
