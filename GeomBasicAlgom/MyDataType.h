#pragma once
#include <memory>
#include <vector>
#include <cmath>
#include <complex>
#include <assert.h>

using namespace std;

#ifndef FLOAT_EPSINON
#define FLOAT_EPSINON 1e-5
#endif

#define FLOAT_MAX HUGE_VALF
#define FLOAT_MIN (-HUGE_VALF)

#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef PI
#define PI 3.14159265358979
#endif

#define DEG_PER_RAD 57.2957795130823

#define EXCHANGE(val1, val2) {auto temp = val1; val1 = val2; val2 = temp;}

#define CONSTRAIN_REGION(val, a, b) { (val) = MAX(val, MIN(a, b)); (val) = MIN(val, MAX(a, b)); }

static float REF_DEFAULT = 0.0f;

typedef float V32F;
typedef double V64F;

template<class T1, class T2>
void normalization(T1& val, T2 lower, T2 upper)
{
	T2 t = upper - lower;
	int divisor = floor((double)(val - lower) / t);
	val -= divisor*t;
}

#define UNIT_ARRAY_2D(vx,vy) \
{\
	auto mod = sqrt((vx)*(vx)+(vy)*(vy)); \
	if (mod > FLOAT_EPSINON)\
	{\
		vx = (vx) / mod; \
		vy = (vy) / mod; \
	}\
}

#define UNIT_ARRAY_3D(vx,vy,vz) \
{\
	auto mod = sqrt((vx)*(vx)+(vy)*(vy)+(vz)*(vz)); \
	if(mod > FLOAT_EPSINON)\
	{\
		vx = (vx) / mod; \
		vy = (vy) / mod; \
		vz = (vz) / mod; \
	}\
}

#define UNIT_VECTOR_2D(vec) UNIT_ARRAY_2D(vec.vx,vec.vy)

#define UNIT_VECTOR_3D(vec) UNIT_ARRAY_3D(vec.vx,vec.vy,vec.vz)

#define MOD_VECTOR_2D(vec) sqrt(vec.vx*vec.vx+vec.vy*vec.vy)

#define MOD_VECTOR_3D(vec) sqrt(vec.vx*vec.vx+vec.vy*vec.vy+vec.vz*vec.vz)

union MyPoint3D
{
	MyPoint3D() :x(0.0f), y(0.0f), z(0.0f) {}
	MyPoint3D(float x_, float y_, float z_) :x(x_), y(y_), z(z_) {}

	MyPoint3D operator+(const MyPoint3D& pnt) const
	{
		MyPoint3D rtn;
		rtn.x = x + pnt.x;
		rtn.y = y + pnt.y;
		rtn.z = z + pnt.z;
		return rtn;
	}

	MyPoint3D operator-(const MyPoint3D& pnt) const
	{
		MyPoint3D rtn;
		rtn.x = x - pnt.x;
		rtn.y = y - pnt.y;
		rtn.z = z - pnt.z;
		return rtn;
	}

	bool operator==(const MyPoint3D& pnt) const
	{
		return fabs(x - pnt.x) < FLOAT_EPSINON && fabs(y - pnt.y) < FLOAT_EPSINON && fabs(z - pnt.z) < FLOAT_EPSINON;
	}

	template<class T>
	friend MyPoint3D operator*(T k, const MyPoint3D& pnt);

	template<class T>
	MyPoint3D operator*(T k) const
	{
		MyPoint3D rtn;
		rtn.x = x*k;
		rtn.y = y*k;
		rtn.z = z*k;
		return rtn;
	}

	template<class T>
	MyPoint3D operator/(T k) const
	{
		MyPoint3D rtn;
		rtn.x = x / k;
		rtn.y = y / k;
		rtn.z = z / k;
		return rtn;
	}

	float data[3];
	struct
	{
		float x;
		float y;
		float z;
	};
};
template<class T>
MyPoint3D operator*(T k, const MyPoint3D& pnt)
{
	MyPoint3D rtn;
	rtn.x = pnt.x*k;
	rtn.y = pnt.y*k;
	rtn.z = pnt.z*k;
	return rtn;
}

union MyPoint2D
{
	MyPoint2D() :x(0.0f), y(0.0f) {}
	MyPoint2D(float x_, float y_) :x(x_), y(y_) {}

	MyPoint2D operator+(const MyPoint2D& pnt) const
	{
		MyPoint2D rtn;
		rtn.x = x + pnt.x;
		rtn.y = y + pnt.y;
		return rtn;
	}

	MyPoint2D operator-(const MyPoint2D& pnt) const
	{
		MyPoint2D rtn;
		rtn.x = x - pnt.x;
		rtn.y = y - pnt.y;
		return rtn;
	}

	bool operator==(const MyPoint2D& pnt) const
	{
		return fabs(x - pnt.x) < FLOAT_EPSINON && fabs(y - pnt.y) < FLOAT_EPSINON;
	}

	template<class T>
	friend MyPoint2D operator*(T k, const MyPoint2D& pnt);

	template<class T>
	MyPoint2D operator*(T k) const
	{
		MyPoint2D rtn;
		rtn.x = x*k;
		rtn.y = y*k;
		return rtn;
	}

	template<class T>
	MyPoint2D operator/(T k) const
	{
		MyPoint2D rtn;
		rtn.x = x / k;
		rtn.y = y / k;
		return rtn;
	}

