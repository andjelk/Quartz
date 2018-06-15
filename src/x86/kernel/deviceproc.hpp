#pragma once
#include"..\library\krnlio.hpp"


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
struct devinfo
{
	unsigned int __dev;
	int (*init)(unsigned long long*, int*);
	int (*deinit)();
	int* devinitpar;
	unsigned long long devattraddr;
};
enum devExInstProg
{
	CONFIGURED_NOT_INSTALLED,
	CONFIGURED_AND_INSTALLED,
	FAILED_INSTALLATION
};
_deverr *initdevices(_deverr* deverr);
size_t deinitdevices();
int installdev(unsigned _dev, int(*initf)(unsigned long long*, int*), int(*deinitf)(), int* __par, unsigned long long* attr);
extern inline unsigned long long getDevAttr(unsigned _dev, unsigned n);
extern constexpr devExInstProg devinstallstate(unsigned  _dev);
