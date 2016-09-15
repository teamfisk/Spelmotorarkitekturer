#include "stdafx.h"
#include <CppUnitTest.h>
#include <thread>
#include <vector>
#include "../Allocator/PoolAllocator.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//Undefine to use normal memory allocator
#define POOL_ALLOCATOR

namespace AllocatorTest
{

	//Class to test all the different use cases for our custom memory pool allocator.
	TEST_CLASS(PoolAllocatorTests)
	{
		//Test a simple case of allocating memory.
		TEST_METHOD(POOL_Allocation)
		{
			//Allocate memory with the size of 50 ints.
			PoolAllocator<int> pool(50);

			for (int i = 0; i < 50; i++) {
				pool.Allocate(i);
			}
			
		}

		//Test a simple case of freeing memory.
		TEST_METHOD(POOL_Free)
		{

		}

		//Test several continuous allocations and free's to see if the
		//memory stays intact without errors or diskfragmentation.
		TEST_METHOD(POOL_ContinuousAllocationAndFree)
		{

		}

		//Test using more memory than we have allocated and see that it is 
		//handled correctly.
		TEST_METHOD(POOL_Boundries)
		{

		}

		//Check the available space left in the memory pool
		//to make sure it's calculated correctly.
		TEST_METHOD(POOL_AvailableSpace)
		{

		}
	};
}