	float data[2];
	struct
	{
		float x;
		float y;
	};
};
template<class T>
MyPoint2D operator*(T k, const MyPoint2D& pnt)
{
	MyPoint2D rtn;
	rtn.x = pnt.x*k;
	rtn.y = pnt.y*k;
	return rtn;
}

union MyLine3D
{
	MyLine3D() :x(0.0f), y(0.0f), z(0.0f), vx(0.0f), vy(0.0f), vz(1.0f) {}
	MyLine3D(float x_, float y_, float z_) :x(x_), y(y_), z(z_), vx(0.0f), vy(0.0f), vz(1.0f) {}
	MyLine3D(float x_, float y_, float z_, float vx_, float vy_, float vz_)
	{
		x = x_;
		y = y_;
		z = z_;
		vx = vx_;
		vy = vy_;
		vz = vz_;
		UNIT_ARRAY_3D(vx, vy, vz);
	}
	MyLine3D(const MyPoint3D& pnt0,const MyPoint3D& pnt1)
	{
		x = (pnt0.x + pnt1.x) / 2;
		y = (pnt0.y + pnt1.y) / 2;
		z = (pnt0.z + pnt1.z) / 2;
		vx = pnt1.x - pnt0.x;
		vy = pnt1.y - pnt0.y;
		vz = pnt1.z - pnt0.z;
		UNIT_ARRAY_3D(vx, vy, vz);
	}

	float data[6];
	struct  
	{
		float x;
		float y;
		float z;
		float vx;
		float vy;
		float vz;
	};
}; typedef MyLine3D MyPose3D;

union MyLine2D
{
	MyLine2D() :x(0.0f), y(0.0f), vx(0.0f), vy(1.0f) {}
	MyLine2D(float x_, float y_, float vx_, float vy_)
	{
		x = x_;
		y = y_;
		vx = vx_;
		vy = vy_;
		UNIT_ARRAY_2D(vx, vy);
	}
	MyLine2D(float x_, float y_, float angle)
	{
		x = x_;
		y = y_;
		vx = cosf(angle / DEG_PER_RAD);
		vy = sinf(angle / DEG_PER_RAD);
	}
	MyLine2D(float k, float b)
	{
		x = 0;
		y = b;
		vx = 1 / sqrt(k*k + 1);
		vy = k*vx;
	}
	MyLine2D(const MyPoint2D& pnt0, const MyPoint2D& pnt1)
	{
		x = (pnt0.x + pnt1.x) / 2;
		y = (pnt0.y + pnt1.y) / 2;
		vx = pnt1.x - pnt0.x;
		vy = pnt1.y - pnt0.y;
		UNIT_ARRAY_2D(vx, vy);
	}

	float data[4];
	struct  
	{
		float x;
		float y;
		float vx;
		float vy;
	};
}; typedef MyLine2D MyPose2D;

union MyLineSeg3D
{
	MyLineSeg3D() :sx(0.0f), sy(0.0f), sz(0.0f), ex(0.0f), ey(0.0f), ez(0.0f) {}
	MyLineSeg3D(const MyPoint3D& pnt_s, const MyPoint3D& pnt_e) :
		sx(pnt_s.x), sy(pnt_s.y), sz(pnt_s.z), ex(pnt_e.x), ey(pnt_e.y), ez(pnt_e.z) {}
	MyLineSeg3D(float sx_, float sy_, float sz_, float ex_, float ey_, float ez_) :
		sx(sx_), sy(sy_), sz(sz_), ex(ex_), ey(ey_), ez(ez_) {}

	float data[6];
	struct
	{
		float sx;
		float sy;
		float sz;
		float ex;
		float ey;
		float ez;
	};
};

union MyLineSeg2D
{
	MyLineSeg2D() :sx(0.0f), sy(0.0f), ex(0.0f), ey(0.0f) {}
	MyLineSeg2D(const MyPoint2D& pnt_s, const MyPoint2D& pnt_e) :
		sx(pnt_s.x), sy(pnt_s.y), ex(pnt_e.x), ey(pnt_e.y) {}
	MyLineSeg2D(float sx_, float sy_, float ex_, float ey_) :
		sx(sx_), sy(sy_), ex(ex_), ey(ey_) {}

	float data[4];
	struct
	{
		float sx;
		float sy;
		float ex;
		float ey;
	};
};

union MyPolyline2D
{
	MyPolyline2D(int num_ = 0, int flag_ = 1)
	{
		if (num_ > 0)
		{
			xp = new float[num_]();
			yp = new float[num_]();
			num = num_;
			flag = flag_;
		}
		else
		{
			xp = nullptr;
			yp = nullptr;
			num = 0;
			flag = 1;
		}
	}

	MyPolyline2D(const MyPolyline2D& obj)
	{
		if (obj.num > 0)
		{
			xp = new float[obj.num];
			yp = new float[obj.num];
			memcpy(xp, obj.xp, obj.num * sizeof(float));
			memcpy(yp, obj.yp, obj.num * sizeof(float));
			num = obj.num;
			flag = obj.flag;
		}
		else
		{
			xp = nullptr;
			yp = nullptr;
			num = 0;
			flag = 1;
		}
	}

