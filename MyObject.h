#pragma once
#include <iostream>

class MyObject
{
public:
	MyObject();
	~MyObject();
	/// <summary>Overwrite the new operator for this class to allow tracking of memory.</summary>
	/// <param name="size"></param>
	/// <returns>Void pointer to the start of the memory allocated to the object.</returns>
	static void* operator new(size_t size);
	/// <summary>Overwrites class delete function to allow memory to be kept track of.</summary>
	/// <param name="p">void pointer to the start of memory to be unallocated.</param>
	/// <param name="size"></param>
	static void operator delete(void* p, size_t size);
private:
	int number = 16;
};

