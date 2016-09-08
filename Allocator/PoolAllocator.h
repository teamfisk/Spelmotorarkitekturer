#ifndef PoolAllocator_h
#define PoolAllocator_h
#include <vector>


template <typename T>
class PoolAllocator
{
public:

	//Allocates memory with "totalBlocks" amout of blocks with a stride of "stride" bytes.

	PoolAllocator(size_t totalBlocks)
		: m_StartAdress(new char[totalBlocks*sizeof(T)]) //TODO: Kolla upp om vi skall använda malloc ist för new
		, m_BlockOccupied(totalBlocks, false)
		, m_Stride(sizeof(T))
		, m_TotalBlocks(totalBlocks)
	{}

	~Poolallocator()
	{}
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

	template<typename T>
	void Free(T* obj)
	{
		obj->~T();
		--m_NumOccupiedBlocks;
		const size_t freeSlot = (reinterpret_cast<char*>(obj) - m_StartAdress) / m_Stride;
		m_BlockOccupied = false;
		if (freeSlot < m_FirstFreeBlock)
			m_FirstFreeBlock = freeSlot;

	}

private:
	char* m_StartAdress;
	std::vector<bool> m_BlockOccupied;
	size_t m_Stride;
	size_t m_TotalBlocks;
	size_t m_FirstFreeBlock;
	size_t m_NumOccupiedBlocks;
};

#endif