	~MyPolyline2D()
	{
		if (xp != nullptr)
		{
			delete[] xp;
			xp = nullptr;
		}
		if (yp != nullptr)
		{
			delete[] yp;
			yp = nullptr;
		}
	}

	MyPolyline2D& operator=(const MyPolyline2D& obj)
	{
		if (xp != nullptr)
			delete[] xp;
		if (yp != nullptr)
			delete[] yp;

		if (obj.num > 0)
		{
			xp = new float[obj.num];
			yp = new float[obj.num];
			memcpy(xp, obj.xp, obj.num * sizeof(float));
			memcpy(yp, obj.yp, obj.num * sizeof(float));
			num = obj.num;
			flag = obj.flag;
		}
		else
		{
			xp = nullptr;
			yp = nullptr;
			num = 0;
			flag = 1;
		}
		return *this;
	}

	float data[4];
	struct
	{
		float* xp;
		float* yp;
		int num;
		int flag;	//0 -> closed, !0 -> open
	};
};

union MyPolygon2D
{
	MyPolygon2D(int num_ = 0)
	{
		if (num_ > 0)
		{
			xp = new float[num_]();
			yp = new float[num_]();
			num = num_;
		}
		else
		{
			xp = nullptr;
			yp = nullptr;
			num = 0;
		}
	}
	~MyPolygon2D()
	{
		if (xp != nullptr)
		{
			delete[] xp;
			xp = nullptr;
		}
		if (yp != nullptr)
		{
			delete[] yp;
			yp = nullptr;
		}
	}

	MyPolygon2D& operator=(const MyPolygon2D& obj)
	{
		if (xp != nullptr)
			delete[] xp;
		if (yp != nullptr)
			delete[] yp;

		if (obj.num > 0)
		{
			xp = new float[obj.num];
			yp = new float[obj.num];
			memcpy(xp, obj.xp, obj.num * sizeof(float));
			memcpy(yp, obj.yp, obj.num * sizeof(float));
			num = obj.num;
		}
		else
		{
			xp = nullptr;
			yp = nullptr;
			num = 0;
		}
		return *this;
	}

	float data[3];
	struct
	{
		float* xp;
		float* yp;
		int num;
	};
};

union MyVector3D
{
	MyVector3D() :vx(0.0f), vy(0.0f), vz(0.0f) {};
	MyVector3D(float x, float y, float z) :vx(x), vy(y), vz(z) {};

	MyVector3D operator+(const MyVector3D& vec) const
	{
		MyVector3D rtn;
		rtn.vx = vx + vec.vx;
		rtn.vy = vy + vec.vy;
		rtn.vz = vz + vec.vz;
		return rtn;
	}

	MyVector3D operator-(const MyVector3D& vec) const
	{
		MyVector3D rtn;
		rtn.vx = vx - vec.vx;
		rtn.vy = vy - vec.vy;
		rtn.vz = vz - vec.vz;
		return rtn;
	}

	bool operator==(const MyVector3D& vec) const
	{
		return fabs(vx - vec.vx) < FLOAT_EPSINON && fabs(vy - vec.vy) < FLOAT_EPSINON && fabs(vz - vec.vz) < FLOAT_EPSINON;
	}

	template<class T>
	friend MyVector3D operator*(T k, const MyVector3D& vec);

	template<class T>
	MyVector3D operator*(T k) const
	{
		MyVector3D rtn;
		rtn.vx = vx*k;
		rtn.vy = vy*k;
		rtn.vz = vz*k;
		return rtn;
	}

	template<class T>
	MyVector3D operator/(T k) const
	{
		MyVector3D rtn;
		rtn.vx = vx / k;
		rtn.vy = vy / k;
		rtn.vz = vz / k;
		return rtn;
	}

	float data[3];
	struct
	{
		float vx;
		float vy;
		float vz;
	};
};
template<class T>
MyVector3D operator*(T k, const MyVector3D& vec)
{
	MyVector3D rtn;
	rtn.vx = vec.vx*k;
	rtn.vy = vec.vy*k;
	rtn.vz = vec.vz*k;
	return rtn;
}

union MyVector2D
{
	MyVector2D() :vx(0.0f), vy(0.0f) {};
	MyVector2D(float x, float y) :vx(x), vy(y) {};

	MyVector2D operator+(const MyVector2D& vec) const
	{
		MyVector2D rtn;
		rtn.vx = vx + vec.vx;
		rtn.vy = vy + vec.vy;
		return rtn;
	}

	MyVector2D operator-(const MyVector2D& vec) const
	{
		MyVector2D rtn;
		rtn.vx = vx - vec.vx;
		rtn.vy = vy - vec.vy;
		return rtn;
	}

	bool operator==(const MyVector2D& vec) const
	{
		return fabs(vx - vec.vx) < FLOAT_EPSINON && fabs(vy - vec.vy) < FLOAT_EPSINON;
	}

	template<class T>
	friend MyVector2D operator*(T k, const MyVector2D& vec);

	template<class T>
	MyVector2D operator*(T k) const
	{
		MyVector2D rtn;
		rtn.vx = vx*k;
		rtn.vy = vy*k;
		return rtn;
	}

	template<class T>
	MyVector2D operator/(T k) const
	{
		MyVector2D rtn;
		rtn.vx = vx / k;
		rtn.vy = vy / k;
		return rtn;
	}

