#pragma once
#include <iostream>
#include <string>

/// <summary>Stores statistics about memory allocation. Category is used in outputing when paired with an appropriate sub-tracker</summary>
struct MemoryData
{
	const char* category;
	size_t bytesAllocated;
	size_t bytesDeallocated;
	size_t bytes;
	MemoryData(const char* category)
	{
		this->category = category;
		this->bytesAllocated = 0;
		this->bytesDeallocated = 0;
		this->bytes = 0;
	}
};

//Forward declare subclasses

class DefaultTracker;
class ObjectTracker;

/// <summary>Declare MemoryTracker as a template class for CRTP, allowing the static method's static members variables to be overwritten in children classes.</summary>
template<class T>
class MemoryTracker
{
protected :
	//Stats must be overwritten in implemented subclasses using:
	//	MemoryData MemoryTracker<DefaultTracker>::m_stats("Default"); 
	//	class DefaultTracker : public MemoryTracker<DefaultTracker>{};
	static MemoryData m_stats;
public:

	static inline void AddBytesAllocated(size_t numberOfBytes)
	{
		m_stats.bytesAllocated += numberOfBytes;
		m_stats.bytes += numberOfBytes;
	}
	static inline void RemoveBytesAllocated(size_t numberOfBytes)
	{
		m_stats.bytesDeallocated += numberOfBytes;
		m_stats.bytes -= numberOfBytes;
	}

	//Ouputs the formatted stats of memory allocation in the program
	static inline void OutputStats()
	{
		printf("%s:\nBytes Allocated: %zu\nBytes Deallocated: %zu\nBytes Total: %zu\n", m_stats.category, m_stats.bytesAllocated, m_stats.bytesDeallocated, m_stats.bytes);
	}

	static inline void OutputHeader(Header* pHeader)
	{
		Footer* pFooter = (Footer*)((char*)pHeader + sizeof(Header) + pHeader->size);   //Find the pointer to the footer by finding the start of the content and adding the size of the content
		std::cout << std::hex << pHeader << ":\n\tCheck:\t" << pHeader->check << "\n\tPrev:\t"  << pHeader->pPrev << "\n\tNext:\t" << pHeader->pNext << "\n\tFooter:\n\t\t";
		std::cout << pFooter << ":\n\t\tCheck:\t" << pFooter->check << "\n\tSize:\t" << std::dec << pHeader->size << "\n\n";
	}

	static inline void WalkHeap()
	{
		Header* pHeader = pFirst;
		while (pHeader != nullptr)
		{
			OutputHeader(pHeader);
			pHeader = pHeader->pNext;
		}
	}

	MemoryTracker() = delete;
};

//Implements static children of static class using CRTP

MemoryData MemoryTracker<DefaultTracker>::m_stats("Default");
class DefaultTracker : public MemoryTracker<DefaultTracker>{};

MemoryData MemoryTracker<ObjectTracker>::m_stats("Objects");
class ObjectTracker : public MemoryTracker<ObjectTracker>{};
