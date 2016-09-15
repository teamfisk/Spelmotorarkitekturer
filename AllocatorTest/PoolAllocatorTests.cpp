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
		TEST_METHOD(Allocation)
		{

		}

		//Test a simple case of freeing memory.
		TEST_METHOD(Free)
		{

		}

		//Test several continuous allocations and free's to see if the
		//memory stays intact without errors or diskfragmentation.
		TEST_METHOD(ContinuousAllocationAndFree)
		{

		}

		//Test using more memory than we have allocated.
		TEST_METHOD(Boundries)
		{

		}

		TEST_METHOD(AvailableSpace)
		{

		}
	}
}




