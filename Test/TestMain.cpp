#define CATCH_CONFIG_MAIN
#include "catch.hpp"

// Put tests in other files, to avoid having to recompile catch every time!

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