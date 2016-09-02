#pragma once

template <typename T>
class StackAllocator	
{
public:
	typedef T value_type;
	
	StackAllocator()
	{
	}
	~StackAllocator() 
	{
	}

	T* allocate(std::size_t n)
	{
		return nullptr;
	}

	void deallocate(T* p, std::size_t n) 
	{
	
	}
};

template <class T, class U>
bool operator==(const StackAllocator<T>&, const StackAllocator<U>&)
{
	return true;
}

template <class T, class U>
bool operator!=(const StackAllocator<T>&, const StackAllocator<U>&)
{
	return true;
}