#pragma once
struct mmngr_info
{
	unsigned char* blockAlloc;
	unsigned char* endBlockAlloc;
	unsigned char* AllocMemStart;
	unsigned int totalBlocks;
};
int init_mmngr(int phMemSize);
void* k_phys_malloc(size_t s);
void k_phys_free(void* ptr);
extern mmngr_info p_mmngr;
