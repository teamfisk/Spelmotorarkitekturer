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

	// Marks the boundry between two stack allocations.
	typedef unsigned int StackMarker;

	// The current mem adress of the top portion of the stack.
	StackMarker stackTop;

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

	void deallocate(StackMarker marker) 
	{
		stackTop -= marker;
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