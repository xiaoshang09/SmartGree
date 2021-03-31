#pragma once
#include "MyDataType.h"

#ifdef GEOMBASICALGOM_EXPORTS
#define GEOMBASICALGOM_API __declspec(dllexport)
#else
#define GEOMBASICALGOM_API __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"GeomBasicAlgomD.lib")
#else
#pragma comment(lib,"GeomBasicAlgom.lib")
#endif
#endif

/**********************************************************************************************

---------- abbreviation ----------
P: Point
L: Line
N: Plane
V: Vector
C: Circle
R: Rectangle
S: Sphere
ist: intersection
dist: distance
pnt: point
vec: vector
pos: pose
area: area
angle: angle
plane: plane

---------- illustration ----------

int istLN(const MyLine3D& line, const MyPlane3D& plane, MyPoint3D& pnt);
    |||||                 ||||                    ||||             ||||
intersection          input a line           input a plane     output a point
    line
    plane

so this function means to calculate the intersection intersected by a line with a plane

***********************************************************************************************/



//**************************************** 3D Algorithm ****************************************

//----------------- point / intersection ----------------

int GEOMBASICALGOM_API istLN(const MyLine3D& line, const MyPlane3D& plane, MyPoint3D& pnt);



//------------------ line / distance -------------------- 

float GEOMBASICALGOM_API distPP(const MyPoint3D& pnt0, const MyPoint3D& pnt1);

float GEOMBASICALGOM_API distPL(const MyPoint3D& pnt, const MyLine3D& line, MyPoint3D& footPnt = MyPoint3D());

//distType:0 -> absolute value, 1 -> algebra value 
float GEOMBASICALGOM_API distPN(const MyPoint3D& pnt, const MyPlane3D& plane, MyPoint3D& footPnt = MyPoint3D(), int distType = 0);



//-------------------- plane / area --------------------- 

float GEOMBASICALGOM_API areaTriangle(const MyPoint3D& pnt0, const MyPoint3D& pnt1, const MyPoint3D& pnt2);

int GEOMBASICALGOM_API plane3P(const MyPoint3D& pnt0, const MyPoint3D& pnt1, const MyPoint3D& pnt2, MyPlane3D& plane);



//----------------------- angle -------------------------

//check: whether need to unitize the vectors or not
//return: 0~180
float GEOMBASICALGOM_API angleVV(MyVector3D vec0, MyVector3D vec1, bool check = true);

//0~180
float GEOMBASICALGOM_API angleLL(MyLine3D line0, MyLine3D line1);

//angle: -180~180
//return: -1 -> non-existant, 1 -> status ok
int GEOMBASICALGOM_API angleAxisVV(MyVector3D axis, MyVector3D vec0, MyVector3D vec1, float& angle, bool check = true);



//----------------------- transform ------------------------

int GEOMBASICALGOM_API csysTrans3D(const MyPoint3D& srcPnt, MyPoint3D& transPnt, const CsysTrans3D& transform);

int GEOMBASICALGOM_API csysTrans3D(const MyVector3D& srcVec, MyVector3D& transVec, const CsysTrans3D& transform);

int GEOMBASICALGOM_API csysTrans3D(const MyPose3D& srcPos, MyPose3D& transPos, const CsysTrans3D& transform);



//**************************************** 2D Algorithm ****************************************

//----------------- point / intersection ----------------

int GEOMBASICALGOM_API istLL(const MyLine2D& line0, const MyLine2D& line1, MyPoint2D& pnt);



//------------------ line / distance -------------------- 

float GEOMBASICALGOM_API distPP(const MyPoint2D& pnt0, const MyPoint2D& pnt1);

//distType:0 -> absolute value, 1 -> algebra value 
float GEOMBASICALGOM_API distPL(const MyPoint2D& pnt, const MyLine2D& line, int distType = 0, MyPoint2D& footPnt = MyPoint2D());

float GEOMBASICALGOM_API distP2PPL(const MyPoint2D& pnt0, const MyPoint2D& pnt1, const MyPoint2D& pnt2);



//-------------------- plane / area ---------------------

float GEOMBASICALGOM_API areaTriangle(const MyPoint2D& pnt0, const MyPoint2D& pnt1, const MyPoint2D& pnt2);

//the input four points must be clockwise or anticlockwise
float GEOMBASICALGOM_API areaQuadrangle(const MyPoint2D& pnt0, const MyPoint2D& pnt1, const MyPoint2D& pnt2, const MyPoint2D& pnt3);



//----------------------- angle -------------------------

//angleType: 1 -> absolute angle (0~180бу), 2 -> relative angle from vec0 to vec1 (-180~180бу)
float GEOMBASICALGOM_API angleVV(MyVector2D vec0, MyVector2D vec1, int angleType = 1);

//angleType: 1 -> absolute angle (0~180бу), 2 -> relative angle from vec0 to vec1 (-180~180бу)
float GEOMBASICALGOM_API angleVV(MyPose2D pos0, MyPose2D pos1, int angleType = 1);

//angleType: 0 -> absolute angle (0~90бу), 1 -> absolute angle (0~180бу), 2 -> relative angle from vec0 to vec1 (-180~180бу)
float GEOMBASICALGOM_API angleLL(const MyLine2D& line0, const MyLine2D& line1, int angleType = 0);



//----------------------- rectangle -------------------------

void GEOMBASICALGOM_API rotRect4P(const MyRotRect& rotRect, MyPoint2D pnt[]);

MyRect GEOMBASICALGOM_API boundRect(const MyRotRect& rotRect);

void GEOMBASICALGOM_API rectNinePalace(const MyRotRect& rotRect, MyPoint2D pnt[]);

vector<vector<MyRotRect>> GEOMBASICALGOM_API rectArrayItems(const MyRectArray& rectArray);



//------------------------ relation --------------------------

bool GEOMBASICALGOM_API lineSplitPnts(const MyLine2D& line, const MyPoint2D& pnt0, const MyPoint2D& pnt1);

bool GEOMBASICALGOM_API rectContainPnt(const MyRotRect& rotRect, const MyPoint2D& pnt);



//----------------------- transform ------------------------

int GEOMBASICALGOM_API csysTrans2D(const MyPoint2D& srcPnt, MyPoint2D& transPnt, const CsysTrans2D& transform);

int GEOMBASICALGOM_API csysTrans2D(const MyVector2D& srcVec, MyVector2D& transVec, const CsysTrans2D& transform);

int GEOMBASICALGOM_API csysTrans2D(const MyPose2D& srcPos, MyPose2D& transPos, const CsysTrans2D& transform);

int GEOMBASICALGOM_API rotationTrans2D(const MyPoint2D& rotCenter, const MyPoint2D& srcPnt, MyPoint2D& transPnt, float rotAngle);

int GEOMBASICALGOM_API rotationTrans2D(const MyPoint2D& rotCenter, const MyPose2D& srcPos, MyPose2D& transPos, float rotAngle);

int GEOMBASICALGOM_API rotationTrans2D(const MyVector2D& srcVec, MyVector2D& transVec, float rotAngle);