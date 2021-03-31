#include "calib2D.h"
#include "algebraBasicAlgom.h"
#include "geomBasicAlgom.h"
//#include "opencvBasic.h"

int affineTrans2D(const vector<MyPoint2D>& imgPnts, const vector<MyPoint2D>& wldPnts, CsysTrans2D& trans)
{
	if (imgPnts.empty() || imgPnts.size() != wldPnts.size())
		return -1;

	int num = imgPnts.size();
	MyMatrix2D<float> A(6, 2 * num);
	MyMatrix2D<float> b(1, 2 * num);
	for (int i = 0; i < num; i++)
	{
		A[2 * i][0] = imgPnts[i].x;
		A[2 * i][1] = imgPnts[i].y;
		A[2 * i][2] = 1;
		A[2 * i + 1][3] = imgPnts[i].x;
		A[2 * i + 1][4] = imgPnts[i].y;
		A[2 * i + 1][5] = 1;
		b[2 * i][0] = wldPnts[i].x;
		b[2 * i + 1][0] = wldPnts[i].y;
	}
	MyMatrix2D<float> x = overLinearEqs(A, b);

	//temp
	MyMatrix2D<float> b_ = A*x;

	float arr[9];
	for (int i = 0; i < 6; i++)
		arr[i] = x[i][0];
	arr[6] = 0;
	arr[7] = 0;
	arr[8] = 1;
	trans = CsysTrans2D(arr);
	return 1;
}

MyPoint2D calcuCircleCenter(const MyPoint2D& pnt0, const MyPoint2D& pnt1, float angle)
{
	MyPose2D src_pos0(pnt0.x, pnt0.y, pnt1.x - pnt0.x, pnt1.y - pnt0.y);
	MyPose2D dst_pos0;
	MyPose2D src_pos1(pnt1.x, pnt1.y, pnt0.x - pnt1.x, pnt0.y - pnt1.y);
	MyPose2D dst_pos1;
	float rot_angle = (180 - angle) / 2;
	MyPoint2D pnt(0, 0);
	rotationTrans2D(pnt0, src_pos0, dst_pos0, rot_angle);
	rotationTrans2D(pnt1, src_pos1, dst_pos1, -rot_angle);
	int rtn = istLL(dst_pos0, dst_pos1, pnt);
	if (rtn != 1)
		pnt = (pnt0 + pnt1) / 2;
	return pnt;
}

MyPoint2D calcuCircleCenter2(const MyPoint2D& pnt0, const MyPoint2D& pnt1, float angle)
{
	MyPoint2D pnt(0, 0);
	float dist = distPP(pnt0, pnt1);
	float lenth = (dist / 2) / tanf(angle / 2 / DEG_PER_RAD);
	MyPoint2D mid_pnt = (pnt0 + pnt1) / 2;
	MyVector2D vec_pnts(pnt1.x - pnt0.x, pnt1.y - pnt0.y);
	MyVector2D vec_vertical(-vec_pnts.vy, vec_pnts.vx);
	UNIT_VECTOR_2D(vec_vertical);
	pnt.x = mid_pnt.x + lenth*vec_vertical.vx;
	pnt.y = mid_pnt.y + lenth*vec_vertical.vy;
	return pnt;
}

