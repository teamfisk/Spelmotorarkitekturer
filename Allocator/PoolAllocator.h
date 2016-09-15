#ifndef PoolAllocator_h
#define PoolAllocator_h
#include <vector>

template <typename T>
class PoolAllocator
{
public:

	//Allocates memory with "totalBlocks" amount of blocks with a stride of "stride" bytes.
	PoolAllocator(std::size_t totalBlocks)
		: m_StartAdress(new char[totalBlocks*sizeof(T)]) 
		, m_BlockOccupied(totalBlocks, false)
		, m_Stride(sizeof(T))
		, m_TotalBlocks(totalBlocks)
	{}

	~PoolAllocator()
	{
		delete[] m_StartAdress;
	}


	//Allocate memory for one T object and return the memory address.
	template<typename... Arguments>
	T* Allocate(Arguments... args)
	{
		for (; m_FirstFreeBlock < m_TotalBlocks && m_BlockOccupied[m_FirstFreeBlock]; ++m_FirstFreeBlock);
		if (m_FirstFreeBlock < m_TotalBlocks) {
			m_BlockOccupied[m_FirstFreeBlock] = true;
			m_NumOccupiedBlocks++;
			return new (m_StartAdress + m_Stride*m_FirstFreeBlock++) T(args...);
		}
		else
		{
			//TODO: Outside memory pool, deal with it.
		}
	}

	void Free(T* obj)
	{
		obj->~T();
		--m_NumOccupiedBlocks;
		const std::size_t freeSlot = (reinterpret_cast<char*>(obj) - m_StartAdress) / m_Stride;
		m_BlockOccupied[m_FirstFreeBlock] = false;
		if (freeSlot < m_FirstFreeBlock)
			m_FirstFreeBlock = freeSlot;

	}

private:
	char* m_StartAdress;
	std::vector<bool> m_BlockOccupied;
	std::size_t m_Stride;
	std::size_t m_TotalBlocks;
	std::size_t m_FirstFreeBlock;
	std::size_t m_NumOccupiedBlocks;
};

#endif