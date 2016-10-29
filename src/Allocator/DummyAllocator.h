#include <cstdlib>
#include <unordered_map>

class DummyAllocator
{
public:
    DummyAllocator()
        : m_SizeRestricted(false)
    { }
    DummyAllocator(std::size_t maxSize)
        : m_SizeRestricted(true)
        , m_MaxSize(maxSize)
    { }

    void* Allocate(std::size_t size);
    void Free(void* memory);

    std::size_t IsSizeRestricted() const { return m_SizeRestricted; }
    std::size_t MaxSize() const { return m_MaxSize; }
    std::size_t Allocated() const { return m_Allocated; }

private:
    bool m_SizeRestricted;
    std::size_t m_MaxSize = 0;
    std::size_t m_Allocated = 0;
    std::unordered_map<void*, std::size_t> m_AllocationMap;
};

void* operator new(std::size_t size, DummyAllocator& stackAllocator);
void* operator new[](std::size_t size, DummyAllocator& stackAllocator);
