typedef void(__cdecl *__INTT)(void);
#pragma data_seg(".CRT$XCA")
__INTT __xc_a[] = { 0 };
#pragma data_seg(".CRT$XCZ")
__INTT __xc_z[] = { 0 };
#pragma data_seg()
#pragma comment(linker, "/merge:.CRT=.data")
int __cdecl ::_purecall()
{
	_asm
	{
		hlt
	}
	return 0;
}
extern "C"
{
	float __declspec(naked) _CIcos()
	{
		_asm
		{
			fcos
			ret
		}
	}
	float __declspec(naked) _CIsin()
	{
		_asm
		{
			fsin
			ret
		}
	}
	float __declspec(naked) _CIsqrt()
	{
		_asm
		{
			fsqrt
			ret
		}
	}
	long __declspec (naked) _ftol2_sse()
	{
		int a;
		_asm
		{
			fistp [a]
			mov	ebx, a
			ret
		}
	}
	__declspec(naked) void __cdecl _aullshr()
	{
		__asm
		{
			cmp     cl, 64
			jae     short RETZERO
			cmp     cl, 32
			jae     short MORE32
			shrd    eax, edx, cl
			shr     edx, cl
			ret
			MORE32:
				mov     eax, edx
				xor     edx, edx
				and     cl, 31
				shr     eax, cl
				ret
			RETZERO:
				xor     eax, eax
				xor     edx, edx
				ret
		}
	}
	int _fltused = 1;
}
static __INTT * pf_atexitlist = (__INTT *)0x50000;
static unsigned max_atexitlist_entries = 32;
static unsigned cur_atexitlist_entries = 0;
void __cdecl _initterm(__INTT * pfbegin, __INTT * pfend) {

	while (pfbegin < pfend)
	{
		if (*pfbegin != 0)
			(**pfbegin) ();
		++pfbegin;
	}
}
int __cdecl atexit(__INTT fn)
{
	if (cur_atexitlist_entries >= max_atexitlist_entries)
		return 1;
	else
	{
		*(pf_atexitlist++) = fn;
		cur_atexitlist_entries++;
	}
	return 0;
}
void _cdecl exitglobal()
{
	while (cur_atexitlist_entries--)(*(--pf_atexitlist))();
}
void _cdecl initglobal()
{
	_initterm(__xc_a, __xc_z);
}

