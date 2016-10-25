#ifndef FilesystemBundle_h__
#define FilesystemBundle_h__

#include <fstream>
#include <boost/filesystem.hpp>
#include "ResourceBundle.h"

class FilesystemBlock : public ResourceBundle::Block
{
public:
    FilesystemBlock(const std::string& path, size_t size, std::ifstream&& file)
        : ResourceBundle::Block(path, size)
        , m_File(std::move(file))
    { }

    std::size_t Read(void* destination) override
    {
        m_File.seekg(std::ios::beg);
        m_File.read(reinterpret_cast<char*>(destination), m_Size);
        return m_File.gcount();
    }

    std::size_t Stream(void* destination, std::size_t size) override
    {
        m_File.read(reinterpret_cast<char*>(destination), size);
        return m_File.gcount();
    }

private:
    std::ifstream m_File;
};

class FilesystemBundle : public ResourceBundle
{
public:
    FilesystemBundle(const std::string& path)
        : ResourceBundle(path)
    { }

    std::shared_ptr<Block> Search(const std::string& path)
    {
        boost::filesystem::path fullPath = m_BundlePath;
        fullPath /= path;

        if (!boost::filesystem::exists(fullPath)) {
            return nullptr;
        }

        std::size_t size = boost::filesystem::file_size(fullPath);
        std::ifstream file(fullPath.string(), std::ios::binary);
        return std::make_shared<FilesystemBlock>(path, size, std::move(file));
    }
};

#endif