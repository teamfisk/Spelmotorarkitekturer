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
		auto sleepDuration = rand() % 5;
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
		stackAllocator.allocate(1000);		
	}

	TEST_CLASS(StackAllocatorTests)
	{
		TEST_METHOD(MultithreadAllocationTest)
		{							
			StackAllocator stackAllocator(1'000'000);
			auto marker = stackAllocator.getMarker();

			const int numThreads = 500;
			std::vector<std::thread> threads;

			for (int i = 0; i < numThreads; i++)
			{
				threads.emplace_back(std::thread(ThreadFunction, std::ref(stackAllocator)));
			}

			for (int i = 0; i < numThreads; i++)
			{
				threads[i].join();
			}

			std::wstring s;

			auto expected = numThreads * 1000;
			s = L"StackAllocator.getMarker() was " + std::to_wstring(stackAllocator.getMarker()) + L" expected " + std::to_wstring(expected);
			Assert::IsTrue(stackAllocator.getMarker() == expected, s.c_str());
			
			stackAllocator.freeToMarker(marker);

			s = L"StackAllocator.getMarker() was " + std::to_wstring(stackAllocator.getMarker()) + L" expected " + std::to_wstring(marker);
			Assert::IsTrue(stackAllocator.getMarker() == marker, s.c_str());			
		}
	};
}