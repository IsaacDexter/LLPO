#include "MemoryManager.h"
#include <iostream>

void* operator new (size_t size)
{
	std::cout << "New.\n";
	char* pMem = (char*)malloc(size);
	void* pStartMemBlock = pMem;
	return pStartMemBlock;
}

void operator delete (void* pMem)
{
	std::cout << "Delete.\n";
	free(pMem);
}