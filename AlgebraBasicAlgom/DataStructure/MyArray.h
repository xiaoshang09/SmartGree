#pragma once

#ifndef FLOAT_EPSINON
#define FLOAT_EPSINON 1e-5
#endif // !1

template<class T>
class MyArray
{
public:
	MyArray(int size = 0)
	{
		if (size > 0)
		{
			m_pData = new T[size]();
			m_nSize = size;
			m_nReserve = size;
			m_pCount = new int(1);
		}
		else
		{
			m_pData = nullptr;
			m_nSize = 0;
			m_nReserve = 0;
			m_pCount = new int(0);
		}
	}

	MyArray(T* arr, int size)
	{
 		if (arr != nullptr && size > 0)
 		{
 			m_pData = new T[size]();
 			m_nSize = size;
 			m_nReserve = size;
 			m_pCount = new int(1);
			for (int i = 0; i < size; i++)
				m_pData[i] = arr[i];
 		}
		else
		{
			m_pData = nullptr;
			m_nSize = 0;
			m_nReserve = 0;
			m_pCount = new int(0);
		}
	}

	MyArray(const MyArray& arr) { addOne(arr); }

	virtual ~MyArray() { substructOne(); };

	MyArray& operator=(const MyArray& arr)
	{
		substructOne();
		addOne(arr);
		return *this;
	}

	inline T& operator[](int idx) const { return m_pData[idx]; }

	inline T& at(int idx) const { return m_pData[idx]; }

	inline int size() const { return m_nSize; }

	inline T* data() const { return m_pData; }

	inline void clear() { *this = MyArray(); }

	virtual void resize(int size)
	{
		m_nSize = size;
		if (size <= m_nReserve)
		{
			for (int i = 0; i < size; i++)
				m_pData[i] = T();
		}
		else
		{
		//	*this = MyArray(size);	// a method briefer but less efficient 

			substructOne();
			if (size > 0)
			{
				m_pData = new T[size]();
				m_nReserve = size;
				m_pCount = new int(1);
			}
			else
			{
				m_pData = nullptr;
				m_nSize = 0;
				m_nReserve = 0;
				m_pCount = new int(0);
			}
		}
	}

	MyArray clone()
	{
		MyArray obj(m_nSize);
		for (int i = 0; i < m_nSize; i++)
			obj.m_pData[i] = m_pData[i];
		return obj;
	}

	template<class T1>
	MyArray<T1> typeClone()
	{
		MyArray<T1> obj(m_nSize);
		for (int i = 0; i < m_nSize; i++)
			obj[i] = m_pData[i];
		return obj;
	}


protected:
	void substructOne()
	{
		(*m_pCount)--;
		if (*m_pCount == 0 && m_pData != nullptr)
		{
			delete[] m_pData;
			m_pData = nullptr;
			delete m_pCount;
			m_pCount = nullptr;
		}
	}

	void addOne(const MyArray& arr)
	{
		m_pData = arr.m_pData;
		m_nSize = arr.m_nSize;
		m_nReserve = arr.m_nReserve;
		m_pCount = arr.m_pCount;
		(*m_pCount)++;
	}

protected:
	T* m_pData;
	int m_nSize;
	int m_nReserve;
	int* m_pCount;
};

typedef MyArray<int> MyArrayInt;
typedef MyArray<float> MyArrayFlt;
typedef MyArray<double> MyArrayDbl;