#ifndef ResourceBundle_h__
#define ResourceBundle_h__

#include <string>
#include <memory>
#include <stdexcept>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/optional.hpp>
#include "Logging.h"

class ResourceBundle
{
public:
    class Block
	{
	public:
		Block(const std::string& path, std::size_t size)
            : m_Path(path)
            , m_Size(size)
		{ }

		const std::string& Path() const { return m_Path; }
		std::size_t Size() const { return m_Size; }

		// Reads the whole block from disk, returns number of bytes read
		virtual std::size_t Read(void* destination) = 0;
		// Streams a chunk of a block from disk, returns number of bytes read
		virtual std::size_t Stream(void* destination, std::size_t size) = 0;

	protected:
		const std::string m_Path;
		std::size_t m_Size;
	};

	class InvalidBundleFormat : public std::exception { };

	ResourceBundle(const std::string& path)
		: m_BundlePath(path)
	{ }

	virtual std::shared_ptr<Block> Search(const std::string& path) = 0;

protected:
	std::string m_BundlePath;
};

#endif
