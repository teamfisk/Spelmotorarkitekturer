#include <thread>
#include <vector>
#include <cstdlib>
#include <cstring>

#include "catch.hpp"
#include "../Allocator/StackAllocator.h"

// Undef to switch to regular memory allocator
#define STACK_ALLOCATOR

const unsigned int allocationSize = 1000;

void ThreadFunction(StackAllocator & stackAllocator, char *& ptr)
{
	auto sleepDuration = std::rand() % 5;
	std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
	ptr = static_cast<char*>(stackAllocator.Allocate(allocationSize));
}
void ThreadFunction2(char *& ptr)
{
	auto sleepDuration = std::rand() % 5;
	std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
	ptr = new char[allocationSize];
}

TEST_CASE("STACK_MultithreadAllocation", "[StackAllocator]")
{
	const int numThreads = 500;
#ifdef STACK_ALLOCATOR
	StackAllocator stackAllocator(1'000'000);
	auto marker = stackAllocator.GetMarker();
#endif
	char* ptrs[numThreads];
	std::vector<std::thread> threads;

	for (int i = 0; i < numThreads; i++) {
#ifdef STACK_ALLOCATOR
		threads.emplace_back(std::thread(ThreadFunction, std::ref(stackAllocator), std::ref(ptrs[i])));
#else
		threads.emplace_back(std::thread(ThreadFunction2, std::ref(ptrs[i])));
#endif
	}

	for (int i = 0; i < numThreads; i++) {
		threads[i].join();
	}

	// Try writing to the allocated memory.
	for (unsigned int i = 0; i < numThreads; i++) {
		memset(ptrs[i], 123, allocationSize);
	}

	std::wstring s;

#ifdef STACK_ALLOCATOR	
	REQUIRE(stackAllocator.GetMarker() == numThreads * allocationSize);

	stackAllocator.FreeToMarker(marker);

	REQUIRE(stackAllocator.GetMarker() == marker);
#else
	for (int i = 0; i < numThreads; i++) {
		delete ptrs[i];
	}
#endif
}

TEST_CASE("STACK_AllocationException", "[StackAllocator]")
{
	StackAllocator stackAllocator(1e6);

	// Try allocating more memory than exists in the allocator.
	bool exceptionCaught = false;
	try
	{
		auto block = stackAllocator.Allocate(10e7);
	}
	catch (...)
	{
		exceptionCaught = true;
	}

	REQUIRE(exceptionCaught);
}

TEST_CASE("STACK_Free", "[StackAllocator]")
{
	unsigned int allocatedSpace = 1e6;
	StackAllocator stackAllocator(allocatedSpace);

	void* block1 = stackAllocator.Allocate(2000);
	//StackAllocator::Marker marker = stackAllocator.GetMarker();
	auto marker = stackAllocator.GetMarker();

	auto block2 = stackAllocator.Allocate(5000);
	auto block3 = stackAllocator.Allocate(10000);


	auto spaceBeforeFree = stackAllocator.GetAvailableSpace();

	auto expected = allocatedSpace - (2000 + 5000 + 10000);
	std::wstring s = L"spaceBeforeFree was " + std::to_wstring(spaceBeforeFree) + L" expected " + std::to_wstring(expected);
	REQUIRE(spaceBeforeFree == expected);

	stackAllocator.FreeToMarker(marker);

	auto spaceAfterFree = stackAllocator.GetAvailableSpace();
	expected = allocatedSpace - 2000;
	s = L"spaceAfterFree was " + std::to_wstring(spaceAfterFree) + L" expected " + std::to_wstring(expected);
	REQUIRE(spaceAfterFree == expected);
}

// Takes REEEELY long time to run.
/*
void AssertMemSet(void* mem, char val, size_t numBytes)
{
	int iteration = 0;

	for (char* ch = static_cast<char*>(mem); ch < ch + numBytes; ch++) {
		//std::wstring s = L"ch pointed at " + std::to_wstring((uintptr_t)ch) + L", *ch was " + std::to_wstring(*ch) + L" val was " + std::to_wstring(val) + L" iteration " + std::to_wstring(iteration);
		REQUIRE(*ch == val);
		iteration++;
	}
}

TEST_CASE("RepeatedFree", "[StackAllocator]")
{
	unsigned int allocatedSpace = 1'000'000;
	StackAllocator stackAllocator(allocatedSpace);

	auto marker = stackAllocator.GetMarker();

	for (int i = 0; i < 5; i++) {
		size_t blockSize = 100'000;
		void* block = stackAllocator.Allocate(blockSize);
		memset(block, 123 + i, blockSize);
		AssertMemSet(block, 123 + i, blockSize);

		stackAllocator.FreeToMarker(marker);
	}
}
*/

void StackAllocateBlocks(int numBlocks, int blockSize)
{
	StackAllocator stackAllocator(numBlocks * blockSize); 
	//StackAllocator stackAllocator(1e8); // 100 mille, slow!
	//StackAllocator stackAllocator(100'000'000); // also slow!

	for (int i = 0; i < numBlocks; i++)
	{	
		//int blockSize = (1 + std::rand() % 5) * minBlockSize;
		void* block = stackAllocator.Allocate(blockSize);

		memset(block, 123, blockSize);
	}
}

void MallocAllocateBlocks(int numBlocks, int blockSize)
{
	for (int i = 0; i < numBlocks; i++)
	{
		//int blockSize = (1 + std::rand() % 5) * minBlockSize;
		void* block = malloc(sizeof(char) * blockSize);
		memset(block, 123, blockSize);

		free(block);
	}
}

TEST_CASE("STACK_MixedSizeAllocationComparison", "[StackAllocator]")
{
	StackAllocateBlocks(5000, 1000);
}

TEST_CASE("STACK_SmallAllocationComparison", "[StackAllocator]")
{
	StackAllocateBlocks(5000, 5);
}

TEST_CASE("MALLOC_MixedSizeAllocationComparison", "[StackAllocator]")
{
	MallocAllocateBlocks(5000, 1000);
}

TEST_CASE("MALLOC_SmallAllocationComparison", "[StackAllocator]")
{
	MallocAllocateBlocks(5000, 5);
}