int calcuRotCircle_(const vector<MyPoint2D>& imgRotPnts, float angleStep, int stepNum, MyCircle& circle, float& e, bool inverse)
{
	if (imgRotPnts.size() != 2 * stepNum + 1)
		return -1;

	float center_x = 0.0f;
	float center_y = 0.0f;

	if (!inverse)
	{
		for (int i = 1; i <= 2 * stepNum; i++)
		{
			float sub_center_x = 0.0f;
			float sub_center_y = 0.0f;
			for (int j = 0; j < 2 * stepNum + 1 - i; j++)
			{
			//	MyPoint2D sub_center = calcuCircleCenter(imgRotPnts.at(j), imgRotPnts.at(j + i), angleStep*i);
				MyPoint2D sub_center = calcuCircleCenter2(imgRotPnts.at(j), imgRotPnts.at(j + i), angleStep*i);
				sub_center_x += sub_center.x;
				sub_center_y += sub_center.y;
			}
			center_x += sub_center_x / (2 * stepNum + 1 - i);
			center_y += sub_center_y / (2 * stepNum + 1 - i);
		}
	}
	else
	{
		for (int i = 1; i <= 2 * stepNum; i++)
		{
			float sub_center_x = 0.0f;
			float sub_center_y = 0.0f;
			for (int j = 0; j < 2 * stepNum + 1 - i; j++)
			{
			//	MyPoint2D sub_center = calcuCircleCenter(imgRotPnts.at(j + i), imgRotPnts.at(j), angleStep*i);
				MyPoint2D sub_center = calcuCircleCenter2(imgRotPnts.at(j + i), imgRotPnts.at(j), angleStep*i);
				sub_center_x += sub_center.x;
				sub_center_y += sub_center.y;
			}
			center_x += sub_center_x / (2 * stepNum + 1 - i);
			center_y += sub_center_y / (2 * stepNum + 1 - i);
		}
	}

	circle.x = center_x / (2 * stepNum);
	circle.y = center_y / (2 * stepNum);
	circle.r = 0;
	for (int i = 0; i <= 2 * stepNum; i++)
		circle.r += distPP(MyPoint2D(circle.x, circle.y), imgRotPnts.at(i));
	circle.r /= 2 * stepNum + 1;

	e = 0.0f;
	for (int i = 0; i < imgRotPnts.size(); i++)
	{
		float dx = imgRotPnts[i].x - circle.x;
		float dy = imgRotPnts[i].y - circle.y;
		float dist = sqrtf(dx*dx + dy*dy);
		e += (dist - circle.r)*(dist - circle.r);
	}
	e = sqrtf(e / (int)imgRotPnts.size()) / circle.r;

	return 1;
}

int calcuRotCircle(const vector<MyPoint2D>& imgRotPnts, float angleStep, int stepNum, MyCircle& circle, float& e)
{
	vector<MyPoint2D> rot_pnts;
	for (int i = 0; i < stepNum; i++)
		rot_pnts.push_back(imgRotPnts.at(2 * stepNum - i));
	for (int i = 0; i <= stepNum; i++)
		rot_pnts.push_back(imgRotPnts.at(i));

	MyCircle circle1;
	MyCircle circle2;
	float e1;
	float e2;
	int rtn1 = calcuRotCircle_(rot_pnts, angleStep, stepNum, circle1, e1, false);
	int rtn2 = calcuRotCircle_(rot_pnts, angleStep, stepNum, circle2, e2, true);

	if (e1 < e2)
	{
		circle = circle1;
		e = e1;
		return rtn1;
	}
	else
	{
		circle = circle2;
		e = e2;
		return rtn2;
	}
}

int calibrationHandEye2D(const vector<MyPoint2D>& imgTransPnts, const vector<MyPoint2D>& imgRotPnts, Calibration2D& calib,
	float width, float height, float angleStep, int stepNum, const MyRobotPos& regCalibPos, int calib2DMode, int cameraInstallType)
{
	if (imgRotPnts.size() != 2 * stepNum + 1)
		return -1;

	CsysTrans2D trans1;
	switch (calib2DMode)
	{
	case CALIB_9P2D:
	{
		if (imgTransPnts.size() != 9)
			return -1;

		vector<MyPoint2D> wldTransPnts;
		if (FIXED_CAMERA == cameraInstallType)
		{
			wldTransPnts.push_back({ 0,0 });
			wldTransPnts.push_back({ width,0 });
			wldTransPnts.push_back({ width,height });
			wldTransPnts.push_back({ 0,height });
			wldTransPnts.push_back({ -width,height });
			wldTransPnts.push_back({ -width,0 });
			wldTransPnts.push_back({ -width,-height });
			wldTransPnts.push_back({ 0,-height });
			wldTransPnts.push_back({ width,-height });
		}
		else if (TRANSLATIONAL_CAMERA == cameraInstallType || TRANS_ROT_CAMERA == cameraInstallType)
		{
			wldTransPnts.push_back({ 0,0 });
			wldTransPnts.push_back({ -width,0 });
			wldTransPnts.push_back({ -width,-height });
			wldTransPnts.push_back({ 0,-height });
			wldTransPnts.push_back({ width,-height });
			wldTransPnts.push_back({ width,0 });
			wldTransPnts.push_back({ width,height });
			wldTransPnts.push_back({ 0,height });
			wldTransPnts.push_back({ -width,height });
		}
		affineTrans2D(imgTransPnts, wldTransPnts, trans1);
		break;
	}
	case CALIB_16P2D:
	{


		break;
	}
	case CALIB_25P2D:
	{

		break;
	}
	default:
		break;
	}

	MyCircle circle;
//	leastSquareCircleFitting(imgRotPnts, circle);
	calcuRotCircle(imgRotPnts, angleStep, stepNum, circle);

	MyPoint2D img_center(circle.x, circle.y);
	MyPoint2D wld_center;
	MyPoint2D reg_center(regCalibPos.x, regCalibPos.y);
	csysTrans2D(img_center, wld_center, trans1);
	CsysTrans2D trans2(wld_center, reg_center);

	calib.trans = trans2*trans1;
	calib.reg_pos = regCalibPos;
	calib.camera_install_type = cameraInstallType;
	return 1;

}

