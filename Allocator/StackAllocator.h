#pragma once
#include <malloc.h>
#include <mutex>

class StackAllocator	
{
	// A Marker marks the boundary between two stack allocations.
	// It should be created from .GetMarker() and used with .FreeToMarker(Marker m_Marker);
	class Marker
	{
		friend class StackAllocator;

		std::size_t m_Marker;

		void operator = (std::size_t x)
		{
			m_Marker = x;
		}

	public:
		operator std::size_t()
		{
			return m_Marker;
		}

		bool operator == (Marker m)
		{
			return m_Marker == m.m_Marker;
		}
	};

public:	
	StackAllocator(std::size_t n)
	{				
		m_Memory = malloc(n);
		if (m_Memory == nullptr) {
			throw std::runtime_error("StackAllocator unable to Allocate memory.");
		}
		m_Size = n;
		m_StackTop = 0;
	}

	~StackAllocator() 
	{
		Clear();
		free(m_Memory);
		m_Memory = nullptr;
	}

	template <typename T>
	T* Allocate()
	{
		return static_cast<T*>(Allocate(sizeof(T)));
	}

	void* Allocate(std::size_t n)
	{	
		std::lock_guard<std::mutex> lockGuard(m_StackTopLock);
		
		std::size_t nextTop = m_StackTop + n;
		if (nextTop > m_Size) {
			throw std::overflow_error("Stop trying to Allocate more memory than exists!! :(");
		}
		
		void* ptr = (void*)(m_StackTop + (std::size_t)m_Memory);

		m_StackTop = nextTop;

		return ptr;
	}

	// Frees all the memory above this m_Marker.
	void FreeToMarker(Marker marker) 
	{
		m_StackTopLock.lock();
		m_StackTop = marker;
		m_StackTopLock.unlock();
	}

	// Returns a m_Marker to the current stack top.
	Marker GetMarker()
	{
		std::lock_guard<std::mutex> lock(m_StackTopLock);
		return m_StackTop;
	}

	std::size_t GetAvailableSpace()
	{
		std::lock_guard<std::mutex> lockGuard(m_StackTopLock);
		return m_Size - m_StackTop;
	}

	// Clears the entire stack
	void Clear()
	{
		m_StackTopLock.lock();
		m_StackTop = 0;
		m_StackTopLock.unlock();
	}

private:	
	void* m_Memory = nullptr;
	std::size_t m_Size = 0;

	// The current mem address offset to the top portion of the stack.
	Marker m_StackTop;

	std::mutex m_StackTopLock;
};

//void* operator new (std::size_t size, StackAllocator& stackAllocator);