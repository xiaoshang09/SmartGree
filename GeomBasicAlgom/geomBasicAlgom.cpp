#include "geomBasicAlgom.h"

int istLN(const MyLine3D& line, const MyPlane3D& plane, MyPoint3D& pnt)
{
	float x0 = line.x;
	float y0 = line.y;
	float z0 = line.z;
	float vx = line.vx;
	float vy = line.vy;
	float vz = line.vz;
	float A = plane.A;
	float B = plane.B;
	float C = plane.C;
	float D = plane.D;

	float delta = A*vx + B*vy + C*vz;
	if (abs(delta) < FLOAT_EPSINON)
	{
		pnt.x = 0;
		pnt.y = 0;
		pnt.z = 0;
		return -1;
	}
	else
	{
		float t = -(A*x0 + B*y0 + C*z0 + D) / delta;
		pnt.x = x0 + vx*t;
		pnt.y = y0 + vy*t;
		pnt.z = z0 + vz*t;
		return 1;
	}
}

float distPP(const MyPoint3D& pnt0, const MyPoint3D& pnt1)
{
	float dx = pnt1.x - pnt0.x;
	float dy = pnt1.y - pnt0.y;
	float dz = pnt1.z - pnt0.z;
	return sqrtf(dx*dx + dy*dy + dz*dz);
}

float distPL(const MyPoint3D& pnt, const MyLine3D& line, MyPoint3D& footPnt)
{
	float x = pnt.x;
	float y = pnt.y;
	float z = pnt.z;
	float x0 = line.x;
	float y0 = line.y;
	float z0 = line.z;
	float vx = line.vx;
	float vy = line.vy;
	float vz = line.vz;
	float& xf = footPnt.x;
	float& yf = footPnt.y;
	float& zf = footPnt.z;

	float ux = x - x0;
	float uy = y - y0;
	float uz = z - z0;
	float len = vx*ux + vy*uy + vz*uz;
	xf = x0 + vx*len;
	yf = y0 + vy*len;
	zf = z0 + vz*len;

	return distPP(pnt, footPnt);
}

float distPN(const MyPoint3D& pnt, const MyPlane3D& plane, MyPoint3D& footPnt, int distType)
{
	float x = pnt.x;
	float y = pnt.y;
	float z = pnt.z;
	float A = plane.A;
	float B = plane.B;
	float C = plane.C;
	float D = plane.D;
	float dist = 0.0f;

	float f = A*x + B*y + C*z + D;
	float delta = A*A + B*B + C*C;
	if (delta < FLOAT_EPSINON)
		return 0;

	if (0 == distType)
		dist = abs(f) / sqrt(delta);
	else
		dist = f / sqrt(delta);

	footPnt.x = x - A*f / delta;
	footPnt.y = y - B*f / delta;
	footPnt.z = z - C*f / delta;

	return dist;
}

float areaTriangle(const MyPoint3D& pnt0, const MyPoint3D& pnt1, const MyPoint3D& pnt2)
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

	/*------------------------------------------

			|    i      j      k    |
			|                       |
			|  x1-x0  y1-y0  z1-z0  |
			|                       |
			|  x2-x0  y2-y0  z2-z0  |

	------------------------------------------*/


	float _i = (y1 - y0)*(z2 - z0) - (z1 - z0)*(y2 - y0);
	float _j = (z1 - z0)*(x2 - x0) - (x1 - x0)*(z2 - z0);
	float _k = (x1 - x0)*(y2 - y0) - (y1 - y0)*(x2 - x0);
	return sqrt(_i*_i + _j*_j + _k*_k) / 2;
}

