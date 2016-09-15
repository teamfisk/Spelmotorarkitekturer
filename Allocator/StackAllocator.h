#pragma once
#include <malloc.h>
#include <mutex>

class StackAllocator	
{
	// A Marker marks the boundry between two stack allocations.
	// It should be created from .getMarker() and used with .freeToMarker(Marker marker);
	class Marker
	{
		friend class StackAllocator;
		friend class Marker;

		uintptr_t marker;

		void operator =(unsigned int x)
		{
			marker = x;
		}

	public:
		operator uintptr_t()
		{
			return marker;
		}

		bool operator == (Marker m)
		{
			return marker == m.marker;
		}
	};
public:	
	StackAllocator(unsigned int n)
	{				
		mem = malloc(n);
		if (mem == nullptr)
		{						
			throw "StackAllocator unable to allocate memory.";
		}
		memSize = n;
		stackTop = 0;
	}

	~StackAllocator() 
	{
		clear();
		free(mem);
		mem = nullptr;
	}

	template <typename T>
	T* allocate()
	{
		return static_cast<T*>(allocate(sizeof(T)));		
	}

	void* allocate(uintptr_t n)
	{	
		std::lock_guard<std::mutex> lockGuard(stackTopLock);
		
		uintptr_t nextTop = stackTop + n;
		if (nextTop > memSize)
		{	
			throw "Stop trying to allocate more memory than exists!! :(";			
		}
		
		void* ptr = (void*)(stackTop + (uintptr_t)mem);

		stackTop = nextTop;

		return ptr;
	}

	// Frees all the memory above this marker.
	void freeToMarker(Marker marker) 
	{
		stackTopLock.lock();
		stackTop = marker;
		stackTopLock.unlock();
	}

	// Returns a marker to the current stack top.
	Marker getMarker()
	{
		std::lock_guard<std::mutex> lock(stackTopLock);		
		return stackTop;		
	}

	unsigned int getAvailableSpace()
	{
		std::lock_guard<std::mutex> lockGuard(stackTopLock);						
		return memSize - stackTop;
	}

	// Clears the entire stack
	void clear()
	{
		stackTopLock.lock();		
		stackTop = 0;
		stackTopLock.unlock();
	}

private:	
	void* mem = nullptr;
	uintptr_t memSize = 0;

	// The current mem adress of the top portion of the stack.
	Marker stackTop;

	std::mutex stackTopLock;
};

void* operator new (std::size_t size, StackAllocator& stackAllocator)
{
	return stackAllocator.allocate(size);
}