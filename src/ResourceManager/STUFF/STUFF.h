#ifndef STUFF_h__
#define STUFF_h__

#include <cstdint>

namespace STUFF
{

struct Header
{
	char Signature[5] = { 'S', 'T', 'U', 'F', 'F' };
	std::uint16_t Version = 1;
	std::uint16_t NumEntries = 0;
};

struct Entry
{
	std::uint16_t PathLength;
	char* FilePath;
	std::uint64_t Offset;
	std::uint64_t Size;
};

struct Block
{
	void* Data = nullptr;
};

}

#endif