int guideRobotPick2D(const MyPose2D& tmplImgPos, const MyPose2D& sceneImgPos, const Calibration2D& calib,
	const MyRobotPos& regCamPos, const MyRobotPos& regPickPos, float& dx, float& dy, float& da)
{
	MyPoint2D rot_center(regPickPos.x, regPickPos.y);
	MyPose2D tmpl_wld_pos;
	MyPose2D scene_wld_pos;
	MyPose2D inter_wld_pos;
	CsysTrans2D ultimate_trans;

	switch (calib.camera_install_type)
	{
	case FIXED_CAMERA:
	{
		ultimate_trans = calib.trans;
		break;
	}
	case TRANSLATIONAL_CAMERA:
	{
		CsysTrans2D trans_t(MyPoint2D(calib.reg_pos.x, calib.reg_pos.y), MyPoint2D(regCamPos.x, regCamPos.y));
		ultimate_trans = trans_t*calib.trans;
		break;
	}
	case TRANS_ROT_CAMERA:
	{
		CsysTrans2D trans_t(MyPoint2D(calib.reg_pos.x, calib.reg_pos.y), MyPoint2D(regCamPos.x, regCamPos.y));
		CsysTrans2D trans_r(MyPoint2D(regCamPos.x, regCamPos.y), regCamPos.Rz - calib.reg_pos.Rz);
		ultimate_trans = trans_r*trans_t*calib.trans;
		break;
	}
	default:
		break;
	}

	csysTrans2D(tmplImgPos, tmpl_wld_pos, ultimate_trans);
	csysTrans2D(sceneImgPos, scene_wld_pos, ultimate_trans);
	da = angleVV(tmpl_wld_pos, scene_wld_pos, 2);
	rotationTrans2D(rot_center, tmpl_wld_pos, inter_wld_pos, da);
	dx = scene_wld_pos.x - inter_wld_pos.x;
	dy = scene_wld_pos.y - inter_wld_pos.y;
	return 1;
}

int guideRobotMultiPick2D(const MyPose2D& tmplImgPos, const MyPose2D& sceneImgPos, const Calibration2D& calib,
	const MyRobotPos& regCamPos, const MyRobotPos& regPickPos, const MyRobotPos& regCurCamPos, float& dx, float& dy, float& da)
{
	switch (calib.camera_install_type)
	{
	case FIXED_CAMERA:
	{
		return guideRobotPick2D(tmplImgPos, sceneImgPos, calib, regCamPos, regPickPos, dx, dy, da);
	}
	case TRANSLATIONAL_CAMERA:
	{
		int rtn = guideRobotPick2D(tmplImgPos, sceneImgPos, calib, regCamPos, regPickPos, dx, dy, da);
		dx += regCurCamPos.x - regCamPos.x;
		dy += regCurCamPos.y - regCamPos.y;
		return rtn;
	}
	case TRANS_ROT_CAMERA:
	{
		int rtn = guideRobotPick2D(tmplImgPos, sceneImgPos, calib, regCamPos, regPickPos, dx, dy, da);
		float theta = regCurCamPos.Rz - regCamPos.Rz;
		normalization(theta, -180, 180);
		float sin_theta = sinf(theta / DEG_PER_RAD);
		float cos_theta = cosf(theta / DEG_PER_RAD);
		float dx_ = dx*cos_theta - dy*sin_theta;
		float dy_ = dx*sin_theta + dy*cos_theta;
		
		CsysTrans2D trans(regCamPos, regCurCamPos);
		MyPose2D cur_pick_pos;
		csysTrans2D(MyPose2D(regPickPos.x, regPickPos.y, regPickPos.Rz), cur_pick_pos, trans);
		dx = cur_pick_pos.x - regPickPos.x + dx_;
		dy = cur_pick_pos.y - regPickPos.y + dy_;
		da = theta;
		return rtn;
	}
	default:
		return -1;
	}
}

