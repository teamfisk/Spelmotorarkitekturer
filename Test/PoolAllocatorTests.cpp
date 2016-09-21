#include <thread>
#include <vector>
#include <iostream>
#include <stdlib.h>

#include "catch.hpp"
#include "../Allocator/PoolAllocator.h"


// Test a simple case of allocating memory.
TEST_CASE("POOL_Allocate", "[PoolAllocator][Func]")
{
	unsigned int amount = 50;

    // Allocate memory with the size of [amount] ints.
    PoolAllocator<int> pool(amount);

	// Make sure the right amount of bytes are allocated
	REQUIRE(pool.Size() == amount * sizeof(int));

	// Fill the pool with data
	for (int i = 0; i < amount; i++) {
        pool.Allocate(i);
	}

	REQUIRE(pool.Size() == amount * sizeof(int));
	REQUIRE(pool.SizeInBlocks() == amount);
}

// Test a simple case of freeing memory.
TEST_CASE("POOL_Free", "[PoolAllocator][Func]")
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
	
	//Check to see if we have one less block used.
	REQUIRE(pool.OccupiedBlocks() == amount - 1);
}

// Test several continuous allocations and free's to see if the
// memory stays intact without errors or diskfragmentation.
TEST_CASE("POOL_ContinuousAllocationAndFree", "[PoolAllocator][Func]")
{
	unsigned int amount = 10;

	PoolAllocator<int> pool(amount);

	// Fill the pool with 7 elements
	for (int i = amount*0.7; i > 0; i--) {
		pool.Allocate(i);
	}

	// Free the first block.
	auto fBlock = pool.begin();
	pool.Free(&(*fBlock));

	// Free the new first block, should be the second block in memory.
	auto fBlock2 = pool.begin();
	pool.Free(&(*fBlock2));


	// Allocate 3 more blocks
	for (int i = 3; i > 0; i--) {
		pool.Allocate(i*10);
	}

	// Free the fifth block
	auto p = pool.begin();
	for (int i = 1; i < 5; i++) {
		p++;
	}
	pool.Free(&(*p));

	// Allocate 6 ints, 3 more than we can fit.
	for (int i = 1; i < 7; i++) {
		pool.Allocate(i * 100);
	}

	//Simple loop to check if our memory is the same as what our answer.
	int Answer[10] = { 30, 20, 5, 4, 100, 2, 1, 10, 200, 300 };
	int i = 0;
	bool Same = true;
	for (auto p : pool) {
		if (Answer[i] != p) {
			Same = false;
			break;
		}
		i++;
	}
	REQUIRE(Same);
}

// Check the available space left in the memory pool
// to make sure it's calculated correctly.
TEST_CASE("POOL_AvailableSpace", "[PoolAllocator][Func]")
{
	unsigned int amount = 10;

	PoolAllocator<int> intPool(amount);
	PoolAllocator<float> floatPool(amount);

	REQUIRE(intPool.Size() == amount * sizeof(int));
	REQUIRE(floatPool.Size() == amount * sizeof(float));

	float f = 0.f;
	int i = 0;
	while (i < 5) {
		intPool.Allocate(i);
		floatPool.Allocate(f);
		i++;
		f += 1.f;
	}
	
	REQUIRE(intPool.SizeFree() == 5 * sizeof(int));
	REQUIRE(floatPool.SizeFree() == 5 * sizeof(float));
}



//------------------------------------------
// Performance tests
// These are tests that are designed to try
// the difference in performance between
// the standard OS allocator and our 
// custom made pool allocator.
//------------------------------------------

// Create 5 pools with 2MB of allocated memory for each using 
// our custom pool allocator.
TEST_CASE("POOL_2MBAlloc", "[PoolAllocator][Perf]") 
{
	unsigned int amount = 2 * 1024 * 1024;
	void* charArr[5];
	for (int i = 0; i < 5; i++)
	{
		charArr[i] = new PoolAllocator<char>(amount);
	}
}

// Create 5 pools with 2MB of allocated memory for each using 
// standard OS allocator
TEST_CASE("MALLOC_2MBAlloc", "[StandardAllocator][Perf]")
{
	unsigned int amount = 2 * 1024 * 1024;
	void* charArr[5];
	for (int i = 0; i < 5; i++) {
		charArr[i] = malloc(amount * sizeof(char));
	}
}

// Create 5 pools with 200MB of allocated memory for each using 
// our custom pool allocator.
TEST_CASE("POOL_200MBAlloc", "[PoolAllocator][Perf]")
{
	unsigned int amount = 200 * 1024 * 1024;
	void* charArr[5];
	for (int i = 0; i < 5; i++)
	{
		charArr[i] = new PoolAllocator<char>(amount);
	}
}

// Create 5 pools with 200MB of allocated memory for each using 
// standard OS allocator
TEST_CASE("MALLOC_200MBAlloc", "[StandardAllocator][Perf]")
{
	unsigned int amount = 200 * 1024 * 1024;
	void* charArr[5];
	for (int i = 0; i < 5; i++) {
		charArr[i] = malloc(amount * sizeof(char));
	}
}

// Allocate 100 000 ints and read them using our custom pool allocator.
// I'm printing every 1000th int in the hope that no unwanted optimizing will occur.
TEST_CASE("POOL_LargeDataRead", "[PoolAllocator][Perf]")
{
	unsigned int amount = 100'000;
	PoolAllocator<int> pool(amount);
	
	for (int i = 0; i < amount; i++) {
		pool.Allocate(i);
	}

	for (auto p : pool) {
		if (p%1000 == 0) {
			p += 1;
		}
	}
	std::cout << std::endl;
}

// Allocate 100 000 ints and read them using the standard OS allocator.
TEST_CASE("MALLOC_LargeDataRead", "[StandardAllocator][Perf]")
{
	unsigned int amount = 100'000;
	int* arr = new int[amount];
	for (int i = 0; i < amount; i++) {
		arr[i] = i;
	}

	for (int i = 0; i < amount; i++) {
		if (arr[i]%1000 == 0) {
			arr[i] += 1;
		}
	}
	std::cout << std::endl;
}