	float data[2];
	struct
	{
		float vx;
		float vy;
	};
};
template<class T>
MyVector2D operator*(T k, const MyVector2D& vec)
{
	MyVector2D rtn;
	rtn.vx = vec.vx*k;
	rtn.vy = vec.vy*k;
	return rtn;
}

union MyPlane3D
{
	MyPlane3D() :A(0.0f), B(0.0f), C(1.0f), D(0.0f) {};
	MyPlane3D(float a, float b, float c, float d) :A(a), B(b), C(c), D(d) {};
	MyPlane3D(const MyPoint3D& pnt, const MyVector3D& vec)
	{
		A = vec.vx;
		B = vec.vy;
		C = vec.vz;
		D = -pnt.x*vec.vx - pnt.y*vec.vy - pnt.z*vec.vz;
	}
	MyPlane3D(const MyPose3D& pos)
	{
		A = pos.vx;
		B = pos.vy;
		C = pos.vz;
		D = -pos.x*pos.vx - pos.y*pos.vy - pos.z*pos.vz;
	}
	MyPlane3D(const MyPoint3D& pnt0, const MyPoint3D& pnt1, const MyPoint3D& pnt2)
	{
		float x0 = pnt0.x;
		float y0 = pnt0.y;
		float z0 = pnt0.z;
		float x1 = pnt1.x;
		float y1 = pnt1.y;
		float z1 = pnt1.z;
		float x2 = pnt2.x;
		float y2 = pnt2.y;
		float z2 = pnt2.z;
		float _i = (y1 - y0)*(z2 - z0) - (z1 - z0)*(y2 - y0);
		float _j = (z1 - z0)*(x2 - x0) - (x1 - x0)*(z2 - z0);
		float _k = (x1 - x0)*(y2 - y0) - (y1 - y0)*(x2 - x0);
		float Det = x0*y1*z2 + x1*y2*z0 + x2*y0*z1 - x2*y1*z0 - x1*y0*z2 - x0*y2*z1;

		if (abs(_i) < FLOAT_EPSINON && abs(_j) < FLOAT_EPSINON && abs(_k) < FLOAT_EPSINON)
		{
			A = 0;
			B = 0;
			C = 0;
			D = 0;
		}
		else if (abs(Det) < FLOAT_EPSINON)
		{
			A = (z0*y1 - z1*y0) / (x0*y1 - x1*y0);
			B = (x0*z1 - x1*z0) / (x0*y1 - x1*y0);
			C = -1;
			D = 0;
		}
		else
		{
			float Det0 = y1*z2 + y2*z0 + y0*z1 - y1*z0 - y0*z2 - y2*z1;
			float Det1 = x0*z2 + x1*z0 + x2*z1 - x2*z0 - x1*z2 - x0*z1;
			float Det2 = x0*y1 + x1*y2 + x2*y0 - x2*y1 - x1*y0 - x0*y2;
			A = Det0 / Det;
			B = Det1 / Det;
			C = Det2 / Det;
			D = -1;
		}
	}

	float data[4];
	struct 
	{
		float A;
		float B;
		float C;
		float D;
	};	
};

union MyCircle
{
	MyCircle() :x(0.0f), y(0.0f), r(0.0f) {}
	MyCircle(const MyPoint2D& pnt, float r_) :x(pnt.x), y(pnt.y),r(r_) {}
	MyCircle(float x_, float y_, float r_) :x(x_), y(y_), r(r_) {}

	float data[3];
	struct  
	{
		float x;
		float y;
		float r;
	};
};

union MyRing
{
	MyRing() :x(0.0f), y(0.0f), min_r(0.0f), max_r(0.0f){}
	MyRing(const MyPoint2D& pnt, float min_r_, float max_r_) :
		x(pnt.x), y(pnt.y), min_r(min_r_), max_r(max_r_)
	{
		assert(0 <= min_r && min_r <= max_r);
	};
	MyRing(float x_, float y_, float min_r_, float max_r_) :x(x_), y(y_), min_r(min_r_), max_r(max_r_)
	{
		assert(0 <= min_r && min_r <= max_r);
	}

	float data[4];
	struct 
	{
		float x;
		float y;
		float min_r;
		float max_r;
	};
};

union MyRect
{
	MyRect() :x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}
	MyRect(float x_, float y_, float w_, float h_) :x(x_), y(y_), w(w_), h(h_) {}

	float data[4];
	struct
	{
		float x;
		float y;
		float w;
		float h;
	};
};

union MyRotRect
{
	MyRotRect() :x(0.0f), y(0.0f), w(0.0f), h(0.0f), angle(0.0f) {}
	MyRotRect(float x_, float y_, float w_, float h_, float angle_) :
		x(x_), y(y_), w(w_), h(h_), angle(angle_)
	{
		normalization(angle, -180, 180);
	}
	MyRotRect(MyPoint2D pnt_s, MyPoint2D pnt_e, float h_) :h(h_)
	{
		x = (pnt_s.x + pnt_e.x) / 2.0f;
		y = (pnt_s.y + pnt_e.y) / 2.0f;
		w = sqrtf((pnt_s.x - pnt_e.x)*(pnt_s.x - pnt_e.x) + (pnt_s.y - pnt_e.y)*(pnt_s.y - pnt_e.y));
		angle = atan2f(pnt_e.y - pnt_s.y, pnt_e.x - pnt_s.x)*DEG_PER_RAD;
	}

