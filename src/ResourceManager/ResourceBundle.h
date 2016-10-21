#ifndef ResourceBundle_h__
#define ResourceBundle_h__

#include <string>
#include <memory>
#include <boost/interprocess/file_mapping.hpp>

class ResourceBundle
{
public:
	class Block
	{
	public:
		Block(boost::interprocess::file_mapping&& fileMapping, const std::string& path, std::uintptr_t offset, std::size_t blockSize)
            : m_FileMapping(fileMapping)
            , m_Path(path)
            , m_Offset(offset)
            , m_BlockSize(blockSize)
		{ }

		std::size_t BlockSize() const { return m_BlockSize; }

		// Reads the whole block from disk, returns number of bytes read
		std::size_t Read(void* destination);
		// Streams part of a block from disk, returns number of bytes read
		std::size_t Stream(void* destination, std::size_t size);

    private:
        boost::interprocess::file_mapping m_FileMapping;
		std::string m_Path;
		std::uintptr_t m_Offset;
		std::size_t m_BlockSize;
	};

	ResourceBundle(const std::string& path)
		: m_BundlePath(path)
	{ }

	virtual std::shared_ptr<Block> Search(const std::string& path) = 0;

protected:
	std::string m_BundlePath;
};

#endif
