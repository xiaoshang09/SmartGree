#pragma once
#include "MyDataType.h"
#include "DataStructure/MyMatrix2D.h"
#include "DataStructure/BasicData.h"
#include <vector>
#include <complex>

#ifdef ALGEBRABASICALGOM_EXPORTS
#define ALGEBRABASICALGOM_API __declspec(dllexport)
#else
#define ALGEBRABASICALGOM_API __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"AlgebraBasicAlgomD.lib")
#else
#pragma comment(lib,"AlgebraBasicAlgom.lib")
#endif
#endif	//ALGEBRABASICALGOM_EXPORTS



typedef complex<float> CP;

inline CP omega(int n, int k)
{
	return CP(cosf(2 * PI*k / n), sinf(2 * PI*k / n));
}

struct CubicCurve
{
	CubicCurve() :a(0.0f), b(0.0f), c(0.0f), d(0.0f) {}

	float a;
	float b;
	float c;
	float d;
};

#ifndef __VAR_H_
#define __VAR_H_
extern int var;
#endif

void fft_(CP* arr, int size, bool inv);

void ALGEBRABASICALGOM_API fft(CP* arr, int size, bool inv);

int ALGEBRABASICALGOM_API leastSquareLineFitting(vector<MyPoint2D>& pnts, MyLine2D& line, float& r = REF_DEFAULT);

int leastSquareLineFitting_(const vector<MyPoint2D>& pnts, float& a0, float& a1, float& r = REF_DEFAULT);

int ALGEBRABASICALGOM_API leastSquareCircleFitting(const vector<MyPoint2D>& pnts, MyCircle& circle, float& e = REF_DEFAULT);

void ALGEBRABASICALGOM_API tdma(float x[], const int n, const float a[], const float b[], const float c[], const float d[]);

void tdma_(float x[], const int n, float a[], float b[], float c[], float d[]);

//boundType: 1->natural, 2->clamped, 3->not-a-knot
int ALGEBRABASICALGOM_API splineInterpolation(const vector<MyPoint2D>& pnts, vector<CubicCurve>& curves, int boundType = 1, float k0 = 0, float k1 = 0);

//type: 1->Newton interpolation, 2->Largrange interpolation
float ALGEBRABASICALGOM_API polynomialInterpolation(float x, const vector<MyPoint2D>& pnts, int type = 1);

template<class T>
T matDet(MyMatrix2D<T>& matrix)
{
	assert(matrix.width() == matrix.height());

	MyMatrix2D<double> matrix_copy = matrix.typeClone<double>();
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
		return rtn;
	else
		return -rtn;
}

//linear equations: A*x=b
template<class T0,class T1>
MyMatrix2D<float> linearEqsCramer(MyMatrix2D<T0>& A, MyMatrix2D<T1>& b)
{
	assert(A.width() == A.height() && A.width() == b.height() && b.width() == 1);

	int len = A.width();
	MyMatrix2D<float> x(1, len);

	//	T0 detA = matDet(A);
	T0 detA = A.determinant();

	if (abs(detA) > FLOAT_EPSINON)
	{
		for (int i = 0; i < len; i++)
		{
			MyMatrix2D<T0> A_ = A.clone();
			for (int j = 0; j < len; j++)
				A_[j][i] = b[j][0];

			//	x[i][0] = (float)matDet(A_) / detA;
			x[i][0] = (float)A_.determinant() / detA;
		}
	}
	return x;
}

//overdetermined linear equations: A*x=b
template<class T0, class T1>
MyMatrix2D<float> overLinearEqs(MyMatrix2D<T0>& A, MyMatrix2D<T1>& b)
{
	assert(A.width() <= A.height() && A.height() == b.height() && b.width() == 1);

	int width = A.width();
	int height = A.height();
	if (width == height)
		return linearEqsCramer(A, b);

	MyMatrix2D<float> x(1, width);
	MyMatrix2D<float> A_(width, width);
	MyMatrix2D<float> b_(1, width);
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i <= j)
			{
				for (int k = 0; k < height; k++)
					A_[i][j] += A[k][i] * A[k][j];
			}
			else
				A_[i][j] = A_[j][i];
		}
	}
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			b_[i][0] += A[j][i] * b[j][0];

	return linearEqsCramer(A_, b_);
}

int ALGEBRABASICALGOM_API testMatrix();