int guideRobotAdjust2D(const MyPose2D& tmplImgPos, const MyPose2D& sceneImgPos, const Calibration2D& calib,
	const MyRobotPos& regAdjustPos, float& dx, float& dy, float& da)
{
	return guideRobotPick2D(sceneImgPos, tmplImgPos, calib, regAdjustPos, regAdjustPos, dx, dy, da);
}

int guideRobotAdjustPlace2D(const MyPose2D& adjustTmplImgPos, const MyPose2D& adjustSceneImgPos, const Calibration2D& adjustCalib,
	const MyRobotPos& regAdjustPos, const MyPose2D& placeTmplImgPos, const MyPose2D& placeSceneImgPos, const Calibration2D& placeCalib,
	const MyRobotPos& regPlacePos, float& dx, float& dy, float& da)
{
	MyPose2D adust_rot_center;		//known
	MyPose2D adjust_tmpl_mark1;		//known
	MyPose2D adjust_scene_mark1;	//known
	MyPose2D place_rot_center;		//known
	MyPose2D place_tmpl_mark2;		//known
	MyPose2D place_cur_mark2;		//known
	MyPose2D place_tmpl_mark1;		
	MyPose2D place_scene_mark1;		
	MyPose2D place_scene_mark2;
	CsysTrans2D transform1;
	CsysTrans2D transform2;
	CsysTrans2D adjust_calib_trans;
	CsysTrans2D place_calib_trans;
	
	//normalize the transformation relationship from image coordinate to robot coordinate at the adjust/place photo position 
	switch (adjustCalib.camera_install_type)
	{
	case FIXED_CAMERA:
	{
		adjust_calib_trans = adjustCalib.trans;
		break;
	}
	case TRANSLATIONAL_CAMERA:
	{
		CsysTrans2D trans_t(MyPoint2D(adjustCalib.reg_pos.x, adjustCalib.reg_pos.y), MyPoint2D(regAdjustPos.x, regAdjustPos.y));
		adjust_calib_trans = trans_t*adjustCalib.trans;
		break;
	}
	case TRANS_ROT_CAMERA:
	{
		CsysTrans2D trans_t(MyPoint2D(adjustCalib.reg_pos.x, adjustCalib.reg_pos.y), MyPoint2D(regAdjustPos.x, regAdjustPos.y));
		CsysTrans2D trans_r(MyPoint2D(regAdjustPos.x, regAdjustPos.y), regAdjustPos.Rz - adjustCalib.reg_pos.Rz);
		adjust_calib_trans = trans_r*trans_t*adjustCalib.trans;
		break;
	}
	default:
		break;
	}
	switch (placeCalib.camera_install_type)
	{
	case FIXED_CAMERA:
	{
		place_calib_trans = placeCalib.trans;
		break;
	}
	case TRANSLATIONAL_CAMERA:
	{
		CsysTrans2D trans_t(MyPoint2D(placeCalib.reg_pos.x, placeCalib.reg_pos.y), MyPoint2D(regPlacePos.x, regPlacePos.y));
		place_calib_trans = trans_t*placeCalib.trans;
		break;
	}
	case TRANS_ROT_CAMERA:
	{
		CsysTrans2D trans_t(MyPoint2D(placeCalib.reg_pos.x, placeCalib.reg_pos.y), MyPoint2D(regPlacePos.x, regPlacePos.y));
		CsysTrans2D trans_r(MyPoint2D(regPlacePos.x, regPlacePos.y), regPlacePos.Rz - placeCalib.reg_pos.Rz);
		place_calib_trans = trans_r*trans_t*placeCalib.trans;
		break;
	}
	default:
		break;
	}

	//preparation of data for calculation
	adust_rot_center = MyPose2D(regAdjustPos.x, regAdjustPos.y, regAdjustPos.Rz);
	place_rot_center = MyPose2D(regPlacePos.x, regPlacePos.y, regPlacePos.Rz);
	csysTrans2D(adjustTmplImgPos, adjust_tmpl_mark1, adjust_calib_trans);
	csysTrans2D(adjustSceneImgPos, adjust_scene_mark1, adjust_calib_trans);
	csysTrans2D(placeTmplImgPos, place_tmpl_mark2, place_calib_trans);
	csysTrans2D(placeSceneImgPos, place_cur_mark2, place_calib_trans);

	//coordinate transformation
	transform1 = CsysTrans2D(adust_rot_center, place_rot_center);
	csysTrans2D(adjust_tmpl_mark1, place_tmpl_mark1, transform1);
	csysTrans2D(adjust_scene_mark1, place_scene_mark1, transform1);
	transform2 = CsysTrans2D(place_tmpl_mark1, place_scene_mark1);
	csysTrans2D(place_tmpl_mark2, place_scene_mark2, transform2);

	//the same as the pick process
	MyPose2D place_inter_mark2;
	da = angleVV(place_scene_mark2, place_cur_mark2, 2);
	rotationTrans2D(MyPoint2D(regPlacePos.x, regPlacePos.y), place_scene_mark2, place_inter_mark2, da);
	dx = place_cur_mark2.x - place_inter_mark2.x;
	dy = place_cur_mark2.y - place_inter_mark2.y;
	return 1;
}

