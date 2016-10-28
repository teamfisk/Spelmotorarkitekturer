#ifndef TXT_h__
#define TXT_h__

#include "Resource.h"

class TXT : public Resource
{
public:
    TXT(std::shared_ptr<ResourceBundle::Block> block)
    {
        m_Text = new char[block->Size()];
        block->Read(m_Text);
        m_Length = block->Size();
    }

    ~TXT()
    {
        if (m_Text != nullptr) {
            delete[] m_Text;
        }
    }

    const char* Text() const { return m_Text; }
    std::size_t Length() const { return m_Length; }

private:
	//TODO: Make sure that all memory used is logged.
	std::size_t Size() { return m_UsedMemory; }

    char* m_Text = nullptr;
    std::size_t m_Length = 0;
};

#endif