	float data[5];
	struct 
	{
		float x;
		float y;
		float w;
		float h;
		float angle;	//-180~180 degree
	};
};

struct MyRectArray
{
	MyRectArray() :layout_rect(), rows(0), cols(0), item_width(0.0f), item_height(0.0f), item_angle(0.0f) {}
	MyRectArray(MyRotRect rect, int r, int c, float width, float height, float angle) :
		layout_rect(rect), rows(r), cols(c), item_width(width), item_height(height), item_angle(angle) {}

	MyRotRect layout_rect;
	int rows;
	int cols;
	float item_width;
	float item_height;
	float item_angle;
};

union MySphere
{
	MySphere() :x(0.0f), y(0.0f), z(0.0f), r(0.0f) {}
	MySphere(const MyPoint3D& pnt, float r_) :x(pnt.x), y(pnt.y), z(pnt.z), r(r_) {}
	MySphere(float x_, float y_, float z_, float r_) :x(x_), y(y_), z(z_), r(r_) {}

	float data[4];
	struct 
	{
		float x;
		float y;
		float z;
		float r;
	};
};

struct MyRobotPos
{
	MyRobotPos() :x(0.0f), y(0.0f), z(0.0f), Rx(0.0f), Ry(0.0f), Rz(0.0f) {}
	MyRobotPos(float x_, float y_, float z_) :x(x_), y(y_), z(z_), Rx(0.0f), Ry(0.0f), Rz(0.0f) {}
	MyRobotPos(float x_, float y_, float z_, float Rx_, float Ry_, float Rz_) :
		x(x_), y(y_), z(z_), Rx(Rx_), Ry(Ry_), Rz(Rz_) {}

	float x;
	float y;
	float z;
	float Rx;
	float Ry;
	float Rz;
};

#define GENERAL_ROTATE(mat,vec,cos_,sin_) \
{\
	mat[0] = cos_ + vec.vx*vec.vx*(1-cos_);\
	mat[1] = -vec.vz*sin_ + vec.vx*vec.vy*(1-cos_);\
	mat[2] = vec.vy*sin_ + vec.vx*vec.vz*(1-cos_);\
	mat[3] = 0;\
	mat[4] = vec.vz*sin_ + vec.vy*vec.vx*(1-cos_);\
	mat[5] = cos_ + vec.vy*vec.vy*(1-cos_);\
	mat[6] = -vec.vx*sin_ + vec.vy*vec.vz*(1-cos_);\
	mat[7] = 0;\
	mat[8] = -vec.vy*sin_ + vec.vz*vec.vx*(1-cos_);\
	mat[9] = vec.vx*sin_ + vec.vz*vec.vy*(1-cos_);\
	mat[10] = cos_ + vec.vz*vec.vz*(1-cos_);\
	mat[11] = 0;\
	mat[12] = 0;\
	mat[13] = 0;\
	mat[14] = 0;\
	mat[15] = 1;\
}

#define NORMAL_ROTATE(mat,vec,cos_,sin_) \
{\
	mat[0] = cos_;\
	mat[1] = -vec.vz*sin_;\
	mat[2] = vec.vy*sin_;\
	mat[3] = 0;\
	mat[4] = vec.vz*sin_;\
	mat[5] = cos_;\
	mat[6] = -vec.vx*sin_;\
	mat[7] = 0;\
	mat[8] = -vec.vy*sin_;\
	mat[9] = vec.vx*sin_;\
	mat[10] = cos_;\
	mat[11] = 0;\
	mat[12] = 0;\
	mat[13] = 0;\
	mat[14] = 0;\
	mat[15] = 1;\
}

enum ConstructionType
{
	CONS_CSYS_BASE = 0,
	CONS_DIRECT_ROT = 1,
	CONS_EULER_ANGLE = 2
};

struct CsysTrans3D
{
	//general construct
	CsysTrans3D() 
	{ 
		memset(mat, 0, 16 * sizeof(float));
		mat[0] = 1;
		mat[5] = 1;
		mat[10] = 1;
		mat[15] = 1;
	}

	//construct base on translation
	CsysTrans3D(const MyVector3D& vec)
	{
		memset(mat, 0, 16 * sizeof(float));
		mat[0] = 1;
		mat[5] = 1;
		mat[10] = 1;
		mat[15] = 1;
		mat[3] = vec.vx;
		mat[7] = vec.vy;
		mat[11] = vec.vz;
	}

	//construct base on rotation
	CsysTrans3D(MyPose3D axis, float angle)
	{
		UNIT_ARRAY_3D(axis.vx, axis.vy, axis.vz);
		normalization(angle, 0, 360);
		float cos_theta = cosf(angle / DEG_PER_RAD);
		float sin_theta = sinf(angle / DEG_PER_RAD);

		GENERAL_ROTATE(mat, axis, cos_theta, sin_theta);
		mat[3] = -axis.x*mat[0] - axis.y*mat[1] - axis.z*mat[2] + axis.x;
		mat[7] = -axis.x*mat[4] - axis.y*mat[5] - axis.z*mat[6] + axis.y;
		mat[11] = -axis.x*mat[8] - axis.y*mat[9] - axis.z*mat[10] + axis.z;
	}

