#ifndef STUFFBundle_h__
#define STUFFBundle_h__

#include <fstream>
#include <cstring>
#include <unordered_map>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include "../Logging.h"
#include "../ResourceBundle.h"
#include "ResourceManager/ResourceTreeNode.h"
#include "STUFF.h"

namespace bip = boost::interprocess;

class STUFFBundle : ResourceBundle
{
public:
	STUFFBundle(const std::string& path)
		: ResourceBundle(path)
	{
        m_FileMapping = bip::file_mapping(path.c_str(), bip::read_only);
        std::ifstream file(path.c_str(), std::ios::binary);

        // Read header
        STUFF::Header header;
        file.read(header.Signature, sizeof(header.Signature));
        if (std::memcmp(header.Signature, "STUFF", 5) != 0) {
            LOG_WARNING("Unknown STUFF header: %s", std::string(header.Signature, 5).c_str());
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
                LOG_WARNING("Encountered an empty file.entry path!");
            }
            entry.FilePath = new char[entry.PathLength];
            file.read(entry.FilePath, entry.PathLength);
            file.read((char*)&entry.Offset, sizeof(entry.Offset));
            file.read((char*)&entry.Size, sizeof(entry.Size));

            LOG_DEBUG("Entry %i: %s (%o, %o)", i + 1, entry.FilePath, entry.Offset, entry.Size);
            m_FileEntries[entry.FilePath] = std::move(entry);
        }

        m_FirstBlockOffset = file.tellg();

	}

	~STUFFBundle()
	{
	}

	boost::optional<Block> Search(const std::string& path) override
	{
		auto it = m_FileEntries.find(path);
		if (it == m_FileEntries.end()) {
			return boost::none;
		} else {
            STUFF::Entry& e = it->second;
            std::size_t blockOffset = m_FirstBlockOffset + e.Offset;
            bip::mapped_region blockRegion(m_FileMapping, bip::read_only, blockOffset, e.Size);
            return Block(std::move(blockRegion), e.FilePath, e.Size);
		}
	}

private:
	bip::file_mapping m_FileMapping;
    std::size_t m_FirstBlockOffset = 0;
	std::unordered_map<std::string, STUFF::Entry> m_FileEntries;
};

#endif
