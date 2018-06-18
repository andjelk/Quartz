#include"mmngr.hpp"
#include<string.hpp>
#include"errcodes.hpp"
mmngr_info p_mmngr;
#define NOT_IGNORE_UNIMPLEMENTATION 0
#define PhMemBitmapBlockSize 1 //One ...
#define PhMemBitmapBlockUnit 1024 // ... Kilobyte.
#define PhMemBitmapActualBlockSize (PhMemBitmapBlockSize*PhMemBitmapBlockUnit)
#define ROUND_BASE 0x50000
#define BITMAP_BASE_ADDRESS 0x100000
#define MALLOC_BASE_ADDRESS 0x150000
/*
For now, we will use a simple memory physical block manager by bitmap on blockAlloc.
phMemSize is in kilobytes
*/
struct phys_blk_header
{
	size_t n_alloc;
};
struct phys_blk
{
	phys_blk_header head;
	unsigned char data[PhMemBitmapActualBlockSize];
};
#define PhMemSizeUnit 1024
int init_pm_mngr(int phMemSize)
{
	const size_t actualMemSize = phMemSize*PhMemSizeUnit;
	if (actualMemSize < MALLOC_BASE_ADDRESS || actualMemSize < BITMAP_BASE_ADDRESS)
		return _NOT_ENOUGH_MEM;
	const size_t start = (BITMAP_BASE_ADDRESS); //p_mmngr.blockAlloc
	const size_t mmst = (MALLOC_BASE_ADDRESS); //p_mmngr.AllocMemStart
	const size_t kbma = (actualMemSize-mmst)/sizeof(phys_blk);
	size_t size = kbma / 8;
	if (kbma % 8)size++;
	const size_t end = start + size; //p_mmngr.endBlockAlloc
	p_mmngr.bitmap_offset = reinterpret_cast<p_addr>(start);
	p_mmngr.bitmap_end = reinterpret_cast<p_addr>(end);
	p_mmngr.totalBlocks = kbma;
	p_mmngr.blockAlloc_offset = reinterpret_cast<p_addr>(mmst);
	memset(p_mmngr.bitmap_offset, 0, size);
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
		if (((p_mmngr.bitmap_offset[_sbyte] >> _sbit) & 1))_segment = 0;
		else
		{
			_segment++;
			if (_segment == blocks)
			{
				phys_blk *ptr = (phys_blk*)(p_mmngr.blockAlloc_offset) + (_byte * 8 + _bit);
				p_addr bmptr = p_mmngr.bitmap_offset + _byte;
				if (_bit)
				{
					const size_t rmtobi = 8 - _bit;
					const size_t acrem = _segment - rmtobi;
					const size_t fullbmblkb = acrem / 8;
					const size_t rmndfmblk = acrem % 8;
					if (rmtobi >= blocks)*(bmptr) |= (static_cast<unsigned char>(0xFF << (8 - blocks)) >> (rmtobi - blocks));
					else
					{
						*(bmptr) |= (0xFF << _bit);
						memset(bmptr + 1, 0xFF, fullbmblkb);
						*(bmptr + fullbmblkb + 1) |= (0xFF >> (8 - rmndfmblk));
					}
				}
				else
				{
					const size_t fullbmblkb = (_segment) / 8;
					const size_t rmndfmblk = _segment % 8;
					memset(bmptr, 0xFF, fullbmblkb);
					*(bmptr + fullbmblkb) |= (0xFF >> (8 - rmndfmblk));
				}
				ptr->head.n_alloc = s;
				return ptr->data;
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
size_t k_phys_free(void* ptr)
{
	phys_blk* _ptr = reinterpret_cast<phys_blk *>((phys_blk_header*)(ptr) - 1);
	const size_t blocks = _ptr->head.n_alloc;
	size_t index = (reinterpret_cast<p_addr>(_ptr) - reinterpret_cast<p_addr>(p_mmngr.blockAlloc_offset)) / sizeof(phys_blk);
	unsigned char* bmptr = p_mmngr.bitmap_offset + index / 8;
	const size_t rmndri = index % 8;
	if (rmndri)
	{
		const size_t rmtobi = 8 - rmndri;
		const size_t acrem = blocks - rmtobi;
		const size_t fullbmblkb = acrem / 8;
		const size_t rmndfmblk = acrem % 8;
		if (rmtobi >= blocks)*(bmptr) ^= (static_cast<unsigned char>(0xFF << (8 - blocks)) >> (rmtobi - blocks));
		else
		{
			*(bmptr) ^= (0xFF << rmndri);
			memset(bmptr + 1, 0x00, fullbmblkb);
			*(bmptr + fullbmblkb + 1) ^= (0xFF >> (8 - rmndfmblk));
		}
	}
	else
	{
		const size_t fullbmblkb = (blocks) / 8;
		const size_t rmndfmblk = blocks % 8;
		memset(bmptr, 0x00, fullbmblkb);
		*(bmptr + fullbmblkb) ^= (0xFF >> (8 - rmndfmblk));
	}
	size_t freeCount=_ptr->head.n_alloc;
	_ptr->head.n_alloc = 0;
	return freeCount;
}
void *kmalloc(size_t s)
{
	return k_phys_malloc(s);
}
size_t kfree(void *ptr)
{
	return k_phys_free(ptr);
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
		if (NOT_IGNORE_UNIMPLEMENTATION&&(res==_UNIMPLEMENTED))return res;
		else return 0;
	}
	else return res;
}
