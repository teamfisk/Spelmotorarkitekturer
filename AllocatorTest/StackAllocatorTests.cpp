#include "stdafx.h"
#include "CppUnitTest.h"
#include <thread>
#include <vector>
#include "../Allocator/StackAllocator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Undef to switch to regular memory allocator
#define STACK_ALLOCATOR

namespace AllocatorTest
{	
	unsigned int allocationSize = 1000;

	void ThreadFunction(StackAllocator & stackAllocator, char *& ptr)
	{
		auto sleepDuration = rand() % 5;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
		ptr = static_cast<char*>(stackAllocator.allocate(allocationSize));
	}
	void ThreadFunction2(char *& ptr)
	{
		auto sleepDuration = rand() % 5;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
		ptr = new char[allocationSize];
	}

	TEST_CLASS(StackAllocatorTests)
	{
		TEST_METHOD(MultithreadAllocationTest)
		{						
			const int numThreads = 500;
#ifdef STACK_ALLOCATOR
			StackAllocator stackAllocator(1'000'000);
			auto marker = stackAllocator.getMarker();
#endif
			char* ptrs[numThreads];
			std::vector<std::thread> threads;
			
			for (int i = 0; i < numThreads; i++)
			{
#ifdef STACK_ALLOCATOR
				threads.emplace_back(std::thread(ThreadFunction, std::ref(stackAllocator), std::ref(ptrs[i])));
#else
				threads.emplace_back(std::thread(ThreadFunction2, std::ref(ptrs[i])));
#endif
			}

			for (int i = 0; i < numThreads; i++)
			{
				threads[i].join();
			}

			// Try writing to the allocated memory.
			for (unsigned int i = 0; i < numThreads; i++)
			{
				memset(ptrs[i], 123, allocationSize);				
			}

			std::wstring s;

#ifdef STACK_ALLOCATOR
			auto expected = numThreads * allocationSize;
			s = L"StackAllocator.getMarker() was " + std::to_wstring(stackAllocator.getMarker()) + L" expected " + std::to_wstring(expected);
			Assert::IsTrue(stackAllocator.getMarker() == expected, s.c_str());

			stackAllocator.freeToMarker(marker);

			s = L"StackAllocator.getMarker() was " + std::to_wstring(stackAllocator.getMarker()) + L" expected " + std::to_wstring(marker);
			Assert::IsTrue(stackAllocator.getMarker() == marker, s.c_str());			
#else			
			for (int i = 0; i < numThreads; i++)
			{
				delete ptrs[i];
			}
#endif 
		}

		TEST_METHOD(AllocationExceptionTest)
		{
			StackAllocator stackAllocator(1e6);

			// Try allocating more memory than exists in the allocator. 
			bool exceptionCaught = false;
			try
			{
				auto block = stackAllocator.allocate(10e7);
			}
			catch (...)
			{
				exceptionCaught = true;
			}

			Assert::IsTrue(exceptionCaught);
		}

		TEST_METHOD(FreeTest)
		{
			unsigned int allocatedSpace = 1e6;
			StackAllocator stackAllocator(allocatedSpace);

			void* block1 = stackAllocator.allocate(2000);
			//StackAllocator::Marker marker = stackAllocator.getMarker();
			auto marker = stackAllocator.getMarker();

			auto block2 = stackAllocator.allocate(5000); 
			auto block3 = stackAllocator.allocate(10000);

			
			auto spaceBeforeFree = stackAllocator.getAvailableSpace();

			auto expected = allocatedSpace - (2000 + 5000 + 10000);
			std::wstring s = L"spaceBeforeFree was " + std::to_wstring(spaceBeforeFree) + L" expected " + std::to_wstring(expected);
			Assert::IsTrue(spaceBeforeFree == expected, s.c_str());

			stackAllocator.freeToMarker(marker);

			auto spaceAfterFree = stackAllocator.getAvailableSpace();
			expected = allocatedSpace - 2000;
			s = L"spaceAfterFree was " + std::to_wstring(spaceAfterFree) + L" expected " + std::to_wstring(expected);
			Assert::IsTrue(spaceAfterFree == expected);
		}

		TEST_METHOD(MixedSizeAllocationComparison)
		{
#ifdef STACK_ALLOCATOR
			unsigned int allocatedSpace = 1e8;
			StackAllocator stackAllocator(allocatedSpace);
#endif						
			const int numBlocks = 5000;
			
			for (int i = 0; i < numBlocks; i++)
			{
				// Allocate blocks between 1000 and 5000 byte large.
				int blockSize = (1 + rand() % 5) * 1000;
#ifdef STACK_ALLOCATOR
				void* block = stackAllocator.allocate(blockSize);
#else
				void* block = new char[blockSize];
#endif
				memset(block, 123, blockSize);
			}						
		}
	};
}