void testCalcuCircle()
{
// 	MyPoint2D circle_center(500, 500);
// 	float angle_step0 = 5.0f;
// 	float angle_init = 45.0f;
// 	int step_num0 = 4;
// 	float radius = 400.0f;
// 	float range = 2.0f;
// 	vector<MyPoint2D> circle_pnts;
// 	srand(time(0));
// 	for (int i = 0; i <= step_num0; i++)
// 	{
// 		MyPoint2D pnt;
// 	//	float theta = angle_init + i*angle_step0;
// 		float theta = angle_init - i*angle_step0;
// 		pnt.x = circle_center.x + radius*cosf(theta / DEG_PER_RAD);
// 		pnt.y = circle_center.y + radius*sinf(theta / DEG_PER_RAD);
// 		pnt.x += ((float)rand() / RAND_MAX * 2 - 1)*range;
// 		pnt.y += ((float)rand() / RAND_MAX * 2 - 1)*range;
// 		circle_pnts.push_back(pnt);
// 	}
// 	for (int i = 1; i <= step_num0; i++)
// 	{
// 		MyPoint2D pnt;
// 	//	float theta = angle_init - i*angle_step0;
// 		float theta = angle_init + i*angle_step0;
// 		pnt.x = circle_center.x + radius*cosf(theta / DEG_PER_RAD);
// 		pnt.y = circle_center.y + radius*sinf(theta / DEG_PER_RAD);
// 		pnt.x += ((float)rand() / RAND_MAX * 2 - 1)*range;
// 		pnt.y += ((float)rand() / RAND_MAX * 2 - 1)*range;
// 		circle_pnts.push_back(pnt);
// 	}
// 
// 	MyCircle calcu_circle0;
// 	MyCircle calcu_circle1;
// 	leastSquareCircleFitting(circle_pnts, calcu_circle0);
// 	calcuRotCircle(circle_pnts, angle_step0, step_num0, calcu_circle1);
//
//
// 	cv::Mat img(1000, 1000, CV_8UC3, cv::Scalar(0, 0, 0));
// 
// 	for (int i = 0; i < circle_pnts.size(); i++)
// 		cv::circle(img, cv::Point2f(circle_pnts[i].x, circle_pnts[i].y), 2, cv::Scalar(255, 255, 255), -1);
// 
// 	cv::circle(img, cv::Point2f(circle_center.x, circle_center.y), radius, cv::Scalar(0, 0, 255), 2);
// 	cv::circle(img, cv::Point2f(calcu_circle0.x, calcu_circle0.y), calcu_circle0.r, cv::Scalar(0, 255, 0), 1);
// 	cv::circle(img, cv::Point2f(calcu_circle1.x, calcu_circle1.y), calcu_circle1.r, cv::Scalar(255, 0, 0), 1);
// 
// 	cv::namedWindow("image" /*, cv::WINDOW_NORMAL*/);
// 	cv::imshow("image", img);
// 	cv::waitKey(0);
// 	cv::destroyAllWindows();
}

