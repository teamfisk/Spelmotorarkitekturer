#include <thread>
#include <vector>
#include <iostream>

#include "catch.hpp"
#include "../Allocator/PoolAllocator.h"

// Undefine to use normal memory allocator
#define POOL_ALLOCATOR

// Test a simple case of allocating memory.
TEST_CASE("POOL_Allocate", "[PoolAllocator]")
{
	unsigned int amount = 50;

    // Allocate memory with the size of [amount] ints.
    PoolAllocator<int> pool(amount);

	// Fill the pool with data
	for (int i = 0; i < amount; i++) {
        pool.Allocate(i);
	}

	REQUIRE(pool.Size() == amount * sizeof(int));
	REQUIRE(pool.SizeInBlocks() == amount);
}

// Test a simple case of freeing memory.
TEST_CASE("POOL_Free", "[PoolAllocator]")
{
	unsigned int amount = 10;

	// Allocate memory with the size of [amount] floats.
	PoolAllocator<float> pool(amount);

	// Fill the pool so we have something to free.
	for (int i = 0; i < amount; i++) {
		pool.Allocate(i%3);
	}
	// Check to make sure we have the right amounts of floats in the memory.
	REQUIRE(pool.OccupiedBlocks() == amount);
	
	//Free a block in the memory.
	auto fBlock = pool.begin();
	fBlock++;
	fBlock++;
	pool.Free(&(*fBlock));
	std::cout << "Free block " << *fBlock+1 << std::endl;
	
	//Check to see if we have one less block used.
	REQUIRE(pool.OccupiedBlocks() == amount - 1);

}

// Test several continuous allocations and free's to see if the
// memory stays intact without errors or diskfragmentation.
TEST_CASE("POOL_ContinuousAllocationAndFree", "[PoolAllocator]")
{

}

// Test using more memory than we have allocated and see that it is
// handled correctly.
TEST_CASE("POOL_Boundaries", "[PoolAllocator]")
{

}

// Check the available space left in the memory pool
// to make sure it's calculated correctly.
TEST_CASE("POOL_AvailableSpace", "[PoolAllocator]")
{

}