	//construct base on two poses
	CsysTrans3D(MyPose3D srcPos, MyPose3D transPos, int constructionType = CONS_CSYS_BASE)
	{
		memset(mat, 0, 16 * sizeof(float));
		UNIT_ARRAY_3D(srcPos.vx, srcPos.vy, srcPos.vz);
		UNIT_ARRAY_3D(transPos.vx, transPos.vy, transPos.vz);

		switch (constructionType)
		{
		case CONS_CSYS_BASE:
		{
			float R_src[16];
			float R_trans[16];
			memset(R_src, 0, 16 * sizeof(float));
			memset(R_trans, 0, 16 * sizeof(float));

			if (abs(srcPos.vx) > FLOAT_EPSINON || abs(srcPos.vy) > FLOAT_EPSINON)
			{
				//Rotation
				R_src[8] = srcPos.vx;
				R_src[9] = srcPos.vy;
				R_src[10] = srcPos.vz;

				R_src[0] = -R_src[9];
				R_src[1] = R_src[8];
				R_src[2] = 0;
				UNIT_ARRAY_3D(R_src[0], R_src[1], R_src[2]);

				R_src[4] = R_src[1] * R_src[10];
				R_src[5] = -R_src[0] * R_src[10];
				R_src[6] = R_src[0] * R_src[9] - R_src[1] * R_src[8];

				R_src[15] = 1;

				//Translation
				R_src[3] = -srcPos.x*R_src[0] - srcPos.y*R_src[1] - srcPos.z*R_src[2];
				R_src[7] = -srcPos.x*R_src[4] - srcPos.y*R_src[5] - srcPos.z*R_src[6];
				R_src[11] = -srcPos.x*R_src[8] - srcPos.y*R_src[8] - srcPos.z*R_src[10];
			}
			else
			{
				//Identity Matrix
				R_src[0] = 1;
				R_src[5] = 1;
				R_src[10] = 1;
				R_src[15] = 1;

				R_src[3] = -srcPos.x;
				R_src[7] = -srcPos.y;
				R_src[11] = -srcPos.z;
			}

			if (abs(transPos.vx) > FLOAT_EPSINON || abs(transPos.vy) > FLOAT_EPSINON)
			{
				//Rotation
				R_trans[2] = transPos.vx;
				R_trans[6] = transPos.vy;
				R_trans[10] = transPos.vz;

				R_trans[0] = -R_trans[6];
				R_trans[4] = R_trans[2];
				R_trans[8] = 0;
				UNIT_ARRAY_3D(R_trans[0], R_trans[4], R_trans[8]);

				R_trans[1] = R_trans[4] * R_trans[10];
				R_trans[5] = -R_trans[0] * R_trans[10];
				R_trans[9] = R_trans[0] * R_trans[6] - R_trans[2] * R_trans[4];

				R_trans[15] = 1;

				//Translation
				R_trans[3] = transPos.x;
				R_trans[7] = transPos.y;
				R_trans[11] = transPos.z;
			}
			else
			{
				//Identity Matrix
				R_trans[0] = 1;
				R_trans[5] = 1;
				R_trans[10] = 1;
				R_trans[15] = 1;

				R_trans[3] = transPos.x;
				R_trans[7] = transPos.y;
				R_trans[11] = transPos.z;
			}

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					for (int k = 0; k < 4; k++)
						mat[i * 4 + j] += R_trans[i * 4 + k] * R_src[k * 4 + j];

			break;
		}
		case CONS_DIRECT_ROT:
		{
			float cos_theta;
			float sin_theta;
			MyVector3D rot_axis;

			rot_axis.vx = srcPos.vy*transPos.vz - srcPos.vz*transPos.vy;
			rot_axis.vy = srcPos.vz*transPos.vx - srcPos.vx*transPos.vz;
			rot_axis.vz = srcPos.vx*transPos.vy - srcPos.vy*transPos.vx;
			cos_theta = srcPos.vx*transPos.vx + srcPos.vy*transPos.vy + srcPos.vz*transPos.vz;
			sin_theta = MOD_VECTOR_3D(rot_axis);
			UNIT_VECTOR_3D(rot_axis);
			GENERAL_ROTATE(mat, rot_axis, cos_theta, sin_theta);
			mat[3] = -srcPos.x*mat[0] - srcPos.y*mat[1] - srcPos.z*mat[2] + transPos.x;
			mat[7] = -srcPos.x*mat[4] - srcPos.y*mat[5] - srcPos.z*mat[6] + transPos.y;
			mat[11] = -srcPos.x*mat[8] - srcPos.y*mat[9] - srcPos.z*mat[10] + transPos.z;
			mat[15] = 1;
			break;
		}	
		case CONS_EULER_ANGLE:
		{
			float R0[16];
			float R1[16];
			float ux, uy;		//source vector project to the latitude plane
			float uxy, uz;		//source vector project to the longitude plane
			float vx, vy;		//transform vector project to the latitude plane
			float vxy, vz;		//transform vector project to the longitude plane
			MyVector3D rot_axis0;
			float cos_theta0;
			float sin_theta0;
			MyVector3D rot_axis1;
			float cos_theta1;
			float sin_theta1;

			if (abs(transPos.vx) < FLOAT_EPSINON && abs(transPos.vy) < FLOAT_EPSINON)
			{
				rot_axis1 = { srcPos.vy,-srcPos.vx,0 };
				UNIT_VECTOR_3D(rot_axis1);
				uxy = sqrt(srcPos.vx*srcPos.vx + srcPos.vy*srcPos.vy);
				uz = srcPos.vz;
				UNIT_ARRAY_2D(uxy, uz);
				cos_theta1 = uz;
				sin_theta1 = uxy;
				GENERAL_ROTATE(mat, rot_axis1, cos_theta1, sin_theta1);
			}
			else
			{
				rot_axis0 = { 0,0,1 };
				ux = srcPos.vx;
				uy = srcPos.vy;
				UNIT_ARRAY_2D(ux, uy);
				vx = transPos.vx;
				vy = transPos.vy;
				UNIT_ARRAY_2D(vx, vy);
				cos_theta0 = ux*vx + uy*vy;
				sin_theta0 = ux*vy - uy*vx;
				memset(R0, 0, 16 * sizeof(float));
				R0[0] = cos_theta0;
				R0[1] = -sin_theta0;
				R0[4] = sin_theta0;
				R0[5] = cos_theta0;
				R0[10] = 1;
				R0[15] = 1;

				rot_axis1 = { transPos.vy,-transPos.vx,0 };
				UNIT_VECTOR_3D(rot_axis1);
				uxy = sqrt(srcPos.vx*srcPos.vx + srcPos.vy*srcPos.vy);
				uz = srcPos.vz;
				UNIT_ARRAY_2D(uxy, uz);
				vxy = sqrt(transPos.vx*transPos.vx + transPos.vy*transPos.vy);
				vz = transPos.vz;
				UNIT_ARRAY_2D(vxy, vz);
				cos_theta1 = uxy*vxy + uz*vz;
				sin_theta1 = uxy*vz - uz*vxy;
				GENERAL_ROTATE(R1, rot_axis1, cos_theta1, sin_theta1);

				for (int i = 0; i < 4; i++)
					for (int j = 0; j < 4; j++)
						for (int k = 0; k < 4; k++)
							mat[i * 4 + j] += R1[i * 4 + k] * R0[k * 4 + j];
			}

			mat[3] = -srcPos.x*mat[0] - srcPos.y*mat[1] - srcPos.z*mat[2] + transPos.x;
			mat[7] = -srcPos.x*mat[4] - srcPos.y*mat[5] - srcPos.z*mat[6] + transPos.y;
			mat[11] = -srcPos.x*mat[8] - srcPos.y*mat[9] - srcPos.z*mat[10] + transPos.z;
			mat[15] = 1;
			break;
		}
		default:
			break;
		}
	}

