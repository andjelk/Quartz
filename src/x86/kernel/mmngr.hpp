#pragma once
typedef unsigned char* p_addr;
struct mmngr_info
{
	p_addr bitmap_offset;
	p_addr bitmap_end;
	p_addr blockAlloc_offset;
	size_t totalBlocks;
};
int init_mmngr(int phMemSize);
void* k_phys_malloc(size_t s);
size_t k_phys_free(void* ptr);
void *kmalloc(size_t s);
size_t kfree(void *ptr);
extern mmngr_info p_mmngr;