void testRobotMoveVerify()
{
	float array[9] = { 0.00495102,-0.353141,817.804,-0.354285,-0.00292378,431.587,0,0,1 };
	CsysTrans2D robot_trans(array);

//	MyPoint2D img_pnt0(1002, 964);
//	MyPoint2D img_pnt0(818, 1273);
//	MyPoint2D img_pnt0(589, 981);
//	MyPoint2D img_pnt0(1104, 643);
//	MyPoint2D img_pnt0(733, 639);
//	MyPoint2D img_pnt0(1055, 634);
	MyPoint2D img_pnt0(770, 646);

	MyPoint2D wld_pnt0;
	csysTrans2D(img_pnt0, wld_pnt0, robot_trans);
}

void testCalib()
{

	vector<MyPoint2D> img_trans_pnts;
	img_trans_pnts.push_back(MyPoint2D(1010, 506));
	img_trans_pnts.push_back(MyPoint2D(1404, 500));
	img_trans_pnts.push_back(MyPoint2D(1413, 108));
	img_trans_pnts.push_back(MyPoint2D(1002, 103));
	img_trans_pnts.push_back(MyPoint2D(621, 98));
	img_trans_pnts.push_back(MyPoint2D(617, 491));
	img_trans_pnts.push_back(MyPoint2D(608, 899));
	img_trans_pnts.push_back(MyPoint2D(994, 901));
	img_trans_pnts.push_back(MyPoint2D(1392, 903));

	vector<MyPoint2D> img_rot_pnts;
	img_rot_pnts.push_back(MyPoint2D(1010, 506));
	img_rot_pnts.push_back(MyPoint2D(1251, 300));
	img_rot_pnts.push_back(MyPoint2D(1398, 140));
	img_rot_pnts.push_back(MyPoint2D(703, 670));
	img_rot_pnts.push_back(MyPoint2D(589, 730));


	Calibration2D calib;
	float width = 20;
	float height = 20;
	float angle_step = 5;
	float step_num = 2;
	MyRobotPos reg_calib_pos(15, 25, 100, 5, 6, 35);

	calibrationHandEye2D(img_trans_pnts, img_rot_pnts, calib, width, height, angle_step, step_num, reg_calib_pos);

	MyPoint2D img_pnt(1402, 913);
	MyPoint2D wld_pnt;
	csysTrans2D(img_pnt, wld_pnt, calib.trans);

	Calibration2D calib_(calib);
}

void testCalib2()
{
	vector<MyPoint2D> img_trans_pnts;
	img_trans_pnts.push_back(MyPoint2D(1012.37, 1127.1));
	img_trans_pnts.push_back(MyPoint2D(1021.13, 1593.09));
	img_trans_pnts.push_back(MyPoint2D(1484.22, 1584.96));
	img_trans_pnts.push_back(MyPoint2D(1477.51, 1118.95));
	img_trans_pnts.push_back(MyPoint2D(1470.03, 651.688));
	img_trans_pnts.push_back(MyPoint2D(1006.92, 659.985));
	img_trans_pnts.push_back(MyPoint2D(544.589, 669.468));
	img_trans_pnts.push_back(MyPoint2D(550.107, 1134.62));
	img_trans_pnts.push_back(MyPoint2D(558.188, 1599.78));

	vector<MyPoint2D> img_rot_pnts;
	img_rot_pnts.push_back(MyPoint2D(1012.25, 1127.08));
	img_rot_pnts.push_back(MyPoint2D(930.222, 1153.73));
	img_rot_pnts.push_back(MyPoint2D(850.898, 1187.49));
	img_rot_pnts.push_back(MyPoint2D(774.788, 1227.94));
	img_rot_pnts.push_back(MyPoint2D(702.737, 1274.85));
	img_rot_pnts.push_back(MyPoint2D(635.171, 1327.66));
	img_rot_pnts.push_back(MyPoint2D(1096.37, 1107.69));
	img_rot_pnts.push_back(MyPoint2D(1181.99, 1095.64));
	img_rot_pnts.push_back(MyPoint2D(1268.28, 1090.99));
	img_rot_pnts.push_back(MyPoint2D(1354.61, 1093.9));
	img_rot_pnts.push_back(MyPoint2D(1440.3, 1104.3));


	Calibration2D calib;
	float width = 50;
	float height = 50;
	float angle_step = 5;
	float step_num = 5;
	MyRobotPos reg_calib_pos(1012.37, 1127.1, 0, 0, 0, 90.25);

	calibrationHandEye2D(img_trans_pnts, img_rot_pnts, calib, width, height, angle_step,
		step_num, reg_calib_pos, CALIB_9P2D, TRANS_ROT_CAMERA);



	MyPose2D tmplImgPos(23.6, 12.3, 0.01);
	MyPose2D sceneImgPos(23.6, 12.3, 0.01);
	MyRobotPos regPickPos(10, 10, 20, 0, 0, 30);
	float dx;
	float dy;
	float da;
	guideRobotPick2D(tmplImgPos, sceneImgPos, calib, regPickPos, regPickPos, dx, dy, da);








	MyPoint2D img_pnt(1012.37, 1127.1);
	MyPoint2D wld_pnt;
	csysTrans2D(img_pnt, wld_pnt, calib.trans);

	Calibration2D calib_(calib);
}

