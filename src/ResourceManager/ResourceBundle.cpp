#include "ResourceBundle.h"

std::size_t ResourceBundle::Block::Read(void* destination)
{
    std::size_t size = std::min(m_BlockSize, m_FileMapping.get_size());
    memcpy(destination, m_FileMapping.get_address(), size);
    return size;
}

std::size_t ResourceBundle::Block::Stream(void* destination, std::size_t size)
{
    //size = std::min()
}
