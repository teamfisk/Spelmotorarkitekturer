#ifndef List_h__
#define List_h__

template <typename T>
class List
{
protected:
	List() { }

public:
	virtual int Size() = 0;
	virtual void AddFirst(T element) = 0;
	virtual void AddLast(T element) = 0;
	virtual T RemoveFirst() = 0;
	virtual T RemoveLast() = 0;
	virtual T ReturnNth(int n) = 0;
	virtual bool Empty() = 0;
	virtual void Clear() = 0;
	virtual void PrintList() = 0;
	virtual void Swap(int n1, int n2) = 0;
	virtual void InsertionSort(bool ascending = true) = 0;
	virtual void QuickSort(bool ascending = true) = 0;
};

#endif // List_h__