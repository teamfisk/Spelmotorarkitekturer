#include "ResourceBundle.h"

std::size_t ResourceBundle::Block::Read(void* destination)
{
    std::size_t size = m_BlockSize;
    memcpy(destination, m_FileMapping.get_address(), size);
    return size;
}

std::size_t ResourceBundle::Block::Stream(void* destination, std::size_t size)
{
    size = std::min(size, m_BlockSize - m_StreamOffset);
    if (size <= 0) {
        return 0;
    }
    memcpy(destination, (char*)m_FileMapping.get_address() + m_StreamOffset, size);
    m_StreamOffset += size;
    return size;
}
