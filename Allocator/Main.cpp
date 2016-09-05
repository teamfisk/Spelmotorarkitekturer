#include "StackAllocator.h"
#include <vector>
#include <iostream>

// Replace the default new with another version. Note: replaced globally.
//void* operator new(std::size_t sz){
//	std::printf("global op new called, size = %zu\n",sz);
//return std::malloc(sz);
//}

StackAllocator gStackAllocator(10'000'000);

void* operator new(std::size_t sz)
{
	return gStackAllocator.allocate(sz);
}

struct GameObjectFoo
{
	int data;
	int health;
	int ammo;
	int swagLevel;
};

void main()
{
	auto g = new int;

	auto foo = new GameObjectFoo();

	StackAllocator stackAllocator(1e6);

	void* block1 = stackAllocator.allocate(2000);
	//StackAllocator::Marker marker = stackAllocator.getMarker();
	auto marker = stackAllocator.getMarker();
	
	auto block2 = stackAllocator.allocate(5000);
	auto block3 = stackAllocator.allocate(10000);

	GameObjectFoo* gobj = (GameObjectFoo*)stackAllocator.allocate(sizeof(GameObjectFoo));
	gobj->swagLevel = 420;

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