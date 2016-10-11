#include "catch.hpp"
#include "ResourceManager/ResourceManager.h"

TEST_CASE("ResourceManager")
{
	Resource* p1;
	{
		ResourceHandle<Resource> r1_1 = ResourceManager::Load<Resource>("Resource1", 0);
		ResourceHandle<Resource> r1_2 = ResourceManager::Load<Resource>("Resource1", 0);

		REQUIRE(*r1_1 == *r1_2);

		ResourceHandle<Resource> r2 = ResourceManager::Load<Resource>("Resource2", 0);

		REQUIRE(*r1_1 != *r2);
		REQUIRE(*r1_2 != *r2);

		p1 = *r1_1;
	}

	ResourceManager::Collect();

	ResourceHandle<Resource> r1_1 = ResourceManager::Load<Resource>("Resource1", 0);
	REQUIRE(*r1_1 != p1);
}