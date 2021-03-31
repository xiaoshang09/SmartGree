#include "algebraBasicAlgom.h"

int var = 1;

void fft_(CP* arr, int size, bool inv)
{
	if (size == 1)
		return;

	int m = size / 2;

	static CP* buf = new CP[size];

	for (int i = 0; i < m; i++)
	{
		buf[i] = arr[i * 2];
		buf[i + m] = arr[i * 2 + 1];
	}

	memcpy(arr, buf, size * sizeof(CP));

	fft_(arr, m, inv);
	fft_(arr + m, m, inv);

	for (int i = 0; i < m; i++)
	{
		CP o = omega(size, i);
		if (inv) o = conj(o);
		buf[i] = arr[i] + o*arr[i + m];
		buf[i + m] = arr[i] - o*arr[i + m];
	}

	memcpy(arr, buf, size * sizeof(CP));

	if (inv)
	{
		for (int i = 0; i < size; i++)
			arr[i] /= size;
	}
}

void fft(CP* arr, int size, bool inv)
{
	int n = size / 2;
	CP* omg = new CP[n]();
	for (int i = 0; i < n; i++)
		omg[i] = omega(size, i);
	if (inv)
		for (int i = 0; i < n; i++)
			omg[i] = conj(omg[i]);

	int lim = 0;
	while (size >> lim > 1)
		lim++;

	for (int i = 0; i < size; i++)
	{
		int j = 0;
		for (int t = 0; t < lim; t++)
			if (i >> t & 1)
				j |= 1 << (lim - t - 1);

		if (i < j)
			swap(arr[i], arr[j]);
	}

	for (int l = 2; l <= size; l *= 2)
	{
		int m = l / 2;
		for (int i = 0; i < size; i += l)
		{
			for (int j = 0; j < m; j++)
			{
				CP temp = omg[size*j / l] * arr[i + m + j];
				arr[i + m + j] = arr[i + j] - temp;
				arr[i + j] += temp;
			}
		}
	}

	if (inv)
	{
		for (int i = 0; i < size; i++)
			arr[i] /= size;
	}

	delete[] omg;
}

int leastSquareLineFitting(vector<MyPoint2D>& pnts, MyLine2D& line, float& r)
{
	int n = pnts.size();
	if (n < 3)
		return -1;

	float a0(0.0f), a1(0.0f);
	int rtn = leastSquareLineFitting_(pnts, a0, a1, r);
	float old_angle = atanf(line.vy / line.vx)*DEG_PER_RAD;
	float new_angle = atanf(a1)*DEG_PER_RAD;
	if (fabs(old_angle - new_angle) < 0.01)
		return 1;

	if (rtn == 1)
		line = MyLine2D(a1, a0);
	else
		line = MyLine2D(1 / a1, -a0 / a1);

	bool ctnu = false;
	float sigma = 0.0f;
	float average = 0.0f;
	float* dist = new float[n]();
	MyPoint2D* foot_pnts = new MyPoint2D[n]();
	for (int i = 0; i < n; i++)
	{
		float dx = pnts[i].x - line.x;
		float dy = pnts[i].y - line.y;
		float v = line.vx*dx + line.vy*dy;
		foot_pnts[i].x = line.x + v*line.vx;
		foot_pnts[i].y = line.y + v*line.vy;
		dist[i] = fabs(line.vx*dy - line.vy*dx);
		average += 1 / dist[i];
	}
	average = n / average;
	for (int i = 0; i < n; i++)
		sigma += sqrtf(1 / fabs(dist[i] - average));
	sigma = 6.18 * (n / sigma)*(n / sigma);
	
	for (int i = n - 1; i >= 0; i--)
	{
		if (dist[i] - average > sigma)
		{
			pnts.erase(pnts.begin() + i);
			ctnu = true;
		}
	}
	delete[] dist;
	delete[] foot_pnts;
	
	if (ctnu)
		return leastSquareLineFitting(pnts, line, r);
	else
		return 1;
}

int leastSquareLineFitting_(const vector<MyPoint2D>& pnts, float& a0, float& a1, float& r)
{
	if (pnts.size() < 2)
		return -1;

	int n = pnts.size();
	float sum_x = 0.0f;
	float sum_y = 0.0f;
	float sum_xx = 0.0f;
	float sum_yy = 0.0f;
	float sum_xy = 0.0f;
	float Ex = 0.0f;
	float Ey = 0.0f;
	float Exx = 0.0f;
	float Eyy = 0.0f;
	float Exy = 0.0f;
	float Dx = 0.0f;
	float Dy = 0.0f;
	float Dxy = 0.0f;

	for (int i = 0; i < n; i++)
	{
		sum_x += pnts[i].x;
		sum_y += pnts[i].y;
		sum_xx += pnts[i].x*pnts[i].x;
		sum_yy += pnts[i].y*pnts[i].y;
		sum_xy += pnts[i].x*pnts[i].y;
	}
	Ex = sum_x / n;
	Ey = sum_y / n;
	Exx = sum_xx / n;
	Eyy = sum_yy / n;
	Exy = sum_xy / n;
	Dx = Exx - Ex*Ex;
	Dy = Eyy - Ey*Ey;
	Dxy = Exy - Ex*Ey;

	r = Dxy / sqrtf(Dx*Dy);
	if (Dx > Dy)
	{
		a1 = Dxy / Dx;
		a0 = Ey - a1*Ex;
		return 1;
	}
	else
	{
		a1 = Dxy / Dy;
		a0 = Ex - a1*Ey;
		return 0;
	}
}

