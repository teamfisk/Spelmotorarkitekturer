#include "StackAllocator.h"
#include <vector>
#include <iostream>

// Replace the default new with another version. Note: replaced globally.
//void* operator new(std::size_t sz){
//	std::printf("global op new called, size = %zu\n",sz);
//return std::malloc(sz);
//}

StackAllocator gStackAllocator(10'000'000);

struct GameObjectFoo
{
	GameObjectFoo()
	{
		this->swagLevel = -1;
	}
	GameObjectFoo(int swagLevel)
	{
		this->swagLevel = swagLevel;
	}
	int data;
	int health;
	int ammo;
	int swagLevel;
};

void main()
{	
	StackAllocator stackAllocator(1e6);

	// Allocates memory for a object, doesn't initialize the object
	GameObjectFoo* foo = stackAllocator.allocate<GameObjectFoo>();							// Version 1
	GameObjectFoo* gobj = (GameObjectFoo*)stackAllocator.allocate(sizeof(GameObjectFoo));   // Version 2
		
	// Using placement new
	foo = new(foo)GameObjectFoo(420);									// Version 1: using the memory from a pointer	
	GameObjectFoo* bestFoo = new (stackAllocator) GameObjectFoo(420);	// Version 2: allocating from the stackAllocator

	// Needs a bit more stuff before it can be used as an std::allocator ...
	// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2271.html#std_allocator
	//std::vector<float, StackAllocator<float>> v;
	//v.reserve(100);	
}