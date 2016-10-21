#ifndef STUFFBundle_h__
#define STUFFBundle_h__

#include <fstream>
#include <cstring>

#include "../Logging.h"
#include "../ResourceBundle.h"
#include "ResourceManager/ResourceTreeNode.h"
#include "STUFF.h"

class STUFFBundle : ResourceBundle
{
public:
	STUFFBundle(const std::string& path)
		: ResourceBundle(path)
	{
		std::ifstream file(path.c_str(), std::ios::binary);

		// Read header
		STUFF::Header header;
		file.read(header.Signature, sizeof(header.Signature));
		if (std::strcmp(header.Signature, "STUFF") != 0) {
			throw std::runtime_error("Bundle is not STUFF");
		}
		file.read((char*)&header.Version, sizeof(header.Version));
		file.read((char*)&header.NumEntries, sizeof(header.NumEntries));
		if (header.NumEntries == 0) {
			LOG_WARNING("No entries in bundle!");
		}
		LOG_DEBUG("Bundle contains %o entries", header.NumEntries);

		// Read file entries
		std::vector<STUFF::Entry> entries;
		for (int i = 0; i < header.NumEntries; ++i) {
			if (file.eof()) {
				LOG_WARNING("File entry list ended prematurely!");
				break;
			}

			STUFF::Entry entry;
			file.read((char*)&entry.PathLength, sizeof(entry.PathLength));
			if (entry.PathLength == 0) {
				LOG_WARNING("Encountered an empty file entry path!");
			}
			entry.FilePath = new char[entry.PathLength];
			file.read(entry.FilePath, entry.PathLength);
			file.read((char*)&entry.Offset, sizeof(entry.Offset));
			file.read((char*)&entry.Size, sizeof(entry.Size));

			entries.push_back(entry);
			LOG_DEBUG("Entry %i: %s (%o, %o)", i + 1, entry.FilePath, entry.Offset, entry.Size);
		}

		if (entries.size() == 0) {
			LOG_WARNING("No file entries found!");
		}
	}

	std::size_t Read(const std::string& resourcePath, void* destination) { return 0; }

private:
	ResourceTree m_Tree;

	void ParseVersion_1(std::ifstream& file)
	{
	}
};

#endif
