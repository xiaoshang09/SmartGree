#pragma once
#include "MyDataType.h"


#ifdef CALIBRATION_EXPORTS
#define CALIBRATION_API __declspec(dllexport)
#else 
#define CALIBRATION_API __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"CalibrationD.lib")
#else
#pragma comment(lib,"Calibration.lib")
#endif
#endif // CALIBRATION_EXPORTS


enum Calib2DMode
{
	CALIB_9P2D = 1,
	CALIB_16P2D,
	CALIB_25P2D,
};

enum CameraInstallType
{
	FIXED_CAMERA = 1,
	TRANSLATIONAL_CAMERA,
	TRANS_ROT_CAMERA
};

// m_RegCalibRobotPos m_RegPickRobotPos m_RegPlaceRobotPos
struct Calibration2D
{
	Calibration2D() :trans(), reg_pos(), camera_install_type(FIXED_CAMERA) {}

	CsysTrans2D trans;
	MyRobotPos reg_pos;
	int camera_install_type;
};

int CALIBRATION_API affineTrans2D(const vector<MyPoint2D>& imgPnts, const vector<MyPoint2D>& wldPnts, CsysTrans2D& trans);

MyPoint2D calcuCircleCenter(const MyPoint2D& pnt0, const MyPoint2D& pnt1, float angle);

MyPoint2D calcuCircleCenter2(const MyPoint2D& pnt0, const MyPoint2D& pnt1, float angle);

int calcuRotCircle_(const vector<MyPoint2D>& imgRotPnts, float angleStep, int stepNum, MyCircle& circle, float& e = REF_DEFAULT, bool inverse = false);

int CALIBRATION_API calcuRotCircle(const vector<MyPoint2D>& imgRotPnts, float angleStep, int stepNum, MyCircle& circle, float& e = REF_DEFAULT);

int CALIBRATION_API calibrationHandEye2D(const vector<MyPoint2D>& imgTransPnts, const vector<MyPoint2D>& imgRotPnts,
	Calibration2D& calib, float width, float height, float angleStep, int stepNum, const MyRobotPos& regCalibPos,
	int calib2DMode = CALIB_9P2D, int cameraInstallType = FIXED_CAMERA);

int CALIBRATION_API guideRobotPick2D(const MyPose2D& tmplImgPos, const MyPose2D& sceneImgPos, const Calibration2D& calib,
	const MyRobotPos& regCamPos, const MyRobotPos& regPickPos, float& dx, float& dy, float& da);

int CALIBRATION_API guideRobotMultiPick2D(const MyPose2D& tmplImgPos, const MyPose2D& sceneImgPos, const Calibration2D& calib,
	const MyRobotPos& regCamPos, const MyRobotPos& regPickPos, const MyRobotPos& regCurCamPos, float& dx, float& dy, float& da);

int CALIBRATION_API guideRobotAdjust2D(const MyPose2D& tmplImgPos, const MyPose2D& sceneImgPos, const Calibration2D& calib,
	const MyRobotPos& regAdjustPos, float& dx, float& dy, float& da);

int CALIBRATION_API guideRobotAdjustPlace2D(const MyPose2D& adjustTmplImgPos, const MyPose2D& adjustSceneImgPos, const Calibration2D& adjustCalib,
	const MyRobotPos& regAdjustPos, const MyPose2D& placeTmplImgPos, const MyPose2D& placeSceneImgPos, const Calibration2D& placeCalib,
	const MyRobotPos& regPlacePos, float& dx, float& dy, float& da);

int CALIBRATION_API testCalibration();