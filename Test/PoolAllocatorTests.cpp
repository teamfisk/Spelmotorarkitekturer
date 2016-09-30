#include <thread>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <cstring>

#include "catch.hpp"
#include "../Allocator/PoolAllocator.h"

//#define PRINT_OUT

template <int SIZE>
struct dataStruct {
	char data[SIZE];
};

class Helper {

public:
	// Fragment the pools.
	// Iterations are how many time we will free/allocate data in the pool.
	// Every iteration some objects are freed then allocated.
	// chanceInPercent is how many percent of blocks will be freed each iteration.
	// The number of allocated will be totalblocks * chanceInPercent * 0.75.
	// This will give us an equilibrium at a 75% full memory.
	template<typename T>
	static void FragmentPool(int iterations, int chanceInPercent, PoolAllocator<T>& mem) {
		srand(1337);
#ifdef PRINT_OUT
		int PreFree, PostFree, PostAlloc;
#endif // PRINT_OUT
		for (int n = 0; n < iterations; n++) {
#ifdef PRINT_OUT
			PreFree = mem.OccupiedBlocks();
#endif // PRINT_OUT

			//Loop through our mem and free some objects.
			for (typename PoolAllocator<T>::iterator it = mem.begin(); it != mem.end(); ++it) {
				if (rand() % 100 <= chanceInPercent) {
					mem.Free(&(*it));
				}
			}
#ifdef PRINT_OUT
			PostFree = mem.OccupiedBlocks();
#endif // PRINT_OUT

#ifdef PRINT_OUT
			PostAlloc = mem.OccupiedBlocks();
			std::cout << "Free'd " << PreFree - PostFree << " Objects and allocated " << PostAlloc - PostFree << " Objects." << std::endl << "New size is: " << PostAlloc << std::endl;
#endif // PRINT_OUT

		}

	}

	template<typename T>
	static void FragmentMalloc(int iterations, int chanceInPercent, std::vector<T*>& mem, int maxElements)
	{
		srand(1337);

		for (int i = 0; i < maxElements; i++) {
			if (rand() % 100 <= chanceInPercent) {
				delete mem[i];
				mem[i] = nullptr;
			}
		}
	}
};

// Test a simple case of allocating memory.
TEST_CASE("POOL_Allocate", "[Pool][Func]")
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
TEST_CASE("POOL_Free", "[Pool][Func]")
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
TEST_CASE("POOL_ContinuousAllocationAndFree", "[Pool][Func]")
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
TEST_CASE("POOL_AvailableSpace", "[Pool][Func]")
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

typedef dataStruct<8> dataTypeSmall;
typedef dataStruct<64> dataTypeMed;
typedef dataStruct<1024> dataTypeLarge;

const int blockAmountSmall = 1'000;
const int blockAmountLarge = 5'000;

//Allocate and fill memory for the tests.
std::vector<dataTypeSmall*> sMem_Small_Small(blockAmountSmall);
std::vector<dataTypeSmall*> sMem_Small_Large(blockAmountLarge);
std::vector<dataTypeMed*> sMem_Med_Small(blockAmountSmall);
std::vector<dataTypeMed*> sMem_Med_Large(blockAmountLarge);
std::vector<dataTypeLarge*> sMem_Large_Small(blockAmountSmall);
std::vector<dataTypeLarge*> sMem_Large_Large(blockAmountLarge);

PoolAllocator<dataTypeSmall> pMem_Small_Small(blockAmountSmall);
PoolAllocator<dataTypeSmall> pMem_Small_Large(blockAmountLarge);
PoolAllocator<dataTypeMed> pMem_Med_Small(blockAmountSmall);
PoolAllocator<dataTypeMed> pMem_Med_Large(blockAmountLarge);
PoolAllocator<dataTypeLarge> pMem_Large_Small(blockAmountSmall);
PoolAllocator<dataTypeLarge> pMem_Large_Large(blockAmountLarge);