int plane3P(const MyPoint3D& pnt0, const MyPoint3D& pnt1, const MyPoint3D& pnt2, MyPlane3D& plane)
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

	/*first judge if the three points are on the same line

				|    i      j      k    |
				|                       |
				|  x1-x0  y1-y0  z1-z0  |
				|                       |
				|  x2-x0  y2-y0  z2-z0  |

	---------------------------------------------------------*/

	float _i = (y1 - y0)*(z2 - z0) - (z1 - z0)*(y2 - y0);
	float _j = (z1 - z0)*(x2 - x0) - (x1 - x0)*(z2 - z0);
	float _k = (x1 - x0)*(y2 - y0) - (y1 - y0)*(x2 - x0);

	if (abs(_i) < FLOAT_EPSINON && abs(_j) < FLOAT_EPSINON && abs(_k) < FLOAT_EPSINON)
		return -1;

	/*second judge if the plane is through the origin

				--                  --
				|  x0  y0  z0  |  1  |
				|			   |     |
				|  x1  y1  z1  |  1  |
				|              |     |
				|  x2  y2  z2  |  1  |
				--                  --

	Cramer Rule-------------------------------------*/

	float Det = x0*y1*z2 + x1*y2*z0 + x2*y0*z1 - x2*y1*z0 - x1*y0*z2 - x0*y2*z1;

	if (abs(Det) < FLOAT_EPSINON)
	{
		plane.A = (z0*y1 - z1*y0) / (x0*y1 - x1*y0);
		plane.B = (x0*z1 - x1*z0) / (x0*y1 - x1*y0);
		plane.C = -1;
		plane.D = 0;
		return 0;
	}

	float Det0 = y1*z2 + y2*z0 + y0*z1 - y1*z0 - y0*z2 - y2*z1;
	float Det1 = x0*z2 + x1*z0 + x2*z1 - x2*z0 - x1*z2 - x0*z1;
	float Det2 = x0*y1 + x1*y2 + x2*y0 - x2*y1 - x1*y0 - x0*y2;

	plane.A = Det0 / Det;
	plane.B = Det1 / Det;
	plane.C = Det2 / Det;
	plane.D = -1;
	return 1;
}

float angleVV(MyVector3D vec0, MyVector3D vec1, bool check)
{
	if (check)
	{
		UNIT_VECTOR_3D(vec0);
		UNIT_VECTOR_3D(vec1);
	}
	float cos_theta = vec0.vx*vec1.vx + vec0.vy*vec1.vy + vec0.vz*vec1.vz;
	if (cos_theta > 1)
		return 0.0f;
	else if (cos_theta < -1)
		return 180.0f;
	return acosf(cos_theta) * DEG_PER_RAD;
}

float angleLL(MyLine3D line0, MyLine3D line1)
{
	MyVector3D vec0(line0.vx, line0.vy, line0.vz);
	MyVector3D vec1(line1.vx, line1.vy, line1.vz);
	return angleVV(vec0, vec1, false);
}

int angleAxisVV(MyVector3D axis, MyVector3D vec0, MyVector3D vec1, float& angle, bool check)
{
	if (check)
	{
		UNIT_VECTOR_3D(axis);
		UNIT_VECTOR_3D(vec0);
		UNIT_VECTOR_3D(vec1);
	}

	float nx0 = vec0.vy*axis.vz - vec0.vz*axis.vy;
	float ny0 = vec0.vz*axis.vx - vec0.vx*axis.vz;
	float nz0 = vec0.vx*axis.vy - vec0.vy*axis.vx;
	float nx1 = vec1.vy*axis.vz - vec1.vz*axis.vy;
	float ny1 = vec1.vz*axis.vx - vec1.vx*axis.vz;
	float nz1 = vec1.vx*axis.vy - vec1.vy*axis.vx;
	float sin_theta = (ny0*nz1 - nz0*ny1)*axis.vx + (nz0*nx1 - nx0*nz1)*axis.vy + (nx0*ny1 - ny0*nx1)*axis.vz;
	float cos_theta = nx0*nx1 + ny0*ny1 + nz0*nz1;
	angle = atan2f(sin_theta, cos_theta) * DEG_PER_RAD;

	float d0 = axis.vx*vec0.vx + axis.vy*vec0.vy + axis.vz*vec0.vz;
	float d1 = axis.vx*vec1.vx + axis.vy*vec1.vy + axis.vz*vec1.vz;

	if (abs(d0 - d1) > FLOAT_EPSINON)
		return -1;
	else 
		return 1;

// 	float vx0 = vec0.vx - axis.vx*d0;
// 	float vy0 = vec0.vy - axis.vy*d0;
// 	float vz0 = vec0.vz - axis.vz*d0;
// 	float vx1 = vec1.vx - axis.vx*d1;
// 	float vy1 = vec1.vy - axis.vy*d1;
// 	float vz1 = vec1.vz - axis.vz*d1;
// 	NORMAL_ARRAY_3D(vx0, vy0, vz0);
// 	NORMAL_ARRAY_3D(vx1, vy1, vz1);
// 	float sin_theta = (vy0*vz1 - vz0*vy1)*axis.vx + (vz0*vx1 - vx0*vz1)*axis.vy + (vx0*vy1 - vy0*vx1)*axis.vz;
// 	float cos_theta = vx0*vx1 + vy0*vy1 + vz0*vz1;
// 	angle = atan2f(sin_theta, cos_theta) * 180 / CV_PI;

}

