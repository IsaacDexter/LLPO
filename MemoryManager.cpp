#include "MemoryManager.h"
#include "MemoryTracker.h"
#include <iostream>

void* operator new(size_t size)
{
    size_t totalSize = sizeof(Header) + size + sizeof(Footer);  //Find the total number of bytes by header + content + footer.
    DefaultTracker::AddBytesAllocated(totalSize);
    char* pMem = (char*)malloc(totalSize);  //Allocate total size

    Header* pHeader = (Header*)pMem;    //Set the header's pointer to the start of allocated memory
    pHeader->size = size;   //Set the size in the header equal to the size of the content
    pHeader->check = 0x4EAD4EAD;

    Footer* pFooter = (Footer*)(pMem + sizeof(Header) + size);  //Set the footer's pointer to after allocated memory, from the start of total allocated memory + header's size + content's size
    pFooter->check = 0xF007F007;

    //std::cout << "global new, allocating total size " << std::dec << totalSize << " (Header: " << sizeof(*pHeader) << ", Content:" << pHeader->size << ", Footer:" << sizeof(*pFooter) << "), ";
    //std::cout << "Header = " << std::hex << pHeader->check << ", Footer = " << pFooter->check << "\n";

    void* pStartMemBlock = pMem + sizeof(Header);   //return the start of content (start of allocated memory + size of the header)
    return pStartMemBlock;
}

void operator delete (void* pMem)
{
    Header* pHeader = (Header*)((char*)pMem - sizeof(Header));  //Find the pointer to the header by finding the start of content and stepping back by the size of the header
    Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);   //Find the pointer to the footer by finding the start of the content and adding the size of the content

    //std::cout << "global delete, deallocating total size " << std::dec << sizeof(*pHeader) + pHeader->size + sizeof(*pFooter) << " (Header: " << sizeof(*pHeader) << ", Content:" << pHeader->size << ", Footer:" << sizeof(*pFooter) << "), ";
    //std::cout << "Header = " << std::hex << pHeader->check << ", Footer = " << pFooter->check << "\n";

    DefaultTracker::RemoveBytesAllocated(sizeof(*pHeader) + pHeader->size + sizeof(*pFooter));
    free(pHeader);  //Deallocate entire block starting at header
}

/*
new
1. class new
2. global new
3. constructor

delete
1. destructor
2. class delete
3. global delete
*/