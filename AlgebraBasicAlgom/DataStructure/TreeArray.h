#pragma once
#include "MyArray.h"

#define LOW_BIT(a) ((a)&-(a))

#ifndef MAX
# define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

#ifndef MIN
# define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

template<class T>
class TreeArray: public MyArray<T>
{
public:
	TreeArray(int size = 0) :MyArray(size) {}

	TreeArray(T* arr, int size) { buildTree_(arr, size); }

	TreeArray(MyArray& myarr, bool flag = false) :MyArray(myarr) { if(flag) buildTree(myarr); }
	
	void buildTree(T* arr, int size)
	{
		substructOne();
		buildTree_(arr, size);
	}

	void buildTree(MyArray& myarr)
	{
		if (this != &myarr)
		{
			substructOne();
			buildTree_(myarr.data(), myarr.size());
		}
		else
		{
			MyArray copyarr(myarr);
			substructOne();
			buildTree_(copyarr.data(), copyarr.size());
		}
	}

	void updateTree(int idx, T add)
	{
		for (int i = idx + 1; i <= m_nSize; i += LOW_BIT(i))
			m_pData[i - 1] += add;
	}

	void recover(int* arr, int size)
	{
		if (m_nSize != size)
			return;

		for (int i = 0; i < size; i++)
			arr[i] = getValue(i);
	}

	T getValue(int idx)
	{
		T rtn = m_pData[idx];
		int diff = LOW_BIT(idx + 1);
		idx -= diff;
		diff >>= 1;
		while (diff > 0)
		{
			idx += diff;
			rtn -= m_pData[idx];
			diff >>= 1;
		}
		return rtn;
	}

	T sum(int idx)
	{
		if (idx < 0 || idx >= m_nSize)
			return 0;
		else
			return sum_(idx);
	}

	int query(T& tar)
	{
		if (tar > sum_(m_nSize - 1) || tar <= 0)
			return -1;
		else
		{
			int rtn = query_(tar, 0, m_nSize - 1);
			return rtn;
		}	
	}

private:
	T sum_(int idx)
	{
		if (idx >= 0)
			return m_pData[idx] + sum_(idx - LOW_BIT(idx + 1));
		else
			return 0;
	}

	int query_(T& tar, int p, int q)
	{
		if (p == q || tar <= m_pData[p])
			return p;

		int prev;
		while (p < q && tar > m_pData[p])
		{
			prev = p;
			p += LOW_BIT(p + 1);
		}
		return query_(tar -= m_pData[prev], prev + 1, MIN(p, q));
	}

	void buildTree_(T* arr, int size)
	{
		if (arr != nullptr && size > 0)
		{
			m_pData = new T[size]();
			m_nSize = size;
			m_nReserve = size;
			m_pCount = new int(1);
			for (int i = 0; i < m_nSize; i++)
				updateTree(i, arr[i]);
		}
		else
		{
			m_pData = nullptr;
			m_nSize = 0;
			m_nReserve = 0;
			m_pCount = new int(0);
		}
	}
};