int csysTrans3D(const MyPoint3D& srcPnt, MyPoint3D& transPnt, const CsysTrans3D& transform)
{
	if (transform.mat == nullptr)
		return -1;

	transPnt.x = srcPnt.x*transform.mat[0] + srcPnt.y*transform.mat[1] + srcPnt.z*transform.mat[2] + transform.mat[3];
	transPnt.y = srcPnt.x*transform.mat[4] + srcPnt.y*transform.mat[5] + srcPnt.z*transform.mat[6] + transform.mat[7];
	transPnt.z = srcPnt.x*transform.mat[8] + srcPnt.y*transform.mat[9] + srcPnt.z*transform.mat[10] + transform.mat[11];
	return 1;
}

int csysTrans3D(const MyVector3D& srcVec, MyVector3D& transVec, const CsysTrans3D& transform)
{
	if (transform.mat == nullptr)
		return -1;

	transVec.vx = srcVec.vx*transform.mat[0] + srcVec.vy*transform.mat[1] + srcVec.vz*transform.mat[2];
	transVec.vy = srcVec.vx*transform.mat[4] + srcVec.vy*transform.mat[5] + srcVec.vz*transform.mat[6];
	transVec.vz = srcVec.vx*transform.mat[8] + srcVec.vy*transform.mat[9] + srcVec.vz*transform.mat[10];
	return 1;
}

int csysTrans3D(const MyPose3D& srcPos, MyPose3D& transPos, const CsysTrans3D& transform)
{
	if (transform.mat == nullptr)
		return -1;

	transPos.x = srcPos.x*transform.mat[0] + srcPos.y*transform.mat[1] + srcPos.z*transform.mat[2] + transform.mat[3];
	transPos.y = srcPos.x*transform.mat[4] + srcPos.y*transform.mat[5] + srcPos.z*transform.mat[6] + transform.mat[7];
	transPos.z = srcPos.x*transform.mat[8] + srcPos.y*transform.mat[9] + srcPos.z*transform.mat[10] + transform.mat[11];
	transPos.vx = srcPos.vx*transform.mat[0] + srcPos.vy*transform.mat[1] + srcPos.vz*transform.mat[2];
	transPos.vy = srcPos.vx*transform.mat[4] + srcPos.vy*transform.mat[5] + srcPos.vz*transform.mat[6];
	transPos.vz = srcPos.vx*transform.mat[8] + srcPos.vy*transform.mat[9] + srcPos.vz*transform.mat[10];
	return 1;
}


//2D Algorithm

int istLL(const MyLine2D& line0, const MyLine2D& line1, MyPoint2D& pnt)
{
	float x0 = line0.x;
	float y0 = line0.y;
	float vx0 = line0.vx;
	float vy0 = line0.vy;
	float x1 = line1.x;
	float y1 = line1.y;
	float vx1 = line1.vx;
	float vy1 = line1.vy;

	float delta = vx0*vy1 - vy0*vx1;
	if (fabs(delta) < FLOAT_EPSINON)
		return 0;

	float t = ((x1 - x0)*vy1 - (y1 - y0)*vx1) / delta;
	pnt.x = x0 + vx0*t;
	pnt.y = y0 + vy0*t;
	return 1;
}

float distPP(const MyPoint2D& pnt0, const MyPoint2D& pnt1)
{
	return sqrt((pnt1.x - pnt0.x)*(pnt1.x - pnt0.x) + (pnt1.y - pnt0.y)*(pnt1.y - pnt0.y));
}

float distPL(const MyPoint2D& pnt, const MyLine2D& line, int distType, MyPoint2D& footPnt)
{
	float dx = pnt.x - line.x;
	float dy = pnt.y - line.y;
	float v = line.vx*dx + line.vy*dy;
	footPnt.x = line.x + v*line.vx;
	footPnt.y = line.y + v*line.vy;
	float dist = dx*(-line.vy) + dy*line.vx;
	if (distType == 0)
		return fabs(dist);
	else
		return dist;
}