int leastSquareCircleFitting(const vector<MyPoint2D>& pnts, MyCircle& circle, float& e)
{
	if (pnts.size() < 3)
		return -1;

	int n = pnts.size();
	float sum_x = 0.0f;
	float sum_y = 0.0f;
	float sum_xx = 0.0f;
	float sum_yy = 0.0f;
	float sum_xy = 0.0f;
	float sum_xxx = 0.0f;
	float sum_yyy = 0.0f;
	float sum_xxy = 0.0f;
	float sum_xyy = 0.0f;

	for (int i = 0; i < n; i++)
	{
		sum_x += pnts[i].x;
		sum_y += pnts[i].y;
		sum_xx += pnts[i].x*pnts[i].x;
		sum_yy += pnts[i].y*pnts[i].y;
		sum_xy += pnts[i].x*pnts[i].y;
		sum_xxx += pnts[i].x*pnts[i].x*pnts[i].x;
		sum_yyy += pnts[i].y*pnts[i].y*pnts[i].y;
		sum_xxy += pnts[i].x*pnts[i].x*pnts[i].y;
		sum_xyy += pnts[i].x*pnts[i].y*pnts[i].y;
	}

	float Ex = sum_x / n;
	float Ey = sum_y / n;
	float Exx = sum_xx / n;
	float Eyy = sum_yy / n;
	float Exy = sum_xy / n;
	float Exxx = sum_xxx / n;
	float Eyyy = sum_yyy / n;
	float Exxy = sum_xxy / n;
	float Exyy = sum_xyy / n;
	float a1 = Ex*Ex - Exx;
	float b1 = Ex*Ey - Exy;
	float c1 = (Exxx + Exyy - (Exx + Eyy)*Ex) / 2;
	float a2 = Ey*Ey - Eyy;
	float b2 = b1;
	float c2 = (Eyyy + Exxy - (Exx + Eyy)*Ey) / 2;

	float det = a1*a2 - b1*b2;
	if (fabs(det) < FLOAT_EPSINON)
		return -1;

	circle.x = (b1*c2 - a2*c1) / det;
	circle.y = (b2*c1 - a1*c2) / det;
	circle.r = sqrtf(circle.x*circle.x + circle.y*circle.y + Exx + Eyy - 2 * circle.x*Ex - 2 * circle.y*Ey);

	e = 0.0f;
	for (int i = 0; i < pnts.size(); i++)
	{
		float dx = pnts[i].x - circle.x;
		float dy = pnts[i].y - circle.y;
		float dist = sqrtf(dx*dx + dy*dy);
		e += (dist - circle.r)*(dist - circle.r);
	}
	e = sqrtf(e / (int)pnts.size()) / circle.r;

	return 1;
}

void tdma(float x[], const int n, const float a[], const float b[], const float c[], const float d[])
{
	if (n < 3)
		return;

	float* gama = new float[n]();
	float* tho = new float[n]();

	gama[0] = c[0] / b[0];
	tho[0] = d[0] / b[0];

	for (int i = 1; i < n; i++)
	{
		float temp = b[i] - a[i] * gama[i - 1];
		gama[i] = c[i] / temp;
		tho[i] = (d[i] - a[i] * tho[i - 1]) / temp;
	}


	x[n - 1] = tho[n - 1];
	for (int i = n - 2; i >= 0; i--)
		x[i] = tho[i] - gama[i] * x[i + 1];

	delete[] gama;
	delete[] tho;
}

void tdma_(float x[], const int n, float a[], float b[], float c[], float d[])
{
	if (n < 3)
		return;

	c[0] /= b[0];
	x[0] = d[0] / b[0];

	for (int i = 1; i < n; i++)
	{
		float temp = b[i] - a[i] * c[i - 1];
		c[i] /= temp;
		x[i] = (d[i] - a[i] * x[i - 1]) / temp;
	}
	
	for (int i = n - 2; i >= 0; i--)
		x[i] = x[i] - c[i] * x[i + 1];
}

