#include "MemoryManager.h"
#include "MemoryTracker.h"
#include <iostream>

Header* pFirst { nullptr };
Header* pLast { nullptr };

void* operator new(size_t size)
{
    size_t totalSize = sizeof(Header) + size + sizeof(Footer);  //Find the total number of bytes by header + content + footer.
    DefaultTracker::AddBytesAllocated(totalSize);
    char* pMem = (char*)malloc(totalSize);  //Allocate total size

    Header* pHeader = (Header*)pMem;    //Set the header's pointer to the start of allocated memory
    pHeader->size = size;   //Set the size in the header equal to the size of the content
    pHeader->check = 0x4EAD4EAD;
    pHeader->pPrev = pLast;
    pHeader->pNext = nullptr;

    if (pLast != nullptr)
    {
        pLast->pNext = pHeader;
    }

    pLast = pHeader;
    if (pFirst == nullptr)
    {
        pFirst = pHeader;
    }

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

    //If this is the first header (has no pPrev)
    if (pHeader == pFirst)
    {
        //Make this' next the new first
        pFirst = pHeader->pNext;
    }
    else
    {
        //Set the previous header to point to the next header
        pHeader->pPrev->pNext = pHeader->pNext;
    }
    //If this is the last header (has no next)
    if (pHeader == pLast)
    {
        //Make this' previous the new last
        pLast = pHeader->pPrev;
    }
    else
    {
        //Set the next header to point to the last header
        pHeader->pNext->pPrev = pHeader->pPrev;
    }

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