TEST_CASE("Pool- Linear allocate", "[Pool][Perf][SetupAlloc][Usefull]")
{
	SECTION("SS") {
		for (int i = 0; i < blockAmountSmall; i++) {
			pMem_Small_Small.Allocate();
		}
	}
	SECTION("SL") {
		for (int i = 0; i < blockAmountLarge; i++) {
			pMem_Small_Large.Allocate();
		}
	}
	SECTION("MS") {
		for (int i = 0; i < blockAmountSmall; i++) {
			pMem_Med_Small.Allocate();
		}
	}
	SECTION("ML") {
		for (int i = 0; i < blockAmountLarge; i++) {
			pMem_Med_Large.Allocate();
		}
	}
	SECTION("LS") {
		for (int i = 0; i < blockAmountSmall; i++) {
			pMem_Large_Small.Allocate();
		}
	}
	SECTION("LL") {
		for (int i = 0; i < blockAmountLarge; i++) {
			pMem_Large_Large.Allocate();
		}
	}
}
TEST_CASE("Stand- Linear allocate", "[Stand][Perf][SetupAlloc][Usefull]")
{
	SECTION("SS") {
		dataTypeSmall** frag = new dataTypeSmall*[blockAmountSmall];

		//Allocate some new objects
		for (int i = 0; i < blockAmountSmall; ++i) {
			sMem_Small_Small[i] = new dataTypeSmall();
			memset(sMem_Small_Small[i]->data, rand() % 255, sizeof(dataTypeSmall));
			frag[i] = new dataTypeSmall();
		}

		for (int i = 0; i < blockAmountSmall; ++i) {
			delete frag[i];
		}
		delete[] frag;
	}
	SECTION("SL") {
		dataTypeSmall** frag = new dataTypeSmall*[blockAmountLarge];

		//Allocate some new objects
		for (int i = 0; i < blockAmountLarge; ++i) {
			sMem_Small_Large[i] = new dataTypeSmall();
			memset(sMem_Small_Large[i]->data, rand() % 255, sizeof(dataTypeSmall));
			frag[i] = new dataTypeSmall();
		}

		for (int i = 0; i < blockAmountLarge; ++i) {
			delete frag[i];
		}
		delete[] frag;
	}
	SECTION("MS") {
		dataTypeMed** frag = new dataTypeMed*[blockAmountSmall];

		//Allocate some new objects
		for (int i = 0; i < blockAmountSmall; ++i) {
			sMem_Med_Small[i] = new dataTypeMed();
			memset(sMem_Med_Small[i]->data, rand() % 255, sizeof(dataTypeMed));
			frag[i] = new dataTypeMed();
		}

		for (int i = 0; i < blockAmountSmall; ++i) {
			delete frag[i];
		}
		delete[] frag;
	}
	SECTION("ML") {
		dataTypeMed** frag = new dataTypeMed*[blockAmountLarge];

		//Allocate some new objects
		for (int i = 0; i < blockAmountLarge; ++i) {
			sMem_Med_Large[i] = new dataTypeMed();
			memset(sMem_Med_Large[i]->data, rand() % 255, sizeof(dataTypeMed));
			frag[i] = new dataTypeMed();
		}

		for (int i = 0; i < blockAmountLarge; ++i) {
			delete frag[i];
		}
		delete[] frag;
	}
	SECTION("LS") {
		dataTypeLarge** frag = new dataTypeLarge*[blockAmountSmall];

		//Allocate some new objects
		for (int i = 0; i < blockAmountSmall; ++i) {
			sMem_Large_Small[i] = new dataTypeLarge();
			memset(sMem_Large_Small[i]->data, rand() % 255, sizeof(dataTypeLarge));
			frag[i] = new dataTypeLarge();
		}

		for (int i = 0; i < blockAmountSmall; ++i) {
			delete frag[i];
		}
		delete[] frag;
	}
	SECTION("LL") {
		dataTypeLarge** frag = new dataTypeLarge*[blockAmountLarge];

		//Allocate some new objects
		for (int i = 0; i < blockAmountLarge; ++i) {
			sMem_Large_Large[i] = new dataTypeLarge();
			memset(sMem_Large_Large[i]->data, rand() % 255, sizeof(dataTypeLarge));
			frag[i] = new dataTypeLarge();
		}

		for (int i = 0; i < blockAmountLarge; ++i) {
			delete frag[i];
		}
		delete[] frag;
	}
}
dataTypeSmall ds;
dataTypeMed dm;
dataTypeLarge dl;
TEST_CASE("Pool- Linear access", "[Pool][Perf][Usefull]")
{
	SECTION("SS") {
		for (auto it = pMem_Small_Small.begin(); it != pMem_Small_Small.end(); it++) {
			memcpy(&ds.data[0], &it->data[0], sizeof(dataTypeSmall));
		}
	}
	SECTION("SL") {
		for (auto it = pMem_Small_Large.begin(); it != pMem_Small_Large.end(); it++) {
			memcpy(&ds.data[0], &it->data[0], sizeof(dataTypeSmall));
		}
	}
	SECTION("MS") {
		for (auto it = pMem_Med_Small.begin(); it != pMem_Med_Small.end(); it++) {
			memcpy(&dm.data[0], &it->data[0], sizeof(dataTypeMed));
		}
	}
	SECTION("ML") {
		for (auto it = pMem_Med_Large.begin(); it != pMem_Med_Large.end(); it++) {
			memcpy(&dm.data[0], &it->data[0], sizeof(dataTypeMed));
		}
	}
	SECTION("LS") {
		for (auto it = pMem_Large_Small.begin(); it != pMem_Large_Small.end(); it++) {
			memcpy(&dl.data[0], &it->data[0], sizeof(dataTypeLarge));
		}
	}
	SECTION("LL") {
		for (auto it = pMem_Large_Large.begin(); it != pMem_Large_Large.end(); it++) {
			memcpy(&dl.data[0], &it->data[0], sizeof(dataTypeLarge));
		}
	}
}
TEST_CASE("Stand- Linear access", "[Stand][Perf][Usefull]")
{
	SECTION("SS") {
		for (int i = 0; i < blockAmountSmall; ++i) {
			dataTypeSmall& it = *sMem_Small_Small[i];
			memcpy(&ds.data[0], &it.data[0], sizeof(dataTypeSmall));
		}
	}
	SECTION("SL") {
		for (int i = 0; i < blockAmountLarge; ++i) {
			dataTypeSmall& it = *sMem_Small_Large[i];
			memcpy(&ds.data[0], &it.data[0], sizeof(dataTypeSmall));
		}
	}
	SECTION("MS") {
		for (int i = 0; i < blockAmountSmall; ++i) {
			dataTypeMed& it = *sMem_Med_Small[i];
			memcpy(&dm.data[0], &it.data[0], sizeof(dataTypeMed));
		}
	}
	SECTION("ML") {
		for (int i = 0; i < blockAmountLarge; ++i) {
			dataTypeMed& it = *sMem_Med_Large[i];
			memcpy(&dm.data[0], &it.data[0], sizeof(dataTypeMed));
		}
	}
	SECTION("LS") {
		for (int i = 0; i < blockAmountSmall; ++i) {
			dataTypeLarge& it = *sMem_Large_Small[i];
			memcpy(&dl.data[0], &it.data[0], sizeof(dataTypeLarge));
		}
	}
	SECTION("LL") {
		for (int i = 0; i < blockAmountLarge; ++i) {
			dataTypeLarge& it = *sMem_Large_Large[i];
			memcpy(&dl.data[0], &it.data[0], sizeof(dataTypeLarge));
		}
	}
}
TEST_CASE("Pool- Fragment the memory", "[Pool][Perf][SetupFrag][Usefull]") {
	SECTION("SS") {
		Helper::FragmentPool(1, 22, pMem_Small_Small);
	}
	SECTION("SL") {
		Helper::FragmentPool(1, 22, pMem_Small_Large);
	}
	SECTION("MS") {
		Helper::FragmentPool(1, 22, pMem_Med_Small);
	}
	SECTION("ML") {
		Helper::FragmentPool(1, 22, pMem_Med_Large);
	}
	SECTION("LS") {
		Helper::FragmentPool(1, 22, pMem_Large_Small);
	}
	SECTION("LL") {
		Helper::FragmentPool(1, 22, pMem_Large_Large);
	}
}
TEST_CASE("Stand- Fragment the memory", "[Pool][Perf][SetupFrag][Usefull]")
{
	SECTION("SS") {
		Helper::FragmentMalloc(1, 22, sMem_Small_Small, blockAmountSmall);
	}
	SECTION("SL") {
		Helper::FragmentMalloc(1, 22, sMem_Small_Large, blockAmountLarge);
	}
	SECTION("MS") {
		Helper::FragmentMalloc(1, 22, sMem_Med_Small, blockAmountSmall);
	}
	SECTION("ML") {
		Helper::FragmentMalloc(1, 22, sMem_Med_Large, blockAmountLarge);
	}
	SECTION("LS") {
		Helper::FragmentMalloc(1, 22, sMem_Large_Small, blockAmountSmall);
	}
	SECTION("LL") {
		Helper::FragmentMalloc(1, 22, sMem_Large_Large, blockAmountLarge);
	}
}
TEST_CASE("Pool- Fragmented linear access", "[Pool][Perf][Usefull]")
{
	SECTION("SS") {
		for (auto& e : pMem_Small_Small) {
			if (&e != nullptr) {
				memcpy(&ds.data[0], &e.data[0], sizeof(dataTypeSmall));
			}
		}
	}
	SECTION("SL") {
		for (auto& e : pMem_Small_Large) {
			if (&e != nullptr) {
				memcpy(&ds.data[0], &e.data[0], sizeof(dataTypeSmall));
			}
		}
	}
	SECTION("MS") {
		for (auto& e : pMem_Med_Small) {
			if (&e != nullptr) {
				memcpy(&dm.data[0], &e.data[0], sizeof(dataTypeMed));
			}
		}
	}
	SECTION("ML") {
		for (auto& e : pMem_Med_Large) {
			if (&e != nullptr) {
				memcpy(&dm.data[0], &e.data[0], sizeof(dataTypeMed));
			}
		}
	}
	SECTION("LS") {
		for (auto& e : pMem_Large_Small) {
			if (&e != nullptr) {
				memcpy(&dl.data[0], &e.data[0], sizeof(dataTypeLarge));
			}
		}
	}
	SECTION("LL") {
		for (auto& e : pMem_Large_Large) {
			if (&e != nullptr) {
				memcpy(&dl.data[0], &e.data[0], sizeof(dataTypeLarge));
			}
		}
	}
}
TEST_CASE("Stand- Fragmented linear access", "[Stand][Perf][Usefull]")
{
	SECTION("SS") {
		for (auto& e : sMem_Small_Small) {
			if (e != nullptr) {
				memcpy(&ds.data[0], &e->data[0], sizeof(dataTypeSmall));
			}
		}
	}
	SECTION("SL") {
		for (auto& e : sMem_Small_Large) {
			if (e != nullptr) {
				memcpy(&ds.data[0], &e->data[0], sizeof(dataTypeSmall));
			}
		}
	}
	SECTION("MS") {
		for (auto& e : sMem_Med_Small) {
			if (e != nullptr) {
				memcpy(&dm.data[0], &e->data[0], sizeof(dataTypeMed));
			}
		}
	}
	SECTION("ML") {
		for (auto& e : sMem_Med_Large) {
			if (e != nullptr) {
				memcpy(&dm.data[0], &e->data[0], sizeof(dataTypeMed));
			}
		}
	}
	SECTION("LS") {
		for (auto& e : sMem_Large_Small) {
			if (e != nullptr) {
				memcpy(&dl.data[0], &e->data[0], sizeof(dataTypeLarge));
			}
		}
	}
	SECTION("LL") {
		for (auto& e : sMem_Large_Large) {
			if (e != nullptr) {
				memcpy(&dl.data[0], &e->data[0], sizeof(dataTypeLarge));
			}
		}
	}
}


