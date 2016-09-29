#define CATCH_CONFIG_MAIN
#include "catch.hpp"

// Put tests in other files, to avoid having to recompile catch every time!

/*
unsigned int Factorial(unsigned int number) {
	return number <= 1 ? number : Factorial(number - 1)*number;
}

TEST_CASE("Factorials are computed", "[factorial]") {
	REQUIRE(Factorial(1) == 1);
	REQUIRE(Factorial(2) == 2);
	REQUIRE(Factorial(3) == 6);
	REQUIRE(Factorial(10) == 3628800);
}

TEST_CASE("tasdfadsf")
{
	REQUIRE(1 == 2);
}

*/

// Note: when timing this code, the test case setup and each section is timed independently.
//TEST_CASE("vectors can be sized and resized", "[vector]") {
//
//	std::vector<int> v(5);         
//
//	REQUIRE(v.size() == 5);
//	REQUIRE(v.capacity() >= 5);
//
//	SECTION("resizing bigger changes size and capacity") {
//		v.resize(10);
//
//		REQUIRE(v.size() == 10);
//		REQUIRE(v.capacity() >= 10);
//	}
//	SECTION("resizing smaller changes size but not capacity") {
//		v.resize(0);
//
//		REQUIRE(v.size() == 0);
//		REQUIRE(v.capacity() >= 5);
//	}
//	SECTION("reserving bigger changes capacity but not size") {
//		v.reserve(10);
//
//		REQUIRE(v.size() == 5);
//		REQUIRE(v.capacity() >= 10);
//	}
//	SECTION("reserving smaller does not change size or capacity") {
//		v.reserve(0);
//
//		REQUIRE(v.size() == 5);
//		REQUIRE(v.capacity() >= 5);
//	}
//	SECTION("do nothing") // Takes 0 seconds to execute, since it does nothing, and the set up code execution time is not a part of this.
//	{
//
//	}
//}

// Note: this test case takes some time to run, even though it contains no code. The section takes 0s though.
//TEST_CASE("vectors can be sized and resized", "[vector]") 
//{	
//	SECTION("do someting")
//	{
//		
//	}
//}