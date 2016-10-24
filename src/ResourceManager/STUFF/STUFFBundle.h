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
//        m_File = bip::file_mapping(path.c_str(), bip::read_only);
//
//		void* data = nullptr;
//		std::size_t offset = 0;
//
//		// Read header
//		bip::mapped_region headerRegion(m_File, bip::read_only, offset, sizeof(STUFF::Header::Signature) + sizeof(STUFF::Header::Version) + sizeof(STUFF::Header::NumEntries));
//		offset += headerRegion.get_size();
//		data = headerRegion.get_address();
//		STUFF::Header header;
//		memcpy(header.Signature, data, sizeof(header.Signature));
//		data += sizeof(header.Signature);
//		if (std::strcmp(header.Signature, "STUFF") != 0) {
//			throw std::runtime_error("Bundle is not STUFF");
//		}
//		memcpy(&header.Version, data, sizeof(header.Version));
//		data += sizeof(header.Version);
//		memcpy(&header.NumEntries, data, sizeof(header.NumEntries));
//		data += sizeof(header.NumEntries);
//		if (header.NumEntries == 0) {
//			LOG_WARNING("No entries in bundle!");
//		}
//
//		// Read file entries
//		if (header.NumEntries > 0) {
//			for (int i = 0; i < header.NumEntries; ++i) {
//                bip::mapped_region entryRegion(m_File, bip::read_only, offset, sizeof(STUFF::Entry));
//                offset += entryRegion.get_size();
//                data = entryRegion.get_address();
//				// Read entry
//				STUFF::Entry entry;
//				memcpy(&entry.PathLength, data, sizeof(entry.PathLength));
//				data += sizeof(entry.PathLength);
//                if (entry.PathLength == 0) {
//                    LOG_WARNING("Encountered an empty m_File.entry path!");
//                }
//                entry.FilePath = new char[entry.PathLength + 1];
//                entry.FilePath[entry.PathLength] = '\0';
//				memcpy(entry.FilePath, data, entry.PathLength);
//				data += entry.PathLength;
//				memcpy(&entry.Offset, data, sizeof(entry.Offset));
//				data += sizeof(entry.Offset);
//				memcpy(&entry.Size, data, sizeof(entry.Size));
//				data += sizeof(entry.Size);
//				LOG_DEBUG("Entry %i: %s (%o, %o)", i + 1, entry.FilePath, entry.Offset, entry.Size);
//
//				// Construct block instance
//                std::size_t blockOffset = offset + entry.Offset;
//				//std::size_t blockOffset = entry.Offset; // HACK: Offsets should be relative to first block later
//                bip::mapped_region blockRegion(m_File, bip::read_only, blockOffset, entry.Size);
//				m_FileEntries[entry.FilePath] = new Block(std::move(blockRegion), entry.FilePath, entry.Size);
//			}
//		}
//
//		if (m_FileEntries.size() == 0) {
//			LOG_WARNING("No file entries found!");
//		}

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
                LOG_WARNING("Encountered an empty file.entry path!");
            }
            entry.FilePath = new char[entry.PathLength];
            file.read(entry.FilePath, entry.PathLength);
            file.read((char*)&entry.Offset, sizeof(entry.Offset));
            file.read((char*)&entry.Size, sizeof(entry.Size));

            LOG_DEBUG("Entry %i: %s (%o, %o)", i + 1, entry.FilePath, entry.Offset, entry.Size);
            entries.push_back(std::move(entry));
        }

        std::size_t firstBlockOffset = file.tellg();

        // Construct block instances
        m_FileMapping = bip::file_mapping(path.c_str(), bip::read_only);
        for (auto& e : entries) {
            std::size_t blockOffset = firstBlockOffset + e.Offset;
            bip::mapped_region blockRegion(m_FileMapping, bip::read_only, blockOffset, e.Size);
            m_FileEntries[e.FilePath] = new Block(std::move(blockRegion), e.FilePath, e.Size);
        }
	}

	~STUFFBundle()
	{
	}

	Block* Search(const std::string& path) override
	{
		auto it = m_FileEntries.find(path);
		if (it == m_FileEntries.end()) {
			return nullptr;
		} else {
			return it->second;
		}
	}

private:
	bip::file_mapping m_FileMapping;
	std::unordered_map<std::string, Block*> m_FileEntries;
};

#endif
