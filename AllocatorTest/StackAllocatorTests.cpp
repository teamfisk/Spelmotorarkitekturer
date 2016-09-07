#include "stdafx.h"
#include "CppUnitTest.h"
#include <thread>
#include <vector>
#include "../Allocator/StackAllocator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AllocatorTest
{	
	void ThreadFunction(StackAllocator & stackAllocator)
	{
		stackAllocator.allocate(1000);
	}

	TEST_CLASS(StackAllocatorTests)
	{
		TEST_METHOD(MultithreadAllocationTest)
		{							
			StackAllocator stackAllocator(1'000'000);
			auto marker = stackAllocator.getMarker();

			const int numThreads = 50;
			std::vector<std::thread> threads;

			for (int i = 0; i < numThreads; i++)
			{
				threads.emplace_back(std::thread(ThreadFunction, std::ref(stackAllocator)));
			}

			for (int i = 0; i < numThreads; i++)
			{
				threads[i].join();
			}

			Assert::IsTrue(stackAllocator.getMarker() == numThreads * 1000);

			stackAllocator.freeToMarker(marker);

			Assert::IsTrue(stackAllocator.getMarker() == marker);
		}
	};
}