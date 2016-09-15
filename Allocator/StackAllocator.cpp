#include "StackAllocator.h"

void* operator new (unsigned long size, StackAllocator& stackAllocator)
{
    return stackAllocator.Allocate(size);
}