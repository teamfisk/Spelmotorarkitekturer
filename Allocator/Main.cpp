#include "StackAllocator.h"
#include <vector>
#include <iostream>

void main()
{
	StackAllocator stackAllocator(1e6);

	StackAllocator::MemBlock block1 = stackAllocator.allocate(2000);
	//StackAllocator::Marker marker = stackAllocator.getMarker();
	auto marker = stackAllocator.getMarker();
	
	auto block2 = stackAllocator.allocate(5000);
	auto block3 = stackAllocator.allocate(10000);

	// Try allocating more memory than exists in the allocator. 
	try
	{
		auto block4 = stackAllocator.allocate(10e7);
	}
	catch (const char* c)
	{
		std::cerr << c << std::endl;
	}

	unsigned int spaceBeforeFree = stackAllocator.getAvailableSpace();
	stackAllocator.freeToMarker(marker);
	unsigned int spaceAfterFree = stackAllocator.getAvailableSpace();

	// Needs a bit more stuff before it can be used as an std::allocator ...
	// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html#std_allocator
	//std::vector<float, StackAllocator<float>> v;
	//v.reserve(100);	
}