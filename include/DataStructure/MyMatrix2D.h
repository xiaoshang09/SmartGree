#pragma once
#include "MyArray.h"

enum ElemTransType
{
	ELEM_TRANS_ROW = 1,
	ELEM_TRANS_COL
};

template<class T>
class MyMatrix2D : public MyArray<T>
{
public:
	MyMatrix2D(int size = 0) :MyArray(size), m_nWidth(size), m_nHeight(1) {}
	MyMatrix2D(int width, int height) :MyArray(width*height), m_nWidth(width), m_nHeight(height) {}
	MyMatrix2D(const MyArray& matrix) :MyArray(matrix), m_nWidth(m_nSize), m_nHeight(1) {}
	MyMatrix2D(const MyMatrix2D& matrix):MyArray(matrix), m_nWidth(matrix.m_nWidth), m_nHeight(matrix.m_nHeight) {}

	MyMatrix2D& operator=(const MyMatrix2D& matrix)
	{
		MyArray::operator=(matrix);
		m_nWidth = matrix.m_nWidth;
		m_nHeight = matrix.m_nHeight;
		return *this;
	}

	MyMatrix2D operator*(MyMatrix2D& matrix)
	{
		assert(m_nWidth == matrix.m_nHeight);
		MyMatrix2D obj(matrix.m_nWidth, m_nHeight);
		for (int i = 0; i < obj.m_nHeight; i++)
		{
			for (int j = 0; j < obj.m_nWidth; j++)
			{
				for (int k = 0; k < m_nWidth; k++)
				{
					obj[i][j] += m_pData[i*m_nWidth + k] * matrix[k][j];
				}
			}
		}
		return obj;
	}

	inline T* operator[](int idx) { return m_pData + idx*m_nWidth; }

	inline T& at(int row, int col) { return m_pData[row*m_nWidth + col]; }

	inline T& at(int index) { return m_pData[index]; }

	inline int width() { return m_nWidth; }

	inline int height() { return m_nHeight; }

	template<class T1>
	void elemTransMultiple(int idx, T1 val, int type = ELEM_TRANS_ROW)
	{
		if (type == ELEM_TRANS_ROW)
		{
			T* ptr = m_pData + idx*m_nWidth;
			for (int i = 0; i < m_nWidth; i++)
				ptr[i] *= val;
		}
		else if (type == ELEM_TRANS_COL)
		{
			for (int i = 0; i < m_nHeight; i++)
				(*this)[i][idx] *= val;
		}	
	}

	void elemTransSwap(int idx0, int idx1, int type = ELEM_TRANS_ROW)
	{
		if (type == ELEM_TRANS_ROW)
		{
			for (int i = 0; i < m_nWidth; i++)
				swap((*this)[idx0][i], (*this)[idx1][i]);
		}
		else if (type == ELEM_TRANS_COL)
		{
			for (int i = 0; i < m_nHeight; i++)
				swap((*this)[i][idx0], (*this)[i][idx1]);
		}
	}

	template<class T1>
	void elemTransMultiAdd(int idx0, int idx1,T1 val, int type = ELEM_TRANS_ROW)
	{
		if (type == ELEM_TRANS_ROW)
		{
			for (int i = 0; i < m_nWidth; i++)
				(*this)[idx1][i] += (*this)[idx0][i] * val;
		}
		else if (type == ELEM_TRANS_COL)
		{
			for (int i = 0; i < m_nHeight; i++)
				(*this)[idx1][i] += (*this)[idx0][i] * val;
		}
	}

	T determinant()
	{
		assert(m_nWidth == m_nHeight);

		MyMatrix2D<double> matrix_copy = this->typeClone<double>();
		int len = matrix_copy.width();
		int count = 0;
		for (int i = 0; i < len; i++)
		{
			for (int j = i + 1; j < len; j++)
			{
				bool valid = false;
				for (int m = i + 1; m <= len; m++)
				{
					if (abs(matrix_copy[i][i]) < FLOAT_EPSINON)
					{
						if (m < len)
						{
							matrix_copy.elemTransSwap(i, m);
							count++;
						}
					}
					else
					{
						valid = true;
						break;
					}
				}

				if (valid)
				{
					double quotient = matrix_copy[j][i] / matrix_copy[i][i];
					matrix_copy[j][i] = 0;
					for (int k = i + 1; k < len; k++)
						matrix_copy[j][k] -= quotient*matrix_copy[i][k];
				}
				else
					return 0;
			}
		}

		double rtn = 1;
		for (int i = 0; i < len; i++)
			rtn *= matrix_copy[i][i];

		if (abs(rtn - round(rtn)) < FLOAT_EPSINON)
			rtn = round(rtn);

		if (count % 2 == 0)
			return (T)rtn;
		else
			return -(T)rtn;

	}

	void resize(int size)
	{
		MyArray::resize(size);
		m_nWidth = size;
		m_nHeight = 1;
	}

	void resize(int width,int height)
	{
		MyArray::resize(width*height);
		m_nWidth = width;
		m_nHeight = height;
	}

	MyMatrix2D clone()
	{
		MyMatrix2D obj = MyArray::clone();
		obj.m_nWidth = m_nWidth;
		obj.m_nHeight = m_nHeight;
		return obj;
	}

	template<class T1> 
	MyMatrix2D<T1> typeClone()
	{
		MyMatrix2D<T1> obj(m_nWidth, m_nHeight);
		for (int i = 0; i < m_nSize; i++)
			obj.MyArray<T1>::at(i) = m_pData[i];
		return obj;
	}
	
protected:
	int m_nWidth;
	int m_nHeight;
};