	//construct base on two vectors
	CsysTrans3D(const MyVector3D& srcVec, const MyVector3D& transVec, int constructionType = CONS_CSYS_BASE)
	{
		MyPose3D src_pos = { 0, 0, 0, srcVec.vx, srcVec.vy, srcVec.vz };
		MyPose3D trans_pos = { 0, 0, 0, transVec.vx, transVec.vy, transVec.vz };
		CsysTrans3D trans(src_pos, trans_pos, constructionType);
		memcpy(mat, trans.mat, 16 * sizeof(float));
	}

	CsysTrans3D& operator=(const CsysTrans3D& trans)
	{
		if (this != &trans)
			memcpy(mat, trans.mat, 16 * sizeof(float));
		return *this;
	}

	CsysTrans3D operator*(const CsysTrans3D& trans)
	{
		CsysTrans3D rst;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				int idx = i * 4 + j;
				rst.mat[idx] = 0;
				for (int k = 0; k < 4; k++)
					rst.mat[idx] += mat[i * 4 + k] * trans.mat[k * 4 + j];
			}
		}
		return rst;
	}

	float mat[16];	//left multiplication matrix
};

struct CsysTrans2D
{
	CsysTrans2D()
	{
		memset(mat, 0, 9 * sizeof(float));
		mat[0] = 1.0f;
		mat[4] = 1.0f;
		mat[8] = 1.0f;

		scale_x = 1.0f;
		scale_y = 1.0f;
		shear = 0.0f;
		rotation = 0.0f;
		trans_x = 0.0f;
		trans_y = 0.0f;
	}

	CsysTrans2D(float mat_[])
	{
		memcpy(mat, mat_, 9 * sizeof(float));
		float a = mat[0];
		float b = mat[1];
		float c = mat[2];
		float d = mat[3];
		float e = mat[4];
		float f = mat[5];

		trans_x = c;
		trans_y = f;
		scale_y = sqrtf(b*b + e*e);
		float sin_theta = -b / scale_y;
		float cos_theta = e / scale_y;
		rotation = atan2f(sin_theta, cos_theta)*DEG_PER_RAD;
		scale_x = a*cos_theta + d*sin_theta;
		float tan_phi = (cos_theta - a / scale_x) / sin_theta;
		shear = atanf(tan_phi)*DEG_PER_RAD;
	}

