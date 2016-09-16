#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "../Allocator/PoolAllocator.h"

// Undefine to use normal memory allocator
#define POOL_ALLOCATOR

// Test a simple case of allocating memory.
TEST(PoolAllocator, Allocation)
{
    // Allocate memory with the size of 50 ints.
    PoolAllocator<int> pool(50);

    for (int i = 0; i < 50; i++) {
        pool.Allocate(i);
    }
}

// Test a simple case of freeing memory.
TEST(PoolAllocator, Free)
{

}

// Test several continuous allocations and free's to see if the
// memory stays intact without errors or diskfragmentation.
TEST(PoolAllocator, ContinuousAllocationAndFree)
{

}

// Test using more memory than we have allocated and see that it is
// handled correctly.
TEST(PoolAllocator, Boundaries)
{

}

// Check the available space left in the memory pool
// to make sure it's calculated correctly.
TEST(PoolAllocator, AvailableSpace)
{

}



