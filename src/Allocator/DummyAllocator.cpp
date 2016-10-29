#include "DummyAllocator.h"

void* DummyAllocator::Allocate(std::size_t size)
{
    if (m_Allocated + size > m_MaxSize) {
        throw std::bad_alloc();
    }
    void* ptr = std::malloc(size);
    m_Allocated += size;
    m_AllocationMap[ptr] = size;
    return ptr;
}

void DummyAllocator::Free(void* memory)
{
    std::free(memory);
    m_Allocated -= m_AllocationMap.at(memory);
    m_AllocationMap.erase(memory);
}

void* operator new(std::size_t size, DummyAllocator& allocator)
{
    return allocator.Allocate(size);
}

void* operator new[](std::size_t size, DummyAllocator& allocator)
{
    return allocator.Allocate(size);
}
