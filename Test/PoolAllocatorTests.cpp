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
    // Allocate memory with the size of 50 ints.
    PoolAllocator<int> pool(50);

	for (int i = 0; i < 50; i++) {
        pool.Allocate(i);
	}

	std::cout << pool.size() << std::endl;
}

// Test a simple case of freeing memory.
TEST_CASE("POOL_Free", "[PoolAllocator]")
{

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



