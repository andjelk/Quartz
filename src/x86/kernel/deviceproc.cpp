#include"deviceproc.hpp"
#define EMPTYDEVENTRY 0xFF
#define ENDOFDEVLIST 0
#define DEVBASE 0xF0000000
devExInstProg devinstll[MAXDEVICES];
unsigned int devLocList[MAXDEVICES];
devinfo *loadeddev=reinterpret_cast<devinfo*>(0x7C00);
unsigned int lddevn;
_deverr* initdevices(_deverr* deverr)
{
	int ern = 0;
	for (unsigned int i = 0; i < MAXDEVICES; i++)
	{
		switch (loadeddev[i].__dev)
		{
		case EMPTYDEVENTRY:
			break;
		case ENDOFDEVLIST:
			deverr->count = ern;
			return deverr;
		default:
			const int result = loadeddev[i].init(reinterpret_cast<unsigned long long*>(loadeddev[i].devattraddr), loadeddev[i].devinitpar);
			if(result)
			{
				if (ern < 32)
				{
					deverr->err[ern].__err = result;
					deverr->err[ern].__dev = loadeddev[i].__dev;
				}
				else
				{
					//Not for now
				}
				ern++;
				devinstll[loadeddev[i].__dev - DEVBASE] = FAILED_INSTALLATION;
			}else devinstll[loadeddev[i].__dev - DEVBASE] = CONFIGURED_AND_INSTALLED;
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
		switch (loadeddev[i].__dev)
		{
		case EMPTYDEVENTRY:
			break;
		case ENDOFDEVLIST:
			return count;
		default:
			if (loadeddev[i].deinit)
			{
				loadeddev[i].deinit();
				count++;
			}
		}
	}
	return count;
}
int installdev(unsigned _dev,  int(*initf)(unsigned long long*, int*), int(*deinitf)(), int* __par, unsigned long long* attr)
{
	lddevn++;
	if (lddevn > MAXDEVICES)return -1;
	for (int i = 0; i < MAXDEVICES; i++)
	{
		switch (loadeddev[i].__dev)
		{
			case ENDOFDEVLIST:loadeddev[i + 1].__dev = ENDOFDEVLIST;
			case EMPTYDEVENTRY:
				loadeddev[i].__dev = _dev;
				loadeddev[i].devinitpar = __par;
				loadeddev[i].init = initf;
				loadeddev[i].deinit = deinitf;
				loadeddev[i].devattraddr = reinterpret_cast<unsigned long long>(attr);
				devinstll[_dev - DEVBASE] = CONFIGURED_NOT_INSTALLED;
				devLocList[_dev - DEVBASE] = i;
				return i;
		}
	}
	return -2;
}
constexpr devExInstProg devinstallstate(unsigned _dev)
{
	return devinstll[_dev - DEVBASE];
}
inline unsigned long long getDevAttr(unsigned _dev, unsigned n)
{
	return reinterpret_cast<unsigned long long*>(loadeddev[devLocList[_dev - DEVBASE]].devattraddr)[n];
}
