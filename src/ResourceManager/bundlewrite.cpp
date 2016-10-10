#include "STUFF/STUFF.h"
#include <cstdio>
#include <cstring>
#include <iostream>

int main()
{
	FILE* file = fopen("bundle.stuff", "w");
	
	std::size_t offset = 0;

	STUFF::Header header; offset += sizeof(header);
	header.NumEntries = 1;

	const char* thing = "Hello I'm a thing";
	STUFF::Entry entry; offset += sizeof(entry) - sizeof(entry.FilePath);
	entry.FilePath = "things/thing.txt"; offset += strlen(entry.FilePath);
	entry.PathLength = strlen(entry.FilePath);
	entry.Offset = offset;
	entry.Size = strlen(thing) + 1;

	fwrite(&header.Signature, sizeof(header.Signature), 1, file);
	fwrite(&header.Version, sizeof(header.Version), 1, file);
	fwrite(&header.NumEntries, sizeof(header.NumEntries), 1, file);

	fwrite(&entry.PathLength, sizeof(entry.PathLength), 1, file);
	fwrite(entry.FilePath, strlen(entry.FilePath), 1, file);
	fwrite(&entry.Offset, sizeof(entry.Offset), 1, file);
	fwrite(&entry.Size, sizeof(entry.Size), 1, file);

	fwrite(thing, strlen(thing) + 1, 1, file);

	fclose(file);

	std::cout << "Offset: " << entry.Offset << std::endl;
	std::cout << "Size: " << entry.Size << std::endl;

	return 0;
}
