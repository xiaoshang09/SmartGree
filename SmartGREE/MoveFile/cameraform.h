#ifndef CAMERAFORM_H
#define CAMERAFORM_H
#include <QWidget>
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <thread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSerialPort>
#include <QSerialPortInfo>
//#include <QtSerialPort>
#include<QTime>
#include <QWidget>
#include<QTextCodec>   //头文件加上它
#include <QDebug>
#include <QFile>
#include <QFileDialog>  //获取文件名称的对话框
#include <QMessageBox>
//2、定义头文件和命名空间
#include "HalconCpp.h"
#include"GalaxyIncludes.h"
#include"GXBitmap.h"
#include <showtextmessage.h>
#include<mythread.h>
#include <QGuiApplication>
#include <QCloseEvent>
#include <QApplication>
#include<mysignal.h>
#include "MyDataType.h"
#include "calib2D.h"

#include <QObjectCleanupHandler>


using namespace HalconCpp;
using namespace std;

namespace Ui {
class CameraForm;
}
class MyTemplate;
class CameraForm : public QWidget
{
    Q_OBJECT

public:
    explicit CameraForm(QWidget *parent = 0);
    ~CameraForm();
    //#pragma region 参数
    //窗口
    HTuple  WindowHandle;
    HTuple  WindowHandle2;

    //相机采集地址

    //model文件参数
    QString CurproductID;

//     float m_fRegCalibX1;
//     float m_fRegCalibY1;
//     float m_fRegCalibZ1;
//     float m_fRegCalibRz1;
// 
//     float m_fRegCalibX2;
//     float m_fRegCalibY2;
//     float m_fRegCalibZ2;
//     float m_fRegCalibRz2;
// 
//     float m_fPlaceTmplImgX1;
//     float m_fPlaceTmplImgY1;
// 	float m_fPlaceTmplImgA1;
// 
//     float m_fAdjustTmplImgY2;
// 	float m_fAdjustTmplImgX2;
//     float m_fAdjustTmplImgA2;


	MyPose2D m_AdjustTmplImgPos;
	MyPose2D m_AdjustSceneImgPos;
	Calibration2D m_AdjustCalib;
	MyRobotPos m_RegAdjustPos;
	
	MyPose2D m_PlaceTmplImgPos;
	MyPose2D m_PlaceSceneImgPos;
	Calibration2D m_PlaceCalib;
	MyRobotPos m_RegPlacePos;

	bool m_bPosReg1;
	bool m_bPosReg2;




    QString basePath = "./Model/";

    //相机开关
    bool open1, open2, open3;
    int isHasCamera;

    //LWH模板全局参数
    QString  prodactID = "00000";
    QString prodactName;
    HTuple  hv_Qx1Test, hv_Qy1Test, hv_HomMat2D;
    HTuple  hv_Qx2Test, hv_Qy2Test, hv_AngleTest;
    HTuple  hv_NCCModel2D11,hv_NCCModel2D12;
    HTuple hv_HomMat2D1,hv_HomMat2D2,hv_RotateCenter1,hv_RotateCenter2,hv_ModelID1,hv_ModelID2,hv_NCCModel2D2;
    HTuple hv_cameraMatrix,hv_robotTeach2D1,hv_offsetData,hv_nccmodelCenter;
    HObject  ho_SHMROI;
    //图片参数
    HObject Image2D1,Image2D2,ho_Image2D1, ho_Image2D2;
    QString file_Path;
    ShowTextMessage* MySTM;
    //算法参数
//     MyPose2D tmplImgPos;
//     MyPose2D sceneImgPos;
//     MyRobotPos regPickPos;
//     MyRobotPos regPlacePos;
//     float dx;float dy;float da;
    //相机参数
    CGXDevicePointer objDevicePtr1, objDevicePtr2;
    CGXFeatureControlPointer objFeatureControlPtr1, objFeatureControlPtr2;
    CGXBitmap*  m_pBitmap;
    //#pragma endregion

