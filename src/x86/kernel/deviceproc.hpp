#pragma once
#include<krnlio.hpp>
#include<stdio.hpp>
#include"qgdef.def"
#include<qzstd.hpp>
#define MAXDEVICES 1408
struct devrr
{
	unsigned int __dev;
	int __err;
};
struct _deverr
{
	devrr err[32];
	size_t count;
};
typedef int devExInstProg;
#define CONFIGURED_AND_INSTALLED 7
#define CONFIGURED_NOT_INSTALLED 3
#define FAILED_INSTALLATION 15
#define UNDEFINED_DEV_TYPE 0
#define VIDEO_DEV 0x100
#define STORAGE_DEV 0x200
#define PC_DEV 0x300
struct extendDevInfo
{
	unsigned int __dev; //Device ID
	unsigned int __dev_type=UNDEFINED_DEV_TYPE; //(Optional) Device type
	unsigned long long *devAttr; //Points to an array of integer-based device attributes.
#ifdef BIT32_COMPIL //Reserved for 64-bit
private:unsigned int reserved_1;
#endif
public:
	FILE *devFile; //(Optional) Contains the device stream.
#ifdef BIT32_COMPIL //Reserved for 64-bit
private:unsigned int reserved_2;
#endif
public:
	const char *devName; //(Optional) Contains the user-firendly name of the device.
#ifdef BIT32_COMPIL //Reserved for 64-bit
private:unsigned int reserved_3;
#endif
public:
	void *devSpec; //Contains a pointer to structure specific for a device
#ifdef BIT32_COMPIL //Reserved for 64-bit
private:unsigned int reserved_4;
#endif
public:
	devExInstProg devState;
	unsigned long long reserved_ext; //In case the structure needs even more expansion
};//52 bytes
struct devinfo
{
	int (*init)(extendDevInfo*);
#ifdef BIT32_COMPIL //Reserved for 64-bit
private:unsigned int reserved_1;
#endif
public:
	int(*deinit)(); //Deinitalization function
#ifdef BIT32_COMPIL //Reserved for 64-bit
private:unsigned int reserved_2;
#endif
public:
	extendDevInfo *__ext;
#ifdef BIT32_COMPIL //Reserved for 64-bit
private:unsigned int reserved_3;
#endif
};//24 bytes
_deverr *initdevices(_deverr* deverr);
size_t deinitdevices();
int configDev(unsigned _dev, int(*initf)(extendDevInfo *_ex), int(*deinitf)(), unsigned long long* attr);
int removeDev(unsigned _dev);
extendDevInfo *getDev(unsigned _dev, extendDevInfo *_buf);
FILE *getDevFile(unsigned _dev, FILE *_buf);
extern FILE DeviceFile;
