#include"mmngr.hpp"
#include"..\library\string.hpp"
#include"errcodes.hpp"
mmngr_info p_mmngr;
#define NOT_IGNORE_UNIMPLEMENTATION 0
#define PhMemBitmapBlockSize 1 //One ...
#define PhMemBitmapBlockUnit 1024 // ... Kilobyte.
#define PhMemBitmapActualBlockSize (PhMemBitmapBlockSize*PhMemBitmapBlockUnit)
#define ROUND_BASE 0x100000
#define BITMAP_BASE_ADDRESS 0x100000
#define MALLOC_BASE_ADDRESS 0x150000
/*
For now, we will use a simple memory physical block manager by bitmap on blockAlloc.
*/
int init_pm_mngr(int phMemSize)
{
	const int start = (BITMAP_BASE_ADDRESS); //p_mmngr.blockAlloc
	const int mmst = (MALLOC_BASE_ADDRESS); //p_mmngr.AllocMemStart
	const int kbma = ((phMemSize - mmst / 1024))/PhMemBitmapBlockSize;
	const int size = kbma / 8;
	const int end = start + size; //p_mmngr.endBlockAlloc
	p_mmngr.blockAlloc = reinterpret_cast<unsigned char*>(start);
	p_mmngr.endBlockAlloc = reinterpret_cast<unsigned char*>(end);
	p_mmngr.totalBlocks = kbma;
	p_mmngr.AllocMemStart = reinterpret_cast<unsigned char*>(mmst);
	memset(p_mmngr.blockAlloc, 0, size);
	return 0;
}
void* k_phys_malloc(size_t s)
{
	size_t blocks = s / PhMemBitmapActualBlockSize;
	if (s% PhMemBitmapActualBlockSize)
	{
		blocks++;
	}
	size_t index = 0;
	size_t _byte = 0, _bit = 0, _segment = 0;
	size_t _sbyte = 0, _sbit = 0;
	for (; index < p_mmngr.totalBlocks; index++)
	{
		if (((p_mmngr.blockAlloc[_sbyte] >> _sbit) & 1))_segment = 0;
		else
		{
			_segment++;
			if (_segment == blocks)
			{
				void* ptr = p_mmngr.AllocMemStart + ((_byte * 8 + _bit)* PhMemBitmapActualBlockSize);
				unsigned char* bmptr = p_mmngr.blockAlloc + _byte;
				if (_bit)
				{
					const size_t rmtobi = 8 - _bit;
					const size_t acrem = _segment - rmtobi;
					const size_t fullbmblkb = acrem / 8;
					const size_t rmndfmblk = acrem % 8;
					if (rmtobi >= blocks)*(bmptr) |= (static_cast<unsigned char>(0xFF << 8 - blocks) >> rmtobi - blocks);
					else
					{
						*(bmptr) |= ((0xFF << _bit));
						memset(bmptr + 1, 0xFF, fullbmblkb);
						*(bmptr + fullbmblkb + 1) |= (0xFF >> 8 - rmndfmblk);
					}
				}
				else
				{
					const size_t fullbmblkb = (_segment) / 8;
					const size_t rmndfmblk = _segment % 8;
					memset(bmptr, 0xFF, fullbmblkb);
					*(bmptr + fullbmblkb) |= (0xFF >> 8 - rmndfmblk);
				}
				*(reinterpret_cast<size_t*>(ptr)) = s;
				return reinterpret_cast<size_t*>(ptr) + 1;
			}
		}
		_sbit++;
		if (_sbit == 8)
		{
			_sbit = 0;
			_sbyte++;
		}
		if (_segment == 0)
		{
			_byte = _sbyte;
			_bit = _sbit;
		}
	}
	return nullptr;
}
void k_phys_free(void* ptr)
{
	unsigned char* _ptr = reinterpret_cast<unsigned char*>(ptr) - sizeof(int) / sizeof(unsigned char);
	const size_t blocks = *reinterpret_cast<size_t*>(_ptr);
	*reinterpret_cast<size_t*>(_ptr) = 0;
	size_t index = reinterpret_cast<size_t>(_ptr);
	index -= reinterpret_cast<size_t>(p_mmngr.AllocMemStart);
	index /= PhMemBitmapActualBlockSize;
	unsigned char* bmptr = p_mmngr.blockAlloc + index / 8;
	const size_t rmndri = index % 8;
	if (rmndri)
	{
		const size_t rmtobi = 8 - rmndri;
		const size_t acrem = blocks - rmtobi;
		const size_t fullbmblkb = acrem / 8;
		const size_t rmndfmblk = acrem % 8;
		if (rmtobi >= blocks)*(bmptr) ^= (static_cast<unsigned char>(0xFF << 8 - blocks) >> rmtobi - blocks);
		else
		{
			*(bmptr) ^= ((0xFF << rmndri));
			memset(bmptr + 1, 0x00, fullbmblkb);
			*(bmptr + fullbmblkb + 1) ^= (0xFF >> 8 - rmndfmblk);
		}
	}
	else
	{
		const size_t fullbmblkb = (blocks) / 8;
		const size_t rmndfmblk = blocks % 8;
		memset(bmptr, 0x00, fullbmblkb);
		*(bmptr + fullbmblkb) ^= (0xFF >> 8 - rmndfmblk);
	}
}
int init_vm_mngr()
{
	return _UNIMPLEMENTED; //Virtual memory not implemented.
}
int init_mmngr(int phMemSize)
{
	int res=init_pm_mngr(phMemSize);
	if (!res)
	{
		res=init_vm_mngr();
		if (NOT_IGNORE_UNIMPLEMENTATION&&res==_UNIMPLEMENTED)return res;
		else return 0;
	}
	else return res;
}
