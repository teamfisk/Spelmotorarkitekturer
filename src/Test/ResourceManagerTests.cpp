#include "catch.hpp"
#include "ResourceManager/ResourceManager.h"

TEST_CASE("ResourceManager")
{
	{
		const ResourceHandle<Resource>& r1_1 = ResourceManager::Load<Resource>("Resource1", 0);
		const ResourceHandle<Resource>& r1_2 = ResourceManager::Load<Resource>("Resource1", 0);

		REQUIRE(*r1_1 == *r1_2);

		const ResourceHandle<Resource>& r2 = ResourceManager::Load<Resource>("Resource2", 0);

		REQUIRE(*r1_1 != *r2);
		REQUIRE(*r1_2 != *r2);
	}

	ResourceManager::Collect();

	const ResourceHandle<Resource>& r1 = ResourceManager::Load<Resource>("Resource1", 0);
    REQUIRE(r1.Valid());
	ResourceManager::ForceFree(1);
	REQUIRE(!r1.Valid());
}