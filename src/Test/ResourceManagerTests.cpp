#include "catch.hpp"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/ResourceTreeNode.h"
#include "ResourceManager/STUFF/STUFFBundle.h"
#include "ResourceManager/FilesystemBundle.h"
#include "ResourceManager/TXT.h"

TEST_CASE("ResourceManager")
{
    ResourceManager::Initialize();
    ResourceManager::RegisterBundleFormat<STUFFBundle>();
	ResourceManager::RegisterBundleFormat<FilesystemBundle>();
	ResourceManager::RegisterBundle("TestFiles/ResourceBundle/Resources");

	auto txt = ResourceManager::Load<TXT>("TestFolder/TestFile1.txt");

	// If this loops forever there's something wrong in the async loading!
	while (!txt.Valid()) {
		ResourceManager::ProcessAsyncQueue();
	}

	REQUIRE(txt.Valid());
	REQUIRE((*txt)->Length() == 12);
	REQUIRE(std::memcmp((*txt)->Text(), "TestContent1", 12) == 0);
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

void testBundle(ResourceBundle* bundle)
{
	// Read()
	{
		auto block = bundle->Search("TestFolder/TestFile2.txt");
		REQUIRE(block != nullptr);
		REQUIRE(block->Size() == 12);

		auto buffer = new char[block->Size()];
		std::size_t bytesRead = block->Read(buffer);
		REQUIRE(bytesRead == 12);

		REQUIRE(std::memcmp(buffer, "TestContent2", 12) == 0);

		delete[] buffer;
	}

	// Stream()
	{
		auto block = bundle->Search("TestFolder/TestFile2.txt");
		REQUIRE(block != nullptr);
		REQUIRE(block->Size() == 12);

		char buffer[4];
		std::size_t bytesRead;

		bytesRead = block->Stream(buffer, 4);
		REQUIRE(bytesRead == 4);
		REQUIRE(std::memcmp(buffer, "Test", 4) == 0);
		bytesRead = block->Stream(buffer, 4);
		REQUIRE(bytesRead == 4);
		REQUIRE(std::memcmp(buffer, "Cont", 4) == 0);
		bytesRead = block->Stream(buffer, 4);
		REQUIRE(bytesRead == 4);
		REQUIRE(std::memcmp(buffer, "ent2", 4) == 0);
		bytesRead = block->Stream(buffer, 4);
		REQUIRE(bytesRead == 0);
	}

	// Invalid file
	{
		auto block = bundle->Search("TestFolder/QWERTY");
		REQUIRE(block == nullptr);
	}
}

TEST_CASE("STUFF_Read")
{
	auto bundle = new STUFFBundle("TestFiles/ResourceBundle/Resources.stuff");
	testBundle(bundle);
	delete bundle;
}

TEST_CASE("FilesystemBundle_Read")
{
    auto bundle = new FilesystemBundle("TestFiles/ResourceBundle/Resources");
	testBundle(bundle);
	delete bundle;
}