//
//// Allocate 100 000 ints and read 1 000 random ones.
//// I'm printing every 100th int in the hope that no unwanted optimizing will occur.
//TEST_CASE("POOL_LargeDataRead", "[Pool][Perf]")
//{
//	unsigned int amount = 1'000'000;
//	PoolAllocator<int> pool(amount);
//	int* randElements = new int[1000];
//	int rE = 0;
//
//	for (int i = 0; i < amount; i++) {
//		if (rand() % 1'000 == 1 && rE < 1000) {
//			randElements[rE] = *pool.Allocate(i);
//			rE++;
//		}
//		else {
//			pool.Allocate(i);
//		}
//	}
//
//	//for (int i = 0; i < rE; i++) {
//	//	//std::cout << randElements[i] << ", ";
//	//}
//	//std::cout << std::endl;
//}
//
//// Allocate 100 000 ints and read 1 000 random ones.
//TEST_CASE("MALLOC_LargeDataRead", "[Stand][Perf]")
//{
//	unsigned int amount = 1'000'000;
//	int* arr = new int[amount];
//	int* randElements = new int[1000];
//	int rE = 0;
//
//	for (int i = 0; i < amount; i++) {
//		arr[i] = i;
//		if (rand() % 1'000 == 1 && rE < 1000) {
//			randElements[rE] = arr[i];
//			rE++;
//		}
//	}
//
//	//for (int i = 0; i < rE; i++) {
//	//	//std::cout << randElements[i] << ", ";
//	//}
//	//std::cout << std::endl;
//}