    //public slots:
    //    void show();

    void setWinSize(int w,int h);

public slots:

	void slotLineEditClicked();
	void slotRegPosBtnClicked();


    void socket_Read_Data();
    void socket_Disconnected();
    void readSerialData();

    void closeCamera();
    void Initialization();
    void Initialization_Interface();
    void Initialization_Camera();
    void Initialization_Parameter();
    void Initialization_communication();
    void on_pushButton_2_clicked();
    void string_replace(std::string &strBig, const std::string &strsrc, const std::string &strdst);
    //2D2定位
    bool Detection2D1(QString prodactID, HObject Image2D2);
    //检测正反
    int DetectionPN(QString prodactID, HObject Image2D2);
    //2D2定位
    //typedef struct AssemblyPos AssemP;
    double* ImageLocation(QString prodactID, HObject Image2D2);
    string string_crop(string name);

    double* Match2D1(HObject ho_ImageTest2D1, HTuple hv_ModelID1, HTuple *hv_Angle);
    double* Match2D2(HObject ho_ImageTest2D2, HTuple hv_ModelID2, HTuple *hv_AngleTest);
    void dev_display_shape_matching_results (HTuple hv_ModelID, HTuple hv_Color, HTuple hv_Row,
                                             HTuple hv_Column, HTuple hv_Angle, HTuple hv_ScaleR, HTuple hv_ScaleC, HTuple hv_Model);
    void dev_display_shape_matching_results2(HTuple hv_ModelID, HTuple hv_Color, HTuple hv_Row,
                                                         HTuple hv_Column, HTuple hv_Angle, HTuple hv_ScaleR, HTuple hv_ScaleC, HTuple hv_Model);
    //2d1处理模块
    bool xiangjicaiji(CGXDevicePointer objDevicePtr, HTuple window,HObject *imgReturn);
    QString xiangjibaocun(HObject img, QString path);
    void openxiangji(int i, GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo);
    void CaRead_file();

    void disp_message (HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,
                       HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box);
    void DirectionDetect (HObject ho_Image, HTuple hv_ShapeModelID,HTuple hv_ShapeModelID1, HTuple hv_MatchROI,
                          HTuple *hv_DetectResult);
    int jiazai(QString path);
    void appendMessage(const QString &msg);

    public slots://xyf
    int checkProduct(const QString &id, int type, QString &picture);

    int check2D1Dir(const QString &id,QString &picture);
    int check2D2pos(const QString &id,QString &picture,float &x,float &y,float &z,float &Rx,float &Ry,float &Rz);
    int check2D1pos(const QString &id,QString &picture);

    int setProductID(const QString &id);
    void showTemplates();
    //    int addTemplate();//old
    int addTemplate(const QString &id, int type);//new
    QWidget* settingsWt();
    int state(){return 0;}
    void setAppPath(const QString &path);
    void setWt(QWidget*wt);
private slots:
    void on_pushButton_clicked();//打开图片
    void on_pushButton_Cap_Cam1_clicked();//单张采集
    void on_pushButton_Connect_clicked();
    void on_pushButton_Send_clicked();
    void on_pushButton_Open_serial_clicked();
    void on_pushButton_Save_Cam_Exposure_clicked();//曝光值
    void on_pushButton_Save_Cam_Exposure_2_clicked();//曝光二
    void on_pushButton_Cap_Cam1_2_clicked();
    void on_pushButton_Dir_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_new_from_2_clicked();
    void on_new_from_clicked();
private:
    Ui::CameraForm *ui;
    QTcpSocket *socket;
    QSerialPort *serial;
    MyThread* Mthread;
    QThread* th=new QThread();
    bool StopFlag=false;
    MyTemplate *myfm;
    QObjectCleanupHandler* m_cleanupHandler;

protected :
    void closeEvent(QCloseEvent *event);

};

#endif



#endif // CAMERAFORM_H
