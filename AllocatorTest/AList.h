// Simon Holmberg 930302-4492

#ifndef ALIST_HPP
#define ALIST_HPP

#include <iostream>
#include <vector>

#include "List.h"

template <typename T>
class AList : public List<T>
{
public:
	AList();
	~AList();
	int Size() override;
	void AddFirst(T element) override;
	void AddLast(T element) override;
	T RemoveFirst() override;
	T RemoveLast()override;
	T ReturnNth(int n)override;
	bool Empty() override;
	void Clear() override;
	void PrintList() override;
	void Swap(int n, int m) override;
	void InsertionSort(bool ascending = true) override;
	void QuickSort(bool ascending = true) override;

private:
	std::vector<T> m_Vector;

	void QuickSort(bool ascending, typename std::vector<T>::iterator first, typename std::vector<T>::iterator last);
};

template <typename T>
void AList<T>::Clear()
{
	m_Vector.clear();
}

template <typename T>
void AList<T>::QuickSort(bool ascending /*= true*/)
{
	QuickSort(ascending, m_Vector.begin(), m_Vector.end() - 1);
}

template <typename T>
void AList<T>::QuickSort(bool ascending, typename std::vector<T>::iterator first, typename std::vector<T>::iterator last)
{
	if (last - first <= 0)
		return;

	typename std::vector<T>::iterator left = first;
	typename std::vector<T>::iterator right = last;
	typename std::vector<T>::iterator pivot = left++;

	while (left != right) {
		if (*left <= *pivot) {
			++left;
		} else {
			while ((left != right) && (*pivot <= *right)) {
				--right;
			}
			std::iter_swap(left, right);
		}
	}
	--left;
	std::iter_swap(pivot, left);

	QuickSort(ascending, first, left);
	QuickSort(ascending, right, last);
}

template <typename T>
void AList<T>::Swap(int n, int m)
{
	std::swap(m_Vector.begin() + n, m_Vector.begin() + m);
}

template <class T>
AList<T>::AList()
{

}

template <class T>
AList<T>::~AList()
{

}

template <class T>
int AList<T>::Size()
{
	return m_Vector.size();
}

template <class T>
void AList<T>::AddFirst(T element)
{
	m_Vector.insert(m_Vector.begin(), element);
}

template <class T>
void AList<T>::AddLast(T element)
{
	m_Vector.push_back(element);
}

template <class T>
T AList<T>::RemoveFirst()
{
	T element = m_Vector.front();
	m_Vector.erase(m_Vector.begin());
	return element;
}

template <class T>
T AList<T>::RemoveLast()
{
	T element = m_Vector.back();
	m_Vector.pop_back();
	return element;
}

template <class T>
T AList<T>::ReturnNth(int n)
{
	return m_Vector.at(n);
}

template <typename T>
void AList<T>::InsertionSort(bool ascending /* = true */)
{
	for (auto i = m_Vector.begin() + 1; i != m_Vector.end(); i++) {
		for (auto j = i; j != m_Vector.begin(); j--) {
			if (ascending && *j >= *(j - 1))
				break;
			else if (!ascending && *j <= *(j - 1))
				break;
			std::iter_swap(j - 1, j);
		}
	}
}

template <class T>
void AList<T>::PrintList()
{
	for (auto it = m_Vector.begin(); it != m_Vector.end(); it++)
		std::cout << (*it) << std::endl;
}

template <class T>
bool AList<T>::Empty()
{
	return m_Vector.empty();
}

#endif