void testCalib3()
{
	CsysTrans2D trans(-0.139, 0.138, -0.175, 89.18, 991.73, 208.5);
	Calibration2D calib;
	calib.trans = trans;
	calib.reg_pos = MyRobotPos(734.73, 34.61, 1117.69, 167.85, 179.34, 90.24);
	calib.camera_install_type = TRANS_ROT_CAMERA;

	MyPose2D img_tmpl_pos(1368.77, 621.481, 0);
	MyPose2D img_scene_pos(1044.66, 673.779, -24.45);
	MyRobotPos reg_pick_pos(734.729, 34.606, 739.782, 167.849, 179.341, 90.24);
	
	float dx, dy, da;
	float x, y, a;
	guideRobotPick2D(img_tmpl_pos, img_scene_pos, calib, reg_pick_pos, reg_pick_pos, dx, dy, da);

	x = reg_pick_pos.x + dx;
	y = reg_pick_pos.y + dy;
	a = reg_pick_pos.Rz + da;



	//temp
	vector<MyPoint2D> imgPnts;
	vector<MyPoint2D> wldPnts;
	CsysTrans2D trans11;
	imgPnts.push_back({ 1226.97f,1022.51f });
	imgPnts.push_back({ 1239.04f,1746.91f });
	imgPnts.push_back({ 1956.9f,1732.59f });
	imgPnts.push_back({ 1948.33f,1010.07f });
	imgPnts.push_back({ 1935.11f,286.311f });
	imgPnts.push_back({ 1217.61f,296.267f });
	imgPnts.push_back({ 499.952f,311.386f });
	imgPnts.push_back({ 506.536f,1035.49f });
	imgPnts.push_back({ 518.529f,1757.57f });

	wldPnts.push_back({ 0,0 });
	wldPnts.push_back({ -100,0 });
	wldPnts.push_back({ -100,-100 });
	wldPnts.push_back({ 0,-100 });
	wldPnts.push_back({ 100,-100 });
	wldPnts.push_back({ 100,0 });
	wldPnts.push_back({ 100,100 });
	wldPnts.push_back({ 0,100 });;
	wldPnts.push_back({ -100,100 });

	affineTrans2D(imgPnts, wldPnts, trans11);






	int b = 0;
}



void testMatrix2D()
{
	int arr[9] = { 1,2,3,4,2,6 };
	MyMatrix2D<int> matrix(2, 3);
	for (int i = 0; i < 6; i++)
	{
		matrix.at(i) = arr[i];
	}

	MyMatrix2D<int> b(1, 3);
	b[0][0] = 5;
	b[1][0] = 6;
	b[2][0] = 20;

	MyMatrix2D<float> x = overLinearEqs(matrix, b);
}

int testCalibration()
{

//	testMatrix2D();

//	testCalcuCircle();

//	testRobotMoveVerify();

//	testCalib();

//	testCalib2();

	testCalib3();

	return 1;
}