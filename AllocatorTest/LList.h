// Simon Holmberg 930302-4492

#ifndef LLIST_HPP
#define LLIST_HPP

#include <iostream>
#include <stdexcept>

#include "List.h"

template <typename T>
class LList : public List<T>
{
public:
	template <typename T>
	struct Item
	{
		Item(T data, Item<T>* prev, Item<T>* next = nullptr) : Data(data), Prev(prev), Next(next) {} 
		T Data;
		Item<T>* Next;
		Item<T>* Prev;
	};

	LList();
	~LList();
	int Size() override;
	void AddFirst(T element) override;
	void AddLast(T element) override;
	T RemoveFirst() override;
	T RemoveLast() override;
	T ReturnNth(int n) override;
	bool Empty() override;
	void Clear() override;
	void PrintList() override;
	void Swap(int n, int m) override;
	void InsertionSort(bool ascending = true) override;
	void QuickSort(bool ascending = true) override;
	
private:
	Item<T>* m_Head;
	Item<T>* m_Tail;
	int m_Size;
	
	Item<T>* ReturnNthItem(int n);
	void Swap(Item<T>* i1, Item<T>* i2);
	void QuickSort(bool ascending, int left, int right);
};

template <typename T>
void LList<T>::Clear()
{
	if (m_Head != nullptr)
	{
		Item<T>* item = m_Head;
		while (item->Next != nullptr)
		{
			Item<T>* next = item->Next;
			delete item;
			item = next;
		}
	}
	m_Head = nullptr;
	m_Tail = nullptr;
}

template <typename T>
void LList<T>::QuickSort(bool ascending /*= true*/)
{
	QuickSort(ascending, 0, m_Size - 1);
}

template <typename T>
void LList<T>::QuickSort(bool ascending, int left, int right)
{
	int pivot = (left + right) / 2;

	while (left <= right) {

	}
}

template <typename T>
void LList<T>::Swap(int n1, int n2)
{
	Swap(ReturnNthItem(n1), ReturnNthItem(n2));
}

template <typename T>
void LList<T>::Swap(Item<T>* i1, Item<T>* i2)
{
	// Store temp stuff
	Item<T>* i1P = i1->Prev;
	Item<T>* i1N = i1->Next;
	Item<T>* i2P = i2->Prev;
	Item<T>* i2N = i2->Next;

	// Swap
	i2->Prev = (i1P == i2) ? i1 : i1P;
	if (i2->Prev != nullptr)
		i2->Prev->Next = i2;
	i2->Next = (i1N == i2) ? i1 : i1N;
	if (i2->Next != nullptr)
		i2->Next->Prev = i2;
	i1->Prev = (i2P == i1) ? i2 : i2P;
	if (i1->Prev != nullptr)
		i1->Prev->Next = i1;
	i1->Next = (i2N == i1) ? i2 : i2N;
	if (i1->Next != nullptr)
		i1->Next->Prev = i1;

	// Fix head and tail
	if (i1->Next == nullptr)
		m_Tail = i1;
	if (i1->Prev == nullptr)
		m_Head = i1;
	if (i2->Next == nullptr)
		m_Tail = i2;
	if (i2->Prev == nullptr)
		m_Head = i2;
}

template <class T>
LList<T>::LList()
{
	m_Head = nullptr;
	m_Tail = nullptr;
	m_Size = 0;
}

template <class T>
LList<T>::~LList()
{
	Clear();
}

template <class T>
void LList<T>::PrintList()
{
	Item<T>* currentItem = m_Head;
	while (currentItem != nullptr)
	{
		std::cout << currentItem->Data << std::endl;
		currentItem = currentItem->Next;
	}
}

template <class T>
bool LList<T>::Empty()
{
	return m_Head == nullptr;
}

template <class T>
T LList<T>::ReturnNth(int n)
{
	return ReturnNthItem(n)->Data;
}

template <class T>
typename LList<T>::Item<T>* LList<T>::ReturnNthItem(int n)
{
	if (m_Head == nullptr)
		throw std::runtime_error("List is empty!");

	Item<T>* item = nullptr;
	if (n < m_Size / 2)
	{
		item = m_Head;
		// Loop forwards
		for (n; n > 0 && item->Next != nullptr; n--)
			item = item->Next;
	}
	else
	{
		item = m_Tail;
		// Loop backwards
		for (n = m_Size - n - 1; n > 0 && item->Prev != nullptr; n--)
			item = item->Prev;
	}

	if (item == nullptr)
		throw std::out_of_range("Index outside the bounds of the list");
	else
		return item;
}

template <class T>
T LList<T>::RemoveLast()
{
	if (m_Head == nullptr)
		throw std::runtime_error("List is empty!");

	T data = m_Tail->Data;
	Item<T>* prev = m_Tail->Prev;

	if (prev == nullptr)
		m_Head = nullptr;
	else
		prev->Next = nullptr;

	delete m_Tail;
	m_Tail = prev;
	m_Size--;

	return data;
}

template <class T>
T LList<T>::RemoveFirst()
{
	if (m_Head == nullptr)
		throw std::runtime_error("List is empty!");

	T data = m_Head->Data;
	Item<T>* next = m_Head->Next;

	if (next == nullptr)
		m_Tail = nullptr;
	else
		next->Prev = nullptr;

	delete m_Head;
	m_Head = next;
	m_Size--;

	return data;
}

template <class T>
void LList<T>::AddLast(T element)
{
	Item<T>* item = new Item<T>(element, m_Tail);

	if (m_Head == nullptr)
		m_Head = item;
	if (m_Tail != nullptr)
		m_Tail->Next = item;
	m_Tail = item;

	m_Size++;
}

template <class T>
void LList<T>::AddFirst(T element)
{
	Item<T>* item = new Item<T>(element, nullptr, m_Head);

	if (m_Tail == nullptr)
		m_Tail = item;
	if (m_Head != nullptr)
		m_Head->Prev = item;
	m_Head = item;

	m_Size++;
}

template <class T>
int LList<T>::Size()
{
	return m_Size;
}

template <typename T>
void LList<T>::InsertionSort(bool ascending /* = true */)
{
	for (auto i = m_Head; i != nullptr; i = i->Next) {
		for (auto j = i; j != m_Head; Swap(j, j->Prev)) {
			if (ascending && j->Data >= j->Prev->Data)
				break;
			else if (!ascending && j->Data <= j->Prev->Data)
				break;
		}
	}
}

#endif