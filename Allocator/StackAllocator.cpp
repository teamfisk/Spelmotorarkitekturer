#include "StackAllocator.h"

void* operator new (std::size_t size, StackAllocator& stackAllocator)
{
    return stackAllocator.Allocate(size);
}

std::ostream& StackAllocator::Marker::operator <<(std::ostream& os) const
{
	os << m_Marker;
	return os;
}