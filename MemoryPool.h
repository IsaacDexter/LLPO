#pragma once
#include<array>
#include <iostream>

/*Why is it called a memory pool, but contains slices. There isn't really a term for an unspecified portion of water.
I propose a rename of a memory pool in the industry. They should be called a memory pie. That way the slices term would make sense. :bow:*/

template<size_t sliceCount, size_t sliceSize>
class MemoryPool
{
public:
	MemoryPool();
	~MemoryPool();
	void* Alloc(size_t iSize);
	void Dealloc(void* pMem);
	void* At(size_t index);
	void Output();
private:
	std::array<void*, sliceCount> slices;
	void* pPool;
};

template<size_t sliceCount, size_t sliceSize>
inline MemoryPool<sliceCount, sliceSize>::MemoryPool()
{
	//allocate a pool that could hold n objects of size i, and store a pointer to its first entry
	pPool = malloc(sliceSize * sliceCount);
	slices.fill(nullptr);
}

template<size_t sliceCount, size_t sliceSize>
inline MemoryPool<sliceCount, sliceSize>::~MemoryPool()
{
	free(pPool);
}

template<size_t sliceCount, size_t sliceSize>
inline void* MemoryPool<sliceCount, sliceSize>::Alloc(size_t iSize)
{
	//Check if what you are trying to allocate can fit in a slice
	if (iSize > sliceSize)
	{
		std::cerr << "MemoryPool:\tNo slices large enough in pool.\n";
		return nullptr;
	}
	
	//find the first free index
	auto it = std::find(slices.begin(), slices.end(), nullptr);
	if (it == slices.end())
	{
		//No free indexes, return nullptr and out an error
		std::cerr << "MemoryPool:\tNo free slices in pool.\n";
		return nullptr;
	}

	size_t index = it - slices.begin();
	void* pSlice = At(index);
	*it = pSlice;
	//void* pSlice = *it;

	return pSlice;
}

template<size_t sliceCount, size_t sliceSize>
inline void MemoryPool<sliceCount, sliceSize>::Dealloc(void* pMem)
{
	//find the index storing this item 
	auto it = std::find(slices.begin(), slices.end(), pMem);
	if (it == slices.end())
	{
		std::cerr << "MemoryPool:\tCould not deallocate, memory not found in pool.\n";
		return;
	}
	//Set that memory address, and the parameter, to point to nothing, preventing further access to that object. 
	*it = nullptr;
	pMem = nullptr;
	return;
}

template<size_t sliceCount, size_t sliceSize>
inline void* MemoryPool<sliceCount, sliceSize>::At(size_t index)
{
	//Check index is within range
	if (index >= sliceCount)
	{
		std::cerr << "MemoryPool:\tIndex out of range.\n";
		return nullptr;
	}
	//find a pointer to the slice at index by multiplying the index by size of slice and adding it to the pointer to the start of the pool
	//change it to a char* as char pointer is 1 byte. incrementing will increment by the size of char, 1 byte. void* would increment by the size of void, i.e. 0
	void* pMem = (char*)(pPool) + (index * sliceSize);
	
	return pMem;
}

template<size_t sliceCount, size_t sliceSize>
inline void MemoryPool<sliceCount, sliceSize>::Output()
{
	std::cout << "MemoryPool:\n\tSlice Count:\t" << sliceCount << "\n\tSlice Size:\t" << sliceSize << "\n\tPool Size:\t" << sliceCount * sliceSize << "\n\tContents:\n";
	for (size_t i = 0; i < sliceCount; i++)
	{
		std::cout << "\t\t" << i << ":\t" << std::hex << At(i) << std::dec << "\n" ;
	}
}
