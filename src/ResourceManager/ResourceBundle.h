#ifndef ResourceBundle_h__
#define ResourceBundle_h__

#include <string>
#include <memory>
#include <boost/interprocess/mapped_region.hpp>

class ResourceBundle
{
public:
	class Block
	{
	public:
		Block(boost::interprocess::mapped_region&& fileMapping, const std::string& path, std::size_t blockSize)
            : m_FileMapping(std::move(fileMapping))
            , m_Path(path)
            , m_BlockSize(blockSize)
		{ }

		std::size_t BlockSize() const { return m_BlockSize; }

		// Reads the whole block from disk, returns number of bytes read
		std::size_t Read(void* destination);
		// Streams part of a block from disk, returns number of bytes read
		std::size_t Stream(void* destination, std::size_t size);

    private:
        boost::interprocess::mapped_region m_FileMapping;
		std::string m_Path;
		std::size_t m_BlockSize;
		std::size_t m_StreamOffset = 0;
	};

	ResourceBundle(const std::string& path)
		: m_BundlePath(path)
	{ }

	virtual Block* Search(const std::string& path) = 0;

protected:
	std::string m_BundlePath;
};

#endif
