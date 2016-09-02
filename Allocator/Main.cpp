#include "StackAllocator.h"
#include <vector>

void main()
{
	StackAllocator<float> myAllocator;
	myAllocator.allocate(100);

	// Needs a bit more stuff before it can be used as an std::allocator ...
	// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html#std_allocator
	//std::vector<float, StackAllocator<float>> v;
	//v.reserve(100);	
}