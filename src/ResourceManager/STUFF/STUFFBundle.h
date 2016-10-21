#ifndef STUFFBundle_h__
#define STUFFBundle_h__

#include <fstream>
#include <cstring>
#include <unordered_map>

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
		m_File.open(path.c_str(), std::ios::binary);

		// Read header
		STUFF::Header header;
		m_File.read(header.Signature, sizeof(header.Signature));
		if (std::strcmp(header.Signature, "STUFF") != 0) {
			throw std::runtime_error("Bundle is not STUFF");
		}
		m_File.read((char*)&header.Version, sizeof(header.Version));
		m_File.read((char*)&header.NumEntries, sizeof(header.NumEntries));
		if (header.NumEntries == 0) {
			LOG_WARNING("No entries in bundle!");
		}
		LOG_DEBUG("Bundle contains %o entries", header.NumEntries);

		// Read m_File.entries
		std::vector<STUFF::Entry> entries;
		for (int i = 0; i < header.NumEntries; ++i) {
			if (m_File.eof()) {
				LOG_WARNING("File entry list ended prematurely!");
				break;
			}

			STUFF::Entry entry;
			m_File.read((char*)&entry.PathLength, sizeof(entry.PathLength));
			if (entry.PathLength == 0) {
				LOG_WARNING("Encountered an empty m_File.entry path!");
			}
			entry.FilePath = new char[entry.PathLength];
			m_File.read(entry.FilePath, entry.PathLength);
			m_File.read((char*)&entry.Offset, sizeof(entry.Offset));
			m_File.read((char*)&entry.Size, sizeof(entry.Size));

			entries.push_back(entry);
			LOG_DEBUG("Entry %i: %s (%o, %o)", i + 1, entry.FilePath, entry.Offset, entry.Size);
		}

		if (entries.size() == 0) {
			LOG_WARNING("No file entries found!");
		}

		// Construct lookup table
        for (auto& e : entries) {
			m_FileEntries[e.FilePath] = new Block(e.FilePath, e.Offset, e.Size);
		}
	}

	~STUFFBundle()
	{
		m_File.close();
	}

	std::shared_ptr<Block> Search(const std::string& path) override
	{
		auto it = m_FileEntries.find(path);
		if (it == m_FileEntries.end()) {
			return nullptr;
		} else {
			return it->second;
		}
	}

private:
	std::ifstream m_File;
	std::unordered_map<std::string, Block*> m_FileEntries;
};

#endif
