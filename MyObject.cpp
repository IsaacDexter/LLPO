#include "MyObject.h"
#include <iostream>
#include "MemoryTracker.h"

MyObject::MyObject()
{
	//std::cout << "MyObject::MyObject()\n";
}

MyObject::~MyObject()
{
	//std::cout << "MyObject::~MyObject()\n";
}

void* MyObject::operator new(size_t size)
{
	ObjectTracker::AddBytesAllocated(size);
	//return global new
	return ::operator new(size);
}

void MyObject::operator delete(void* p, size_t size)
{
	ObjectTracker::RemoveBytesAllocated(size);
	//return global delete
	::operator delete(p);
}
