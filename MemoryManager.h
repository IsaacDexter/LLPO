#pragma once
#include <cstdlib>			//For malloc and free

void* operator new(size_t size);
void operator delete (void* pMem);