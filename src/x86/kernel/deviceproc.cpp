#include"deviceproc.hpp"
#include<qzstd.hpp>
#define EMPTYDEVENTRY 0xFF
#define ENDOFDEVLIST 0
#define DEVBASE 0xF0000000
volatile devinfo *loadeddev=reinterpret_cast<devinfo*>((char*)0x7C00);
extendDevInfo *exLoadedDev=reinterpret_cast<extendDevInfo*>((char*)0x50080);
unsigned int lddevn=0;
unsigned int last_dev_i=0;
FILE DeviceFile;
_deverr* initdevices(_deverr* deverr)
{
	DeviceFile.id = DEVBASE;
	DeviceFile.parent = nullptr;
	DeviceFile.special = true;
	int ern = 0;
	for (unsigned int i = 0; i < MAXDEVICES; i++)
	{
		switch (loadeddev[i].__ext->__dev)
		{
		case EMPTYDEVENTRY:
			break;
		case ENDOFDEVLIST:
			deverr->count = ern;
			return deverr;
		default:
			if (loadeddev[i].__ext->devState == CONFIGURED_AND_INSTALLED)continue;
			const int result = loadeddev[i].init(loadeddev[i].__ext);
			if(result)
			{
				if (ern < 32)
				{
					deverr->err[ern].__err = result;
					deverr->err[ern].__dev = loadeddev[i].__ext->__dev;
				}
				else
				{
					//Not for now
				}
				ern++;
				loadeddev[i].__ext->devState = FAILED_INSTALLATION;
			}
			else loadeddev[i].__ext->devState = CONFIGURED_AND_INSTALLED;
		}
	}
	deverr->count = ern;
	return deverr;
}
size_t deinitdevices()
{
	size_t count = 0;
	for (unsigned int i = 0; i < MAXDEVICES; i++)
	{
		switch (loadeddev[i].__ext->__dev)
		{
		case EMPTYDEVENTRY:
			break;
		case ENDOFDEVLIST:
			i = MAXDEVICES;
			break;
		default:
			if (loadeddev[i].deinit&&loadeddev[i].__ext->devState==CONFIGURED_AND_INSTALLED)
			{
				loadeddev[i].deinit();
				loadeddev[i].__ext->__dev = EMPTYDEVENTRY;
				count++;
			}
		}
	}
	loadeddev[0].__ext->__dev = 0;
	return count;
}
int find_dev(unsigned _dev)
{
	for (int i = 0; i < MAXDEVICES; i++)
	{
		switch (loadeddev[i].__ext->__dev)
		{
		case ENDOFDEVLIST:
			return -1;
		case EMPTYDEVENTRY:break;
		default:
			if (loadeddev[i].__ext->__dev == _dev)return i;
		}
	}
	return -2;
}
int removeDev(unsigned _dev)
{
	unsigned int i = find_dev(_dev);
	if (i < 0)return 1;
	switch (loadeddev[i].__ext->devState)
	{
	case CONFIGURED_AND_INSTALLED:
		if (loadeddev[i].deinit)loadeddev[i].deinit();
	case CONFIGURED_NOT_INSTALLED:
		if (i == last_dev_i)
		{
			loadeddev[i].__ext->__dev = ENDOFDEVLIST;
		} else loadeddev[i].__ext->__dev = EMPTYDEVENTRY;
		return 0;
	}
}
int configDev(unsigned _dev,  int(*initf)(extendDevInfo *_ex), int(*deinitf)(), unsigned long long* attr)
{
	lddevn++;
	if (lddevn >= MAXDEVICES)return -1;
	for (int i = 0; i < MAXDEVICES; i++)
	{
		if (!loadeddev[i].__ext)
		{
			loadeddev[i].__ext = exLoadedDev + i;
		}
		switch (loadeddev[i].__ext->__dev)
		{
			case ENDOFDEVLIST:
				if((i+1)!=MAXDEVICES)loadeddev[i + 1].__ext->__dev = ENDOFDEVLIST;
				last_dev_i = i;
			case EMPTYDEVENTRY:
				loadeddev[i].init = initf;
				loadeddev[i].deinit = deinitf;
				loadeddev[i].__ext->__dev = _dev;
				loadeddev[i].__ext->devAttr = attr;
				return i;
		}
	}
	return -2;
}
#include<string.hpp>
extendDevInfo *getDev(unsigned _dev, extendDevInfo *_buf)
{
	unsigned int i = find_dev(_dev);
	if (i < 0)return nullptr;
	else
	{
		memcpy(_buf, loadeddev[i].__ext, sizeof(extendDevInfo));
		return _buf;
	}
}
FILE *getDevFile(unsigned _dev, FILE *_buf)
{
	unsigned int i = find_dev(_dev);
	if (i < 0)return nullptr;
	else
	{
		memcpy(_buf, loadeddev[i].__ext->devFile, sizeof(FILE));
		return _buf;
	}
}