int splineInterpolation(const vector<MyPoint2D>& pnts, vector<CubicCurve>& curves, int boundType, float k0, float k1)
{
	int n = pnts.size();
	if (n < 3) return -1;
	
	curves.clear();
	float* h = new float[n - 1]();
	float* m = new float[n]();
	float* a = new float[n]();
	float* b = new float[n]();
	float* c = new float[n]();
	float* d = new float[n]();

	h[0] = pnts[1].x - pnts[0].x;
	for (int i = 1; i < n - 1; i++)
	{
		h[i] = pnts[i + 1].x - pnts[i].x;
		a[i] = h[i - 1];
		b[i] = 2 * (h[i - 1] + h[i]);
		c[i] = h[i];
		d[i] = 6 * ((pnts[i + 1].y - pnts[i].y) / h[i] - (pnts[i].y - pnts[i - 1].y) / h[i - 1]);
	}

	switch (boundType)
	{
	case 1:
		b[0] = 1;
		b[n - 1] = 1;
		break;	
	case 2:
		b[0] = 2 * h[0];
		c[0] = h[0];
		d[0] = 6 * ((pnts[1].y - pnts[0].y) / h[0] - k0);
		a[n - 1] = h[n - 2];
		b[n - 1] = 2 * h[n - 2];
		d[n - 1] = 6 * (k1 - (pnts[n - 1].y - pnts[n - 2].y) / h[n - 2]);
		break;
	case 3:
		a[0] = -h[1];
		b[0] = h[0] + h[1];
		c[0] = -h[0];
		a[n - 1] = -h[n - 2];
		b[n - 1] = h[n - 3] + h[n - 2];
		c[n - 1] = -h[n - 3];
		break;
	default:
		break;
	}

	tdma(m, n, a, b, c, d);

	for (int i = 0; i < n - 1; i++)
	{
		CubicCurve curve;
		curve.a = pnts[i].y;
		curve.b = (pnts[i + 1].y - pnts[i].y) / h[i] - h[i] * m[i] / 2 - h[i] * (m[i + 1] - m[i]) / 6;
		curve.c = m[i] / 2;
		curve.d = (m[i + 1] - m[i]) / h[i] / 6;

		float x = pnts[i].x;
		curve.a += -curve.b*x + curve.c*x*x - curve.d*x*x*x;
		curve.b += -2 * curve.c*x + 3 * curve.d*x*x;
		curve.c += -3 * curve.d*x;
		curves.push_back(curve);
	}

	delete[] h;
	delete[] m;
	delete[] a;
	delete[] b;
	delete[] c;
	delete[] d;
	return 0;
}

float polynomialInterpolation(float x, const vector<MyPoint2D>& pnts, int type)
{
	if (pnts.empty())
		return 0;

	int n = pnts.size();
	float y = 0.0f;
	switch (type)
	{
	case 1:
	{
		float* map = new float[n*n]();
		for (int i = 0; i < n; i++)
			map[i] = pnts[i].y;

		y += pnts[0].y;
		for (int i = 1; i < n; i++)
		{
			int pole = i*n;
			for (int j = i; j < n; j++)
			{
				int idx = pole + j;
				map[idx] = (map[idx - n] - map[idx - n - 1]) / (pnts[j].x - pnts[j - i].x);
			}
			
			float polynome = 1.0f;
			for (int j = 0; j < i; j++)
				polynome *= (x - pnts[j].x);
			y += map[pole + i] * polynome;
		}
		delete[] map;
		break;
	}	
	case 2:
		for (int i = 0; i < pnts.size(); i++)
		{
			float polynome = 1.0f;
			for (int j = 0; j < pnts.size(); j++)
			{
				if (i == j)
					continue;
				polynome *= (x - pnts[j].x) / (pnts[i].x - pnts[j].x);
			}
			y += pnts[i].y*polynome;
		}
		break;
	default:
		break;
	}
	return y;
}

void fun_recursion(int b)
{
	if (b <= 0)
		return;
	int a = b - 1;
	fun_recursion(a);
}

int testMatrix()
{
	int a = 100;
	fun_recursion(a);


	MyMatrix2D<float> matrix0(2, 3);
	matrix0[0][0] = 1;
	matrix0[0][1] = 2;
	//	matrix0[0][2] = 3;
	matrix0[1][0] = 3;
	matrix0[1][1] = 4;
	//	matrix0[1][2] = 4;
	matrix0[2][0] = 5;
	matrix0[2][1] = 6;
	//	matrix0[2][2] = 6;

	MyMatrix2D<int> matrix1(1, 3);
	matrix1[0][0] = 7;
	matrix1[1][0] = 10;
	matrix1[2][0] = 20;

	MyMatrix2D<float> mi = overLinearEqs(matrix0, matrix1);

	MyMatrix2D<float> mm = matrix0*mi;


	return 0;
}