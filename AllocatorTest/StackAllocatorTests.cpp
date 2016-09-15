#include <thread>
#include <vector>
#include <cstdlib>

#include <gtest/gtest.h>

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

TEST(StackAllocator, MultithreadAllocation)
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
    auto expected = numThreads * allocationSize;
    ASSERT_TRUE(stackAllocator.GetMarker() == expected) << "HEJ";

    stackAllocator.FreeToMarker(marker);

    ASSERT_TRUE(stackAllocator.GetMarker() == marker);
#else
    for (int i = 0; i < numThreads; i++) {
        delete ptrs[i];
    }
#endif
}

TEST(StackAllocator, AllocationException)
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

    ASSERT_TRUE(exceptionCaught);
}

TEST(StackAllocator, Free)
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
    ASSERT_TRUE(spaceBeforeFree == expected) << s;

    stackAllocator.FreeToMarker(marker);

    auto spaceAfterFree = stackAllocator.GetAvailableSpace();
    expected = allocatedSpace - 2000;
    s = L"spaceAfterFree was " + std::to_wstring(spaceAfterFree) + L" expected " + std::to_wstring(expected);
    ASSERT_TRUE(spaceAfterFree == expected) << s;
}

void AssertMemSet(void* mem, char val, size_t numBytes)
{
    int iteration = 0;

    for (char* ch = static_cast<char*>(mem); ch < ch + numBytes; ch++) {
        std::wstring s = L"ch pointed at " + std::to_wstring((uintptr_t)ch) + L", *ch was " + std::to_wstring(*ch) + L" val was " + std::to_wstring(val) + L" iteration " + std::to_wstring(iteration);
        ASSERT_TRUE(*ch == val) << s;
        iteration++;
    }
}

TEST(StackAllocator, RepeatedFree)
{
    unsigned int allocatedSpace = 1'000'000;
    StackAllocator stackAllocator(allocatedSpace);

    auto marker = stackAllocator.GetMarker();

    for(int i = 0; i < 5; i++) {
        size_t blockSize = 100'000;
        void* block = stackAllocator.Allocate(blockSize);
        memset(block, 123 + i, blockSize);
        AssertMemSet(block, 123 + i, blockSize);

        stackAllocator.FreeToMarker(marker);
    }
}

TEST(StackAllocator, MixedSizeAllocationComparison)
{
#ifdef STACK_ALLOCATOR
    unsigned int allocatedSpace = 1e8;
    StackAllocator stackAllocator(allocatedSpace);
#endif
    const int numBlocks = 5000;

    for (int i = 0; i < numBlocks; i++) {
        // Allocate blocks between 1000 and 5000 byte large.
        int blockSize = (1 + std::rand() % 5) * 1000;
#ifdef STACK_ALLOCATOR
        void* block = stackAllocator.Allocate(blockSize);
#else
        void* block = new char[blockSize];
#endif
        memset(block, 123, blockSize);
    }
}