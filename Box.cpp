#include "Box.h"

MemoryPool<BOX_COUNT, sizeof(Box)> boxPool {};

void* Box::operator new(size_t size)
{
    return boxPool.Alloc(size);
}

void Box::operator delete(void* pMem, size_t size)
{
    return boxPool.Dealloc(pMem);
}