float distP2PPL(const MyPoint2D& pnt0, const MyPoint2D& pnt1, const MyPoint2D& pnt2)
{
	float area = areaTriangle(pnt0, pnt1, pnt2);
	float length = distPP(pnt1, pnt2);
	if (length < FLOAT_EPSINON)
		return distPP(pnt0, pnt1);
	else
		return area * 2 / length;
}

float areaTriangle(const MyPoint2D& pnt0, const MyPoint2D& pnt1, const MyPoint2D& pnt2)
{
	float x0 = pnt0.x;
	float y0 = pnt0.y;
	float x1 = pnt1.x;
	float y1 = pnt1.y;
	float x2 = pnt2.x;
	float y2 = pnt2.y;

	/*---------------------------------

			|  x0  y0  1  |
			|             |
			|  x1  y1  1  |
			|             |
			|  x2  y2  1  |

	---------------------------------*/

	float det = x0*y1 + x1*y2 + x2*y0 - x0*y2 - x1*y0 - x2*y1;
	return abs(det) / 2;
}

float areaQuadrangle(const MyPoint2D& pnt0, const MyPoint2D& pnt1, const MyPoint2D& pnt2, const MyPoint2D& pnt3)
{
	return areaTriangle(pnt0, pnt1, pnt2) + areaTriangle(pnt0, pnt3, pnt2);
}

float angleVV(MyVector2D vec0, MyVector2D vec1, int angleType)
{
	float cos_theta;
	float sin_theta;
	float theta = 0.0f;

	UNIT_VECTOR_2D(vec0);
	UNIT_VECTOR_2D(vec1);
	cos_theta = vec0.vx*vec1.vx + vec0.vy*vec1.vy;
	sin_theta = vec0.vx*vec1.vy - vec0.vy*vec1.vx;

	if (angleType == 1)
		theta = acosf(cos_theta) * DEG_PER_RAD;
	else if (angleType == 2)
		theta = atan2(sin_theta, cos_theta) * DEG_PER_RAD;

	return theta;
}

float angleVV(MyPose2D pos0, MyPose2D pos1, int angleType)
{
	MyVector2D vec0(pos0.vx, pos0.vy);
	MyVector2D vec1(pos1.vx, pos1.vy);
	return angleVV(vec0, vec1, angleType);
}

float angleLL(const MyLine2D& line0, const MyLine2D& line1, int angleType)
{
	MyVector2D vec0(line0.vx, line0.vy);
	MyVector2D vec1(line1.vx, line1.vy);
	if (angleType == 0)
	{
		float angle = angleVV(vec0, vec1, 1);
		if (angle > 90)
			return 180 - angle;
		else
			return angle;
	}
	else
		return angleVV(vec0, vec1, angleType);
}

void rotRect4P(const MyRotRect& rotRect, MyPoint2D pnt[])
{
	MyPoint2D center(rotRect.x, rotRect.y);
	MyPoint2D pnt_[4];
	pnt_[0] = { rotRect.x - rotRect.w / 2,rotRect.y - rotRect.h / 2 };
	pnt_[1] = { rotRect.x + rotRect.w / 2,rotRect.y - rotRect.h / 2 };
	pnt_[2] = { rotRect.x + rotRect.w / 2,rotRect.y + rotRect.h / 2 };
	pnt_[3] = { rotRect.x - rotRect.w / 2,rotRect.y + rotRect.h / 2 };

	for (int i = 0; i < 4; i++)
		rotationTrans2D(center, pnt_[i], pnt[i], rotRect.angle);
}

MyRect boundRect(const MyRotRect& rotRect)
{
	MyRect rect;
	MyPoint2D pnt[4];
	rotRect4P(rotRect, pnt);
	float angle = rotRect.angle;
	normalization(angle, 0, 360);
	int flag = (int)angle / 90;

	float top = pnt[(4 - flag) % 4].y;
	float right = pnt[(5 - flag) % 4].x;
	float bottom = pnt[(6 - flag) % 4].y;
	float left = pnt[(7 - flag) % 4].x;

	return MyRect(left, top, right - left, bottom - top);
}

