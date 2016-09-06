#pragma once
#include <malloc.h>

class StackAllocator	
{
	// A Marker marks the boundry between two stack allocations.
	// It should be created from .getMarker() and used with .freeToMarker(Marker marker);
	class Marker
	{
		friend class StackAllocator;

		unsigned int marker;
		operator unsigned int()
		{
			return marker;
		}
		void operator =(unsigned int x)
		{
			marker = x;
		}
	};
public:	
	// The current mem adress of the top portion of the stack.
	Marker stackTop;

	StackAllocator(unsigned int n)
	{				
		mem = malloc(n);
		if (mem == nullptr)
		{						
			throw "StackAllocator unable to allocate memory.";
		}
		memSize = n;
	}

	~StackAllocator() 
	{
		clear();
	}

	void* allocate(unsigned int n)
	{
		unsigned int nextTop = stackTop + n;
		if (nextTop > memSize)
		{			
			throw "Stop trying to allocate more memory than exists!! :(";
		}
		
		void* ptr = (void*)(stackTop.marker + (unsigned int)mem);

		stackTop = nextTop;
		
		return ptr;
	}

	// Frees all the memory above this marker.
	void freeToMarker(Marker marker) 
	{
		stackTop = marker;
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

	void clear()
	{
		free(mem);
		mem = nullptr;
		stackTop = 0;
	}

private:	
	void* mem = nullptr;
	unsigned int memSize = 0;
};