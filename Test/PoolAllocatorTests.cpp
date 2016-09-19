#include <thread>
#include <vector>
#include <iostream>

#include "catch.hpp"
#include "../Allocator/PoolAllocator.h"

// Undefine to use normal memory allocator
#define POOL_ALLOCATOR

// Test a simple case of allocating memory.
TEST_CASE("Allocation", "[PoolAllocator]")
{
    // Allocate memory with the size of 50 ints.
    PoolAllocator<int> pool(50);

	for (int i = 0; i < 50; i++) {
        pool.Allocate(i);
	}

	std::cout << pool.size() << std::endl;
}

// Test a simple case of freeing memory.
TEST_CASE("Free", "[PoolAllocator]")
{

}

// Test several continuous allocations and free's to see if the
// memory stays intact without errors or diskfragmentation.
TEST_CASE("ContinuousAllocationAndFree", "[PoolAllocator]")
{

}

// Test using more memory than we have allocated and see that it is
// handled correctly.
TEST_CASE("Boundaries", "[PoolAllocator]")
{

}

// Check the available space left in the memory pool
// to make sure it's calculated correctly.
TEST_CASE("AvailableSpace", "[PoolAllocator]")
{

}



