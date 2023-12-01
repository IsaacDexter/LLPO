#pragma once

/// <summary>Overwrites the global new function to allow memory to be kept track of.</summary>
/// <param name="size"></param>
/// <returns>Void pointer to the start of the memory allocated to the object.</returns>
void* operator new (size_t size);

/// <summary>Overwrites global delete function to allow memory to be kept track of.</summary>
/// <param name="pMem">void pointer to the start of memory to be unallocated.</param>
void operator delete (void* pMem);

struct Header
{
	/// <summary>Size of main allocated section</summary>
	size_t size;
	int check;
	Header* pNext;
	Header* pPrev;
};

struct Footer
{
	int check;
};

extern Header* pFirst;
extern Header* pLast;