void rectNinePalace(const MyRotRect& rotRect, MyPoint2D pnt[])
{

	/*```````````````````````````````````````````````````````

	0			1			2
	*-----------*-----------*
	|			|			|
	|			|			|
	|			|			|
	|3			|4			|5
	*-----------*-----------*
	|			|			|
	|			|			|
	|			|			|
	|			|			|
	*-----------*-----------*
	6			7			8

	````````````````````````````````````````````````````````*/

	MyPoint2D pnt_[9];
	pnt_[0].x=rotRect.x - rotRect.w / 2;
	pnt_[0].y=rotRect.y - rotRect.h / 2;
	pnt_[1].x=rotRect.x;
	pnt_[1].y=rotRect.y - rotRect.h / 2;
	pnt_[2].x=rotRect.x + rotRect.w / 2;
	pnt_[2].y=rotRect.y - rotRect.h / 2;
	pnt_[3].x=rotRect.x - rotRect.w / 2;
	pnt_[3].y=rotRect.y;
	pnt_[4].x=rotRect.x;
	pnt_[4].y=rotRect.y;
	pnt_[5].x=rotRect.x + rotRect.w / 2;
	pnt_[5].y=rotRect.y;
	pnt_[6].x=rotRect.x - rotRect.w / 2;
	pnt_[6].y=rotRect.y + rotRect.h / 2;
	pnt_[7].x=rotRect.x;
	pnt_[7].y=rotRect.y + rotRect.h / 2;
	pnt_[8].x=rotRect.x + rotRect.w / 2;
	pnt_[8].y=rotRect.y + rotRect.h / 2;

	float rot_angle = rotRect.angle;
	normalization(rot_angle, -180, 180);
	float cos_theta = cosf(rot_angle / DEG_PER_RAD);
	float sin_theta = sinf(rot_angle / DEG_PER_RAD);
	float x0 = rotRect.x;
	float y0 = rotRect.y;
	for (int i = 0; i < 9; i++)
	{
		float x_ = (pnt_[i].x - x0)*cos_theta - (pnt_[i].y - y0)*sin_theta + x0;
		float y_ = (pnt_[i].x - x0)*sin_theta + (pnt_[i].y - y0)*cos_theta + y0;
		pnt[i].x = x_;
		pnt[i].y = y_;
	}
}

vector<vector<MyRotRect>> rectArrayItems(const MyRectArray& rectArray)
{
	vector<vector<MyRotRect>> item_rects(rectArray.rows, vector<MyRotRect>(rectArray.cols));
	float w_x(0.0f), w_y(0.0f), h_x(0.0f), h_y(0.0f);
	int idx = rectArray.rows > 1 ? rectArray.cols > 1 ? 0 : 1 : rectArray.cols > 1 ? 3 : 4;
	MyPoint2D layout_ninepnt[9];
	rectNinePalace(rectArray.layout_rect, layout_ninepnt);
	if (rectArray.rows > 1)
	{
		h_x = (layout_ninepnt[6].x - layout_ninepnt[0].x) / (rectArray.rows - 1);
		h_y = (layout_ninepnt[6].y - layout_ninepnt[0].y) / (rectArray.rows - 1);
	}
	if (rectArray.cols > 1)
	{
		w_x = (layout_ninepnt[2].x - layout_ninepnt[0].x) / (rectArray.cols - 1);
		w_y = (layout_ninepnt[2].y - layout_ninepnt[0].y) / (rectArray.cols - 1);
	}
	for (int i = 0; i < rectArray.rows; i++)
	{
		for (int j = 0; j < rectArray.cols; j++)
		{
			item_rects[i][j].x = layout_ninepnt[idx].x + h_x*i + w_x*j;
			item_rects[i][j].y = layout_ninepnt[idx].y + h_y*i + w_y*j;
			item_rects[i][j].w = rectArray.item_width;
			item_rects[i][j].h = rectArray.item_height;
			item_rects[i][j].angle = rectArray.item_angle;
		}
	}
	return item_rects;
}

bool lineSplitPnts(const MyLine2D& line, const MyPoint2D& pnt0, const MyPoint2D& pnt1)
{
	float v0 = line.vx*(pnt0.y - line.y) - line.vy*(pnt0.x - line.x);
	float v1 = line.vx*(pnt1.y - line.y) - line.vy*(pnt1.x - line.x);
	return v0*v1 < 0;
}