	CsysTrans2D(float scale_x_, float scale_y_, float shear_, float rotation_, float trans_x_, float trans_y_)
	{

		scale_x = scale_x_;
		scale_y = scale_y_;
		shear = shear_;
		normalization(shear, -90, 90);
		rotation = rotation_;
		normalization(rotation, -180, 180);
		trans_x = trans_x_;
		trans_y = trans_y_;

		float cos_theta = cosf(rotation / DEG_PER_RAD);
		float sin_theta = sinf(rotation / DEG_PER_RAD);
		float tan_phi = tanf(shear / DEG_PER_RAD);
		mat[0] = scale_x*(cos_theta - sin_theta*tan_phi);
		mat[1] = -scale_y*sin_theta;
		mat[2] = trans_x;
		mat[3] = scale_x*(sin_theta + cos_theta*tan_phi);
		mat[4] = scale_y*cos_theta;
		mat[5] = trans_y;
		mat[6] = 0.0f;
		mat[7] = 0.0f;
		mat[8] = 1.0f;
	}

	CsysTrans2D(const MyVector2D& vec)
	{
		mat[0] = 1.0f;
		mat[1] = 0;
		mat[2] = vec.vx;
		mat[3] = 0;
		mat[4] = 1.0f;
		mat[5] = vec.vy;
		mat[6] = 0;
		mat[7] = 0;
		mat[8] = 1.0f;

		scale_x = 1.0f;
		scale_y = 1.0f;
		shear = 0.0f;
		rotation = 0.0f;
		trans_x = mat[2];
		trans_y = mat[5];
	}

	CsysTrans2D(const MyPoint2D& srcPnt, const MyPoint2D& tgtPnt)
	{
		mat[0] = 1.0f;
		mat[1] = 0;
		mat[2] = tgtPnt.x - srcPnt.x;
		mat[3] = 0;
		mat[4] = 1.0f;
		mat[5] = tgtPnt.y - srcPnt.y;
		mat[6] = 0;
		mat[7] = 0;
		mat[8] = 1.0f;

		scale_x = 1.0f;
		scale_y = 1.0f;
		shear = 0.0f;
		rotation = 0.0f;
		trans_x = mat[2];
		trans_y = mat[5];
	}

	CsysTrans2D(const MyPoint2D& rotCenter, float rotAngle)
	{
		normalization(rotAngle, -180, 180);
		float cos_theta = cosf(rotAngle / DEG_PER_RAD);
		float sin_theta = sinf(rotAngle / DEG_PER_RAD);
		float x = rotCenter.x;
		float y = rotCenter.y;

		mat[0] = cos_theta;
		mat[1] = -sin_theta;
		mat[2] = x - x*cos_theta + y*sin_theta;
		mat[3] = sin_theta;
		mat[4] = cos_theta;
		mat[5] = y - x*sin_theta - y*cos_theta;
		mat[6] = 0;
		mat[7] = 0;
		mat[8] = 1;

		scale_x = 1.0f;
		scale_y = 1.0f;
		shear = 0.0f;
		rotation = rotAngle;
		trans_x = mat[2];
		trans_y = mat[5];
	}

	CsysTrans2D(const MyPose2D& srcPos, const MyPose2D& tgtPos)
	{
		float cos_theta = srcPos.vx*tgtPos.vx + srcPos.vy*tgtPos.vy;
		float sin_theta = tgtPos.vy*srcPos.vx - srcPos.vy*tgtPos.vx;
		float inter_x = srcPos.x*cos_theta - srcPos.y*sin_theta;
		float inter_y = srcPos.x*sin_theta + srcPos.y*cos_theta;
		float tx = tgtPos.x - inter_x;
		float ty = tgtPos.y - inter_y;

		mat[0] = cos_theta;
		mat[1] = -sin_theta;
		mat[2] = tx;
		mat[3] = sin_theta;
		mat[4] = cos_theta;
		mat[5] = ty;
		mat[6] = 0;
		mat[7] = 0;
		mat[8] = 1.0f;

		scale_x = 1.0f;
		scale_y = 1.0f;
		shear = 0.0f;
		rotation = atan2f(sin_theta, cos_theta)*DEG_PER_RAD;
		trans_x = mat[2];
		trans_y = mat[5];
	}

	CsysTrans2D(const MyRobotPos& srcPos, const MyRobotPos& tgtPos)
	{
		*this = CsysTrans2D(MyPose2D(srcPos.x, srcPos.y, srcPos.Rz), MyPose2D(tgtPos.x, tgtPos.y, tgtPos.Rz));	
	}

	CsysTrans2D& operator=(const CsysTrans2D& trans)
	{
		if (this != &trans)
		{
			memcpy(mat, trans.mat, 9 * sizeof(float));
			scale_x = trans.scale_x;
			scale_y = trans.scale_y;
			shear = trans.shear;
			rotation = trans.rotation;
			trans_x = trans.trans_x;
			trans_y = trans.trans_y;
		}
		return *this;
	}

	CsysTrans2D operator*(const CsysTrans2D& trans)
	{
		float mat_[9];
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int idx = i * 3 + j;
				mat_[idx] = 0;
				for (int k = 0; k < 3; k++)
					mat_[idx] += mat[i * 3 + k] * trans.mat[k * 3 + j];
			}
		}
		return CsysTrans2D(mat_);
	}

	float mat[9];
	float scale_x;
	float scale_y;
	float shear;		//-90~90
	float rotation;		//-180~180
	float trans_x;
	float trans_y;
};