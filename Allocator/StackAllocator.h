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

		unsigned int marker;		

		void operator =(unsigned int x)
		{
			marker = x;
		}

	public:
		operator unsigned int()
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

	void* allocate(unsigned int n)
	{		
		unsigned int nextTop = stackTop + n;
		if (nextTop > memSize)
		{			
			throw "Stop trying to allocate more memory than exists!! :(";			
		}
		
		void* ptr = (void*)(stackTop.marker + (unsigned int)mem);

		stackTopLock.lock();
		stackTop = nextTop;
		stackTopLock.unlock();

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
		return stackTop;
	}

	unsigned int getAvailableSpace()
	{
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
	unsigned int memSize = 0;

	// The current mem adress of the top portion of the stack.
	Marker stackTop;

	std::mutex stackTopLock;
};