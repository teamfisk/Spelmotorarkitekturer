#include "catch.hpp"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/ResourceTreeNode.h"
#include "ResourceManager/STUFF/STUFFBundle.h"

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
}

TEST_CASE("ResourceTree")
{
    ResourceTreeNode root;

	ResourceTreeNode n1;
	n1.Name = "Folder";
	ResourceTreeNode n2;
	n2.Name = "File";

	root.Insert("", &n1);
	root.Insert("Folder", &n2);

	auto file = root.Search("Folder/File");
	REQUIRE(file->Name == n2.Name);
}

TEST_CASE("STUFF_Read")
{
    STUFFBundle bundle("TestFiles/STUFF_Read/STUFF_Read.stuff");

    auto block = bundle.Search("Resources/TestFolder/TestFile2.txt");
    REQUIRE(block != nullptr);

    REQUIRE(block->BlockSize() == 12);

    auto buffer = new char[block->BlockSize()];
    std::size_t size = block->Read(buffer);
    REQUIRE(size == 12);

    REQUIRE(std::memcmp(buffer, "TestContent2", 12) == 0);
}