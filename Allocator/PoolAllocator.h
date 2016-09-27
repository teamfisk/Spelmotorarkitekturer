#ifndef PoolAllocator_h
#define PoolAllocator_h
#include <vector>
#include <mutex>

//Iterator class
//Is used by our Pool Allocator to iterate through the memory.
template<typename T>
class MemoryPoolIterator;

//Pool allocator class that handles our custom pool memory allocator.
template <typename T>
class PoolAllocator
{
	//Memory pool iterator that handle all iteration through our memory pool.
	//If you implement dynamic memory this will need to be changed to handle that.
	template <typename T>
	class MemoryPoolIterator
		: public std::iterator<std::forward_iterator_tag, T>
	{
	public:
		MemoryPoolIterator(const PoolAllocator<T>* pool, size_t pos)
			: m_Pool(pool)
			, m_Pos(pos)
		{ }
		//Operators override the custom operators and make the iterator function.

		//Iterate to the next non empty block.
		//Pre
		MemoryPoolIterator<T> operator++() {
			while (++m_Pos < m_Pool->m_TotalBlocks && !m_Pool->m_BlockOccupied[m_Pos]);
			return *this;
		}

		//Iterate to the next non empty block
		//Post
		MemoryPoolIterator<T> operator++(int) {
			auto cpyIt = MemoryPoolIterator(*this);
			while (++m_Pos < m_Pool->m_TotalBlocks && !m_Pool->m_BlockOccupied[m_Pos]);
			return cpyIt;
		}

		//Return a reference to the MemoryPoolIterator. 
		T& operator*() {
			if (!m_Pool->m_BlockOccupied[m_Pos]) {
				throw std::runtime_error("Iterator invalid");
			}
			return *this->operator->();
		}

		//Return a pointer to the block at m_Pos.
		T* operator->() {
			return reinterpret_cast<T*>(m_Pool->m_StartAdress + m_Pool->m_Stride * m_Pos);
		}

		bool operator==(const MemoryPoolIterator<T>& it) { return m_Pos >= it.m_Pos; }
		bool operator!=(const MemoryPoolIterator<T>& it) { return m_Pos != it.m_Pos; }
	private:
		const PoolAllocator<T>* m_Pool;
		std::size_t m_Pos;
	};

	template <typename T2>
	friend class MemoryPoolIterator;
public:
	typedef MemoryPoolIterator<T> iterator;
	//These Typedefs are needed for the iterator to work.
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;

	// Allocates memory with "totalBlocks" amount of blocks with a stride of "stride" bytes.
	PoolAllocator(std::size_t totalBlocks)
		: m_StartAdress(new char[totalBlocks * sizeof(T)])
		, m_BlockOccupied(totalBlocks, false)
		, m_Stride(sizeof(T))
		, m_TotalBlocks(totalBlocks)
		, m_FirstFreeBlock(0)
		, m_NumOccupiedBlocks(0)
		, m_FirstBlock(m_TotalBlocks)
	{}

	~PoolAllocator() {
		if (m_StartAdress != nullptr) {
			delete[] m_StartAdress;
			m_StartAdress = nullptr;
		}
	}

	//Allocate memory for one T object and return the memory address.
	template<typename... Arguments>
	T* Allocate(Arguments... args) {
        //std::lock_guard<std::mutex> lockGuard(m_MutexLock);
		for (; m_FirstFreeBlock < m_TotalBlocks && m_BlockOccupied[m_FirstFreeBlock]; ++m_FirstFreeBlock);
		if (m_FirstFreeBlock < m_TotalBlocks) {
			m_FirstBlock = m_FirstFreeBlock < m_FirstBlock ? m_FirstFreeBlock : m_FirstBlock;
			m_BlockOccupied[m_FirstFreeBlock] = true;
			m_NumOccupiedBlocks++;
			return new (m_StartAdress + m_Stride*m_FirstFreeBlock++) T(args...);
		}
		else
		{
			//TODO: Outside memory pool, deal with it if you want.
		}
	}

	// Free a block from our allocated memory.
	// If the first block in memory is freed we set the next block as our first.
	void Free(T* obj) {
		obj->~T();
        //m_MutexLock.lock();
		--m_NumOccupiedBlocks;
		const std::size_t freeSlot = (reinterpret_cast<char*>(obj) - m_StartAdress) / m_Stride;
		m_BlockOccupied[freeSlot] = false;
		m_FirstFreeBlock = freeSlot < m_FirstFreeBlock ? freeSlot : m_FirstFreeBlock;
		if (freeSlot == m_FirstBlock) {
			while (m_FirstBlock != m_TotalBlocks && !m_BlockOccupied[m_FirstBlock]) {
				m_FirstBlock++;
			}
		}
        //m_MutexLock.unlock();
	}

	iterator begin() const {
		return iterator(this, m_FirstBlock);
	}

	iterator end() const {
		return iterator(this, m_TotalBlocks);
	}

	iterator Empty() const {
		return m_FirstBlock == m_NumOccupiedBlocks;
	}

	// Return the size of the memory in bytes.
	size_t Size() const {
		return m_TotalBlocks * sizeof(T);
	}

	// Return how many blocks this memory has.
	// If you allocated memory for 50 ints, it will return 50, not 50*sizeof(int).
	size_t SizeInBlocks() const {
		return m_TotalBlocks;
	}

	//Return the amount of free memory in bytes.
	size_t SizeFree() const {
		return FreeBlocks() * sizeof(T);
	}

	//Return the amount of free blocks.
	size_t FreeBlocks() const {
		return m_TotalBlocks - m_NumOccupiedBlocks;
	}

	// Return how many blocks are occupied.
	// Each block is the size of the Type allocated in memory.
	size_t OccupiedBlocks() const{
		return m_NumOccupiedBlocks;
	}

private:
	char* m_StartAdress;
	std::vector<bool> m_BlockOccupied;
	std::size_t m_Stride;
	std::size_t m_TotalBlocks;
	std::size_t m_FirstFreeBlock;
	std::size_t m_NumOccupiedBlocks;
	std::size_t m_FirstBlock;
	std::mutex m_Mutex;
};
#endif