#pragma once
#include <malloc.h>

class StackAllocator	
{
public:	
	struct MemBlock
	{
		void* blockAddress;
		unsigned int blockSize;
	};

	// A Marker marks the boundry between two stack allocations.
	typedef unsigned int Marker;

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

	MemBlock allocate(unsigned int n)
	{
		unsigned int nextTop = stackTop + n;
		if (nextTop > memSize)
		{			
			throw "Stop trying to allocate more memory than exists!! :(";
		}

		MemBlock block;
		block.blockAddress = (void*)stackTop;
		block.blockSize = n;

		stackTop = nextTop;

		return block;
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