bool rectContainPnt(const MyRotRect& rotRect, const MyPoint2D& pnt)
{
	float vx = cosf(rotRect.angle / DEG_PER_RAD);
	float vy = sinf(rotRect.angle / DEG_PER_RAD);
	float hx = rotRect.h / 2.0f * vy;
	float hy = rotRect.h / 2.0f * vx;
	float wx = rotRect.w / 2.0f * vx;
	float wy = rotRect.w / 2.0f * vy;
	MyPoint2D center(rotRect.x, rotRect.y);
	MyLine2D up_line(rotRect.x + hx, rotRect.y - hy, vx, vy);
	MyLine2D down_line(rotRect.x - hx, rotRect.y + hy, -vx, -vy);
	MyLine2D left_line(rotRect.x - wx, rotRect.y - wy, vy, -vx);
	MyLine2D right_line(rotRect.x + wx, rotRect.y + wy, -vy, vx);
	return !lineSplitPnts(up_line, center, pnt) && !lineSplitPnts(down_line, center, pnt) &&
		!lineSplitPnts(left_line, center, pnt) && !lineSplitPnts(right_line, center, pnt);
}

int csysTrans2D(const MyPoint2D& srcPnt, MyPoint2D& transPnt, const CsysTrans2D& transform)
{
	if (transform.mat == nullptr)
		return -1;

	transPnt.x = srcPnt.x*transform.mat[0] + srcPnt.y*transform.mat[1] + transform.mat[2];
	transPnt.y = srcPnt.x*transform.mat[3] + srcPnt.y*transform.mat[4] + transform.mat[5];
	return 1;
}

int csysTrans2D(const MyVector2D& srcVec, MyVector2D& transVec, const CsysTrans2D& transform)
{
	if (transform.mat == nullptr)
		return -1;

	transVec.vx = srcVec.vx*transform.mat[0] + srcVec.vy*transform.mat[1];
	transVec.vy = srcVec.vx*transform.mat[3] + srcVec.vy*transform.mat[4];
	UNIT_VECTOR_2D(transVec);
	return 1;
}

int csysTrans2D(const MyPose2D& srcPos, MyPose2D& transPos, const CsysTrans2D& transform)
{
	if (transform.mat == nullptr)
		return -1;

	transPos.x = srcPos.x*transform.mat[0] + srcPos.y*transform.mat[1] + transform.mat[2];
	transPos.y = srcPos.x*transform.mat[3] + srcPos.y*transform.mat[4] + transform.mat[5];
	transPos.vx = srcPos.vx*transform.mat[0] + srcPos.vy*transform.mat[1];
	transPos.vy = srcPos.vx*transform.mat[3] + srcPos.vy*transform.mat[4];
	UNIT_ARRAY_2D(transPos.vx, transPos.vy);
	return 1;
}

int rotationTrans2D(const MyPoint2D& rotCenter, const MyPoint2D& srcPnt, MyPoint2D& transPnt, float rotAngle)
{
	normalization(rotAngle, -180, 180);
	float cos_theta = cosf(rotAngle / DEG_PER_RAD);
	float sin_theta = sinf(rotAngle / DEG_PER_RAD);
	float x0 = rotCenter.x;
	float y0 = rotCenter.y;
	float x1 = srcPnt.x;
	float y1 = srcPnt.y;
	transPnt.x = (x1 - x0)*cos_theta - (y1 - y0)*sin_theta + x0;
	transPnt.y = (x1 - x0)*sin_theta + (y1 - y0)*cos_theta + y0;
	return 1;
}

int rotationTrans2D(const MyPoint2D& rotCenter, const MyPose2D& srcPos, MyPose2D& transPos, float rotAngle)
{
	normalization(rotAngle, -180, 180);
	MyPoint2D src_pnt(srcPos.x, srcPos.y);
	MyPoint2D tran_pnt;
	rotationTrans2D(rotCenter, src_pnt, tran_pnt, rotAngle);
	transPos.x = tran_pnt.x;
	transPos.y = tran_pnt.y;
	float cos_theta = cosf(rotAngle / DEG_PER_RAD);
	float sin_theta = sinf(rotAngle / DEG_PER_RAD);
	transPos.vx = srcPos.vx*cos_theta - srcPos.vy*sin_theta;
	transPos.vy = srcPos.vy*cos_theta + sin_theta*srcPos.vx;
	return 1;
}

int rotationTrans2D(const MyVector2D& srcVec, MyVector2D& transVec, float rotAngle)
{
	float cos_theta = cosf(rotAngle / DEG_PER_RAD);
	float sin_theta = sinf(rotAngle / DEG_PER_RAD);
	transVec.vx = srcVec.vx*cos_theta - srcVec.vy*sin_theta;
	transVec.vy = srcVec.vx*sin_theta + srcVec.vy*cos_theta;
	return 1;
}