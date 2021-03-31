//#include "cameraform.h"
//#include "ui_cameraform.h"
#include <QDebug>
//#pragma region 头文件
#include "cameraform.h"
#include "ui_cameraform.h"
#include "MyTemplate.h"
#include <QThread>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <fstream>
#include <string>
#include<QString>
//#pragma endregion
//#pragma region 接口函数
//设置工程路径
void CameraForm::setAppPath(const QString &path)
{
    basePath = path;
}
//切换产品=加載模板
int CameraForm::setProductID(const QString &id)
{  // 加載模板
    jiazai(id);
    return 0;
}
//拍照检测
//传值Type:
//2   :2D2判断泡沫方向
//3   :2D1定位
//4   :2D定位运算返回pos(x,y,z,Rx,Ry,Rz)
int CameraForm::checkProduct(const QString &id, int type, QString &picture) {
    return 0;
}

/**
返回值 -1:采图失败,系统停止
-2:ProductID与当前加载模板不一致
0:方向为反
1:方向为正
2:缺少泡沫
 */
int CameraForm::check2D1Dir(const QString &id, QString &picture)
{

    if(id != CurproductID)
    {
        appendMessage(QString("2D2----->ProductID not match!"));

        qDebug() << "2D2----->ProductID not match!" << CurproductID << id;
        return -2;
    }
    if (open2)
    {
        HTuple  hv_Exception;
        ho_Image2D2 = HObject();
        //采集图片
        bool isOK = xiangjicaiji(objDevicePtr2, WindowHandle2, &ho_Image2D1);
        if(!isOK)
        {
            appendMessage(QString("Camera2D2  Capture Fail!"));
            qDebug() << "Camera2D2  Capture Fail!";
            return -1;     //2D2采图失败!
        }
        try
        {
            QString Str_path = basePath+"Model_img/2D2/";
            picture = xiangjibaocun(ho_Image2D1, Str_path);

            //LWH算法2
            //2d1图像检测
            bool Result = DetectionPN(prodactID, ho_Image2D1);
            if(Result)
            {
                appendMessage(u8"Direction Right");
            }
            else
            {
                appendMessage( "Direction Wrong!!!" );
                appendMessage( QString("%1").arg(Result));
            }
            return Result;
        }
        catch (HalconCpp::HException &HDevExpDefaultException)
        {
            HDevExpDefaultException.ToHTuple(&hv_Exception);
            throw HalconCpp::HException(hv_Exception);
            return -1;
        }
    }
    else
    {
        qDebug() << "Camera2 Not Open!";
        appendMessage("Camera2 Not Open!");
        return -1;
    }
}
/**
返回值 -1:采图失败,系统停止
-2:ProductID与当前加载模板不一致
0:检测定位失败,系统停止
1:定位成功
 */
int CameraForm::check2D2pos(const QString &id, QString &picture,float &x, float &y, float &z, float &Rx, float &Ry, float &Rz)
{
    if(id != CurproductID)
    {
        appendMessage(QString("2D1---->ProductID not match!"));

        qDebug() << "2D1---->ProductID not match!";
        return -2;
    }
    if (open1)
    {
        QString Str_path;
        HTuple  hv_Exception;
        //采集图片
        ho_Image2D2 = HObject();
        bool isOK = xiangjicaiji(objDevicePtr1, WindowHandle, &ho_Image2D2);
        if(!isOK)
        {
            appendMessage(QString("Camera2D1  Capture Fail!"));

            qDebug() << "Camera2D1  Capture Fail!";
            return -1;     //2D1采图失败!
        }
        try
        {

            Str_path = basePath+"Model_img/2D1/";
            picture = xiangjibaocun(ho_Image2D2, Str_path);
            //LWH算法2
            //正面进行2D2定位
            //返回装配X，Y，Rz
            double *AssemblyPos;
            AssemblyPos = ImageLocation(prodactID, ho_Image2D2);
            //装配高度AssemblyZ
			double AssemblyZ = double(/*m_fRegCalibZ2*/  m_RegAdjustPos.z);
            double AssemblyPositin[6] = { AssemblyPos[0], AssemblyPos[1],AssemblyZ,180.0,0.0,AssemblyPos[2] };
            QString printeddata;
            for (int i = 0; i < 6; i++)
            {
                printeddata.append(QString("%1").arg(AssemblyPositin[i]));
                printeddata.append(",");
            }
            appendMessage(printeddata);

            //返回值转化
            if(AssemblyPositin[6] == 0)
            {
                qDebug() << "Camera1 Locate Fail!!";
                return 0;
            }
            x = AssemblyPositin[0];
            y = AssemblyPositin[1];
            z = AssemblyPositin[2];
            Rx = AssemblyPositin[3];
            Ry = AssemblyPositin[4];
            Rz = AssemblyPositin[5];
            return 1;
        }
        catch (HalconCpp::HException &HDevExpDefaultException)
        {
            HDevExpDefaultException.ToHTuple(&hv_Exception);
            throw HalconCpp::HException(hv_Exception);
            qDebug() << "Locate 2D1 Fail!";
            return 0;
        }
    }
    else {
        qDebug() << "Camera1 Not Open!";
        appendMessage("Camera1 Not Open!");

        return -1;
    }
}
/**
返回值 -1:采图失败,系统停止
-2:ProductID与当前加载模板不一致
0:检测定位失败,系统停止
1:定位成功
 */
int CameraForm::check2D1pos(const QString &id, QString &picture)
{
    if(id != CurproductID)
    {
        appendMessage(QString("2D2Locate----->ProductID not match!"));

        qDebug() << "2D2Locate----->ProductID not match!";
        return -2;
    }
    if (open2)
    {
        HTuple  hv_Exception;
        ho_Image2D2= HObject();
        //采集图片
        bool isOK = xiangjicaiji(objDevicePtr2, WindowHandle2, &ho_Image2D2);
        if(!isOK)
        {
            appendMessage(QString("Camera2D2  Capture Fail!"));

            qDebug() << "Camera2D2  Capture Fail!";
            return -1;     //2D2采图失败!
        }
        try
        {
            QString Str_path = basePath+"Model_img/2D2/";
            picture = xiangjibaocun(ho_Image2D2, Str_path);
            //LWH算法
            //2d1图像检测
            bool Result = Detection2D1(prodactID, ho_Image2D2);
            if(Result)
            {
                appendMessage("Locate 2D2 success");

            }
            return Result;
        }
        catch (HalconCpp::HException &HDevExpDefaultException)
        {
            HDevExpDefaultException.ToHTuple(&hv_Exception);
            throw HalconCpp::HException(hv_Exception);
            qDebug() << "Camera2 Locate Fail!!";
            return 0;
        }
    }
    else {
        qDebug() << "Camera2 Not Open!";
        appendMessage("Camera2 Not Open!");

        return -1;
    }
}


//显示模板列表
void CameraForm::showTemplates() {
    //TODO  show();
}
//添加模板
int CameraForm::addTemplate(const QString& id,int type) {
    return 0;
}
//获取界面

QWidget* CameraForm::settingsWt() {
    //    ui->setupUi(this);
    //    Initialization();
    return ui->groupBox_24;
}
//#pragma endregion

//#pragma region function
CameraForm::CameraForm(QWidget *parent) :QWidget(parent), ui(new Ui::CameraForm)
{
    ui->setupUi(this);
    ui->gvMain->setFixedSize(350,180);
    ui->gvMain_2->setFixedSize(350,180);

    //    QTimer::singleShot(100,this,SLOT( Initialization() ));
    //    Initialization();


    //信号槽方式拍照
    //绑定信号槽
    // connect(Ms,SIGNAL(Signal()),this,SLOT(cap_image()));
    //触发信号
    //Ms->test();
    //m_cleanupHandler->add(Ms);

    setWt(new MyTemplate());
    myfm->setWt(this);




	//add by xiao.s 2020/11/18

	connect(ui->lineEdit_Calib1, SIGNAL(Clicked()), this, SLOT(slotLineEditClicked()));
	connect(ui->lineEdit_Calib2, SIGNAL(Clicked()), this, SLOT(slotLineEditClicked()));
	connect(ui->pushButton_RegPos1, SIGNAL(clicked()), this, SLOT(slotRegPosBtnClicked()));
	connect(ui->pushButton_RegPos2, SIGNAL(clicked()), this, SLOT(slotRegPosBtnClicked()));
	m_bPosReg1 = false;
	m_bPosReg2 = false;


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ui->tableWidget1->setItem(i, j, new QTableWidgetItem());
			ui->tableWidget2->setItem(i, j, new QTableWidgetItem());
		}
	}
}

void CameraForm::slotLineEditClicked()
{
	QString path = QCoreApplication::applicationDirPath();
	path += "/../SmartGREE/Gree1118Img";

	if (sender() == ui->lineEdit_Calib1)
	{
		QString str = QFileDialog::getOpenFileName(this, "file", path, "Calibration Files(*.calib2D)");
		if (!str.isEmpty())
		{
			QFile file(str);
			file.open(QIODevice::ReadOnly);
			file.read((char*)(&m_PlaceCalib), sizeof(Calibration2D));
			file.close();

			ui->lineEdit_Calib1->setText(str);

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					ui->tableWidget1->item(i, j)->setText(QString::number(m_PlaceCalib.trans.mat[i * 3 + j]));

			ui->lineEdit_ScaleX1->setText(QString::number(m_PlaceCalib.trans.scale_x, 'f', 3));
			ui->lineEdit_ScaleY1->setText(QString::number(m_PlaceCalib.trans.scale_y, 'f', 3));
			ui->lineEdit_Shear1->setText(QString::number(m_PlaceCalib.trans.shear, 'f', 3));
			ui->lineEdit_Rotation1->setText(QString::number(m_PlaceCalib.trans.rotation, 'f', 3));
			ui->lineEdit_TransX1->setText(QString::number(m_PlaceCalib.trans.trans_x, 'f', 3));
			ui->lineEdit_TransY1->setText(QString::number(m_PlaceCalib.trans.trans_y, 'f', 3));

			ui->lineEdit_CalibRegPosX1->setText(QString::number(m_PlaceCalib.reg_pos.x, 'f', 3));
			ui->lineEdit_CalibRegPosY1->setText(QString::number(m_PlaceCalib.reg_pos.y, 'f', 3));
			ui->lineEdit_CalibRegPosZ1->setText(QString::number(m_PlaceCalib.reg_pos.z, 'f', 3));
			ui->lineEdit_CalibRegPosRx1->setText(QString::number(m_PlaceCalib.reg_pos.Rx, 'f', 3));
			ui->lineEdit_CalibRegPosRy1->setText(QString::number(m_PlaceCalib.reg_pos.Ry, 'f', 3));
			ui->lineEdit_CalibRegPosRz1->setText(QString::number(m_PlaceCalib.reg_pos.Rz, 'f', 3));

			switch (m_PlaceCalib.camera_install_type)
			{
			case FIXED_CAMERA:
				ui->label_CameraInstallType1->setText(QString::fromStdString("固定安装"));
				break;
			case TRANSLATIONAL_CAMERA:
				ui->label_CameraInstallType1->setText(QString::fromStdString("眼在腕上"));
				break;
			case TRANS_ROT_CAMERA:
				ui->label_CameraInstallType1->setText(QString::fromStdString("眼在手上"));
				break;
			default:
				break;
			}

			m_RegPlacePos = m_PlaceCalib.reg_pos;
		}
	}
	else if (sender() == ui->lineEdit_Calib2)
	{
		QString str = QFileDialog::getOpenFileName(this, "file", path, "Calibration Files(*.calib2D)");
		if (!str.isEmpty())
		{
			QFile file(str);
			file.open(QIODevice::ReadOnly);
			file.read((char*)(&m_AdjustCalib), sizeof(Calibration2D));
			file.close();

			ui->lineEdit_Calib2->setText(str);

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					ui->tableWidget2->item(i, j)->setText(QString::number(m_AdjustCalib.trans.mat[i * 3 + j]));

			ui->lineEdit_ScaleX2->setText(QString::number(m_AdjustCalib.trans.scale_x, 'f', 3));
			ui->lineEdit_ScaleY2->setText(QString::number(m_AdjustCalib.trans.scale_y, 'f', 3));
			ui->lineEdit_Shear2->setText(QString::number(m_AdjustCalib.trans.shear, 'f', 3));
			ui->lineEdit_Rotation2->setText(QString::number(m_AdjustCalib.trans.rotation, 'f', 3));
			ui->lineEdit_TransX2->setText(QString::number(m_AdjustCalib.trans.trans_x, 'f', 3));
			ui->lineEdit_TransY2->setText(QString::number(m_AdjustCalib.trans.trans_y, 'f', 3));

			ui->lineEdit_CalibRegPosX2->setText(QString::number(m_AdjustCalib.reg_pos.x, 'f', 3));
			ui->lineEdit_CalibRegPosY2->setText(QString::number(m_AdjustCalib.reg_pos.y, 'f', 3));
			ui->lineEdit_CalibRegPosZ2->setText(QString::number(m_AdjustCalib.reg_pos.z, 'f', 3));
			ui->lineEdit_CalibRegPosRx2->setText(QString::number(m_AdjustCalib.reg_pos.Rx, 'f', 3));
			ui->lineEdit_CalibRegPosRy2->setText(QString::number(m_AdjustCalib.reg_pos.Ry, 'f', 3));
			ui->lineEdit_CalibRegPosRz2->setText(QString::number(m_AdjustCalib.reg_pos.Rz, 'f', 3));

			switch (m_AdjustCalib.camera_install_type)
			{
			case FIXED_CAMERA:
				ui->label_CameraInstallType2->setText(QString::fromStdString("固定安装"));
				break;
			case TRANSLATIONAL_CAMERA:
				ui->label_CameraInstallType2->setText(QString::fromStdString("眼在腕上"));
				break;
			case TRANS_ROT_CAMERA:
				ui->label_CameraInstallType2->setText(QString::fromStdString("眼在手上"));
				break;
			default:
				break;
			}

			m_RegAdjustPos = m_AdjustCalib.reg_pos;
		}
	}
}

void CameraForm::slotRegPosBtnClicked()
{
	if (sender() == ui->pushButton_RegPos1)
	{
		if (m_bPosReg1)
		{

// 			string ss;
// 			QString s;
// 			ss = s.toLocal8Bit().toStdString();
// 			s = QString::fromLocal8Bit(ss.s_str());

			ui->pushButton_RegPos1->setText(QString::fromLocal8Bit("注册自定义位置"));
			ui->lineEdit_RegPosX1->setEnabled(true);
			ui->lineEdit_RegPosY1->setEnabled(true);
			ui->lineEdit_RegPosZ1->setEnabled(true);
			ui->lineEdit_RegPosRx1->setEnabled(true);
			ui->lineEdit_RegPosRy1->setEnabled(true);
			ui->lineEdit_RegPosRz1->setEnabled(true);
			m_bPosReg1 = false;

			m_RegPlacePos = m_PlaceCalib.reg_pos;
		}
		else
		{
			ui->pushButton_RegPos1->setText(QString::fromLocal8Bit("取消自定义位置"));
			ui->lineEdit_RegPosX1->setEnabled(false);
			ui->lineEdit_RegPosY1->setEnabled(false);
			ui->lineEdit_RegPosZ1->setEnabled(false);
			ui->lineEdit_RegPosRx1->setEnabled(false);
			ui->lineEdit_RegPosRy1->setEnabled(false);
			ui->lineEdit_RegPosRz1->setEnabled(false);
			m_bPosReg1 = true;

			m_RegPlacePos.x = ui->lineEdit_RegPosX1->text().toFloat();
			m_RegPlacePos.y = ui->lineEdit_RegPosY1->text().toFloat();
			m_RegPlacePos.z = ui->lineEdit_RegPosZ1->text().toFloat();
			m_RegPlacePos.Rx = ui->lineEdit_RegPosRx1->text().toFloat();
			m_RegPlacePos.Ry = ui->lineEdit_RegPosRy1->text().toFloat();
			m_RegPlacePos.Rz = ui->lineEdit_RegPosRz1->text().toFloat();
		}
	}
	else if (sender() == ui->pushButton_RegPos2)
	{

		if (m_bPosReg2)
		{
			ui->pushButton_RegPos2->setText(QString::fromLocal8Bit("注册自定义位置"));
			ui->lineEdit_RegPosX2->setEnabled(true);
			ui->lineEdit_RegPosY2->setEnabled(true);
			ui->lineEdit_RegPosZ2->setEnabled(true);
			ui->lineEdit_RegPosRx2->setEnabled(true);
			ui->lineEdit_RegPosRy2->setEnabled(true);
			ui->lineEdit_RegPosRz2->setEnabled(true);
			m_bPosReg2 = false;

			m_RegAdjustPos = m_AdjustCalib.reg_pos;
		}
		else
		{
			ui->pushButton_RegPos2->setText(QString::fromLocal8Bit("取消自定义位置"));
			ui->lineEdit_RegPosX2->setEnabled(false);
			ui->lineEdit_RegPosY2->setEnabled(false);
			ui->lineEdit_RegPosZ2->setEnabled(false);
			ui->lineEdit_RegPosRx2->setEnabled(false);
			ui->lineEdit_RegPosRy2->setEnabled(false);
			ui->lineEdit_RegPosRz2->setEnabled(false);
			m_bPosReg2 = true;

			m_RegAdjustPos.x = ui->lineEdit_RegPosX2->text().toFloat();
			m_RegAdjustPos.y = ui->lineEdit_RegPosY2->text().toFloat();
			m_RegAdjustPos.z = ui->lineEdit_RegPosZ2->text().toFloat();
			m_RegAdjustPos.Rx = ui->lineEdit_RegPosRx2->text().toFloat();
			m_RegAdjustPos.Ry = ui->lineEdit_RegPosRy2->text().toFloat();
			m_RegAdjustPos.Rz = ui->lineEdit_RegPosRz2->text().toFloat();
		}
	}
}


void CameraForm::readSerialData()
{
    QByteArray buf;
    buf = serial->readAll(); // serial->readAll().toHex() 转换成16进制形式
    //qDebug()<<buf;
    QString  str=tr(buf);
    //刷新显示
    appendMessage("收到串口消息:"+str);
}

void CameraForm::socket_Read_Data()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    if(!buffer.isEmpty())
    {
        //QString str = ui->textEdit_Recv->toPlainText();
        QString  str=tr(buffer);
        //刷新显示
        appendMessage("收到socket消息:"+str);

    }
}

void CameraForm::socket_Disconnected()
{
    //发送按键失能
    //ui->pushButton_Send->setEnabled(false);
    //修改按键文字
    ui->pushButton_Connect->setText("连接");
    //显示
    appendMessage("Disconnected!");

}

CameraForm::~CameraForm()
{
    closeCamera();
    delete ui;
}

void CameraForm::setWinSize(int w, int h)
{
//    Hlong windID = (Hlong)this->ui->gvMain->winId();//3、将控件和halcon窗体绑定起来，设定父窗体
//    Hlong windID_2 = (Hlong)this->ui->gvMain_2->winId();//3、将控件和halcon窗体绑定起来，设定父窗体

////    SetWindowAttr("background_color", "#2D2D2D");
////    OpenWindow(0, 0, w, h, windID, "visible", "", &WindowHandle);//windID作为父窗口  ，与halcon WindowHandle绑定
////    OpenWindow(0, 0, w, h, windID_2, "visible", "", &WindowHandle2);//windID作为父窗口  ，与halcon WindowHandle绑定

//    SetWindowAttr("background_color", "#2D2D2D");
//    OpenWindow(0, 0, w, h, windID, "visible", "", &WindowHandle);//windID作为父窗口  ，与halcon WindowHandle绑定
//    SetDraw(WindowHandle, "margin");
//    SetLineWidth(WindowHandle, 1);
//    SetColor(WindowHandle, "red");

}

void CameraForm::Initialization() {

    //初始化参数
    Initialization_Parameter();

    //初始化界面
    Initialization_Interface();

    //初始化通信
    Initialization_communication();

    //初始化相机
    Initialization_Camera();

    //文件读取
     //CaRead_file();

}

//窗口
void CameraForm::Initialization_Interface() {

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    Hlong windID = (Hlong)this->ui->gvMain->winId();//3、将控件和halcon窗体绑定起来，设定父窗体
    Hlong windID_2 = (Hlong)this->ui->gvMain_2->winId();//3、将控件和halcon窗体绑定起来，设定父窗体


//    int width = ui->gvMain->width();
//    int height = ui->gvMain->height();
    int width = 350;
    int height = 180;
    // qDebug()<<"ttttttttttt"<<  width << height << windID ;

    SetWindowAttr("background_color", "#2D2D2D");
    OpenWindow(0, 0, width, height, windID, "visible", "", &WindowHandle);//windID作为父窗口  ，与halcon WindowHandle绑定
    SetDraw(WindowHandle, "margin");
    SetLineWidth(WindowHandle, 1);
    SetColor(WindowHandle, "red");

    SetWindowAttr("background_color", "#2D2D2D");
    OpenWindow(0, 0, width, height, windID_2, "visible", "", &WindowHandle2);//windID作为父窗口  ，与halcon WindowHandle2绑定
    SetDraw(WindowHandle2, "margin");
    SetLineWidth(WindowHandle2, 1);
    SetColor(WindowHandle2, "red");

}

//参数
void CameraForm::Initialization_Parameter() {
    //窗口
    WindowHandle = NULL;
    WindowHandle2 = NULL;
    //model文件参数
    CurproductID = "";
//     m_fRegCalibX1 =0;
//     m_fRegCalibY1 =0;
//     m_fRegCalibY1 =0;
//     m_fRegCalibRz1 =0;
//     m_fPlaceTmplImgX1 =0;
//     m_fPlaceTmplImgY1 =0;
//     m_fPlaceTmplImgA1 =0;
//     m_fAdjustTmplImgX2 =0;
//     m_fAdjustTmplImgY2 =0;
//     m_fPlaceTmplImgA1 =0;
//     m_fRegCalibX2 =0;
//     m_fRegCalibY2 =0;
//     m_fRegCalibY2 =0;
//     m_fRegCalibRz2  =0;
    //相机开关
    open1 = FALSE, open2 = FALSE;


    //LWH模板全局参数
    prodactID = "00000";
    hv_HomMat2D1 = NULL, hv_RotateCenter1 = NULL, hv_ModelID1 = NULL;
    hv_HomMat2D2 = NULL, hv_RotateCenter2 = NULL, hv_ModelID2 = NULL;
    hv_robotTeach2D1 = NULL, hv_offsetData = NULL;
    hv_Qx1Test = NULL, hv_Qy1Test = NULL, hv_HomMat2D = NULL;
    hv_Qx2Test = NULL, hv_Qy2Test = NULL, hv_AngleTest = NULL;

    //图片参数
    Image2D1, Image2D2, ho_Image2D1, ho_Image2D2;

    //相机参数
    objDevicePtr1, objDevicePtr2;
    objFeatureControlPtr1, objFeatureControlPtr2;
    m_pBitmap = NULL;
}
//自动打开相机设备
void CameraForm::Initialization_Camera() {
    // TODO: 在此添加控件通知处理程序代码
    IGXFactory::GetInstance().Init();//相机初始化
    GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;//枚举设备
    IGXFactory::GetInstance() .UpdateDeviceList(1000, vectorDeviceInfo);
    int count = vectorDeviceInfo.size();
    //appendMessage(QString(count));

    if (count > 0)
    {
        for (int i = 0; i < count; i++)
        {

            openxiangji(i, vectorDeviceInfo);
        }
    }
    else
    {
        isHasCamera = 0;
        appendMessage(QString("NoCamera!!"));
    }
}
//通信
void CameraForm::Initialization_communication() {
    //socket通信
    //1、客户端的代码比服务器稍简单，总的来说，使用QT中的QTcpSocket类与服务器进行通信只需要以下5步：
    //（1）创建QTcpSocket套接字对象
    socket = new QTcpSocket();
    //连接信号槽
    //（4）当socket接收缓冲区有新数据到来时，会发出readRead()信号，因此为该信号添加槽函数以读取数据
    QObject::connect(socket, &QTcpSocket::readyRead, this, &CameraForm::socket_Read_Data);
    //（5）断开与服务器的连接（关于close()和disconnectFromHost()的区别，可以按F1看帮助）
    QObject::connect(socket, &QTcpSocket::disconnected, this, &CameraForm::socket_Disconnected);
    ui->pushButton_Send->setEnabled(false);
    ui->lineEdit_IP->setText("127.0.0.1");
    ui->lineEdit_Port->setText("60000");
}

//初始化文件读取
void CameraForm::CaRead_file() {
    HTuple modelbasepath = "../CalibrationFiles/";
    HTuple hv_HomMat2D1path = modelbasepath + "2D1_HomMat2D.txt";
    HTuple hv_HomMat2D2path = modelbasepath + "2D2_HomMat2D.txt";
    HTuple hv_RotateCenter1path = modelbasepath + "2D1_RotateCenter.txt";
    HTuple hv_RotateCenter2path = modelbasepath + "2D2_RotateCenter.txt";

    ReadTuple(hv_HomMat2D1path, &hv_HomMat2D1);
    ReadTuple(hv_RotateCenter1path, &hv_RotateCenter1);
    ReadTuple(hv_HomMat2D2path, &hv_HomMat2D2);
    ReadTuple(hv_RotateCenter2path, &hv_RotateCenter2);
}
#pragma endregion


//#pragma region 设备一
//曝光一
void CameraForm::on_pushButton_Save_Cam_Exposure_clicked()
{
    if(open1) {
        double Expo = ui->lineEdit_Exposure_1->text().toDouble();
        objFeatureControlPtr1->GetFloatFeature("ExposureTime")->SetValue(Expo);
        //显示
        appendMessage(QString("Set exposure successful!"));
    }
    else
    {

        //显示
        appendMessage(QString("Set exposure failed!"));


    }
}
//相机采集一
void CameraForm::on_pushButton_Cap_Cam1_clicked()
{
    if(open1){
        bool isOK = xiangjicaiji(objDevicePtr1, WindowHandle, &ho_Image2D1);
        QString Str_path = basePath+"gather_img/2D1/";
        QString picture = xiangjibaocun(ho_Image2D1, Str_path);
        appendMessage(u8"采图成功");
    }else{
        appendMessage(u8"相机未打开");
    }


}
//相机采集二
void CameraForm::on_pushButton_Cap_Cam1_2_clicked()
{
    if(open2){
        bool isOK = xiangjicaiji(objDevicePtr2, WindowHandle2, &ho_Image2D2);
        QString Str_path = basePath+"gather_img/2D2/";
        QString picture = xiangjibaocun(ho_Image2D2, Str_path);
        appendMessage(u8"采图成功");
    }else{
        appendMessage(u8"相机未打开");
    }
}
//2d1(打开图片)
void CameraForm::on_pushButton_clicked()
{
//     HTuple  hv_Width, hv_Height;
//     //主体函数   获取地址
//     QString path = QFileDialog::getOpenFileName(this, tr("选择图像"), "", tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
//     QTextCodec *code = QTextCodec::codecForName("GB2312");
//     if (path == "") return;
//     //定义变量
//     HTuple  FileNameModel;
//     //qt  QString  转化成 halcon  tuple文件
//     //FileNameModel      =  HTuple(path.toLatin1().data());
//     FileNameModel = HTuple(code->fromUnicode(path).data());
//     ReadImage(&Image2D1, FileNameModel);//只有读取图像，里面才有数据 ，halconimage
//     // ReadImage(&ho_Image, "E:/LKF/KuangLing_2D/1.bmp");
//     GetImageSize(Image2D1, &hv_Width, &hv_Height);
//     SetPart(WindowHandle, 0, 0, hv_Height, hv_Width); //设置窗口的大小
//     DispObj(Image2D1, WindowHandle);
//     appendMessage("Image loaded successfully!");




	//add by xiao.s 2020-11-19

	QString path = QCoreApplication::applicationDirPath();
	path += "/../SmartGREE/Model/gather_img/2D1/tmpl/";
	bool GrapImageOk = xiangjicaiji(objDevicePtr1, WindowHandle, &Image2D1);
	if (GrapImageOk)
	{
		xiangjibaocun(Image2D1, path);
	}


	HObject  ho_ROI_0, ho_ImageReduced;
	HTuple  hv_RowTest, hv_ColTest, hv_ScoreTest;
	HTuple  hv_AngleTest;

	GenRectangle1(&ho_ROI_0, 1133.599, 517.038, 2601.02, 3064.02);
	ReduceDomain(Image2D1, ho_ROI_0, &ho_ImageReduced);
	DispObj(ho_ImageReduced, WindowHandle);

	if (hv_ModelID1 != NULL)
	{
		FindShapeModel(Image2D1, hv_ModelID1, 0, HTuple(360).TupleRad(), 0, 1,
			0.5, "least_squares", 0, 0.9, &hv_RowTest, &hv_ColTest, &hv_AngleTest, &hv_ScoreTest);
		dev_display_shape_matching_results(hv_ModelID1, "red", hv_RowTest, hv_ColTest, hv_AngleTest, 1, 1, 0);

// 		m_fPlaceTmplImgX1 = hv_ColTest.D();
// 		m_fPlaceTmplImgY1 = hv_RowTest.D();
// 		m_fPlaceTmplImgA1 = -hv_AngleTest.D() * 180 / 3.14;

		m_PlaceTmplImgPos = MyPose2D(hv_ColTest.D(), hv_RowTest.D(), -hv_AngleTest.D() * 180 / 3.14);

	}

}
//2d2处理按钮
void CameraForm::on_pushButton_6_clicked()
{
//    try
//    {
//        //返回装配X，Y，Rz
//        double *AssemblyPos;
//        AssemblyPos = ImageLocation(prodactID, Image2D2);
//        //装配高度AssemblyZ
//        double AssemblyZ = atof(ArmX_2D1.c_str());
//        double AssemblyPositin[6] = { AssemblyPos[0], AssemblyPos[1],AssemblyZ,180.0,0.0,AssemblyPos[2] };
//        QString printeddata;
//        for (int i = 0; i < 6; i++)
//        {
//            printeddata.append(QString("%1").arg(AssemblyPositin[i]));
//            printeddata.append(",");
//        }
//        appendMessage(printeddata);
//    }
//    catch (...)
//    {
//        appendMessage(QString("Iamge not loaded"));

//    }


//     double *Result1;
//     try
//     {
//         Result1 = Match2D2(Image2D2, hv_ModelID2, &hv_AngleTest);
//         //////////////////////////定位结果计算
//         while (0 != (Result1[2]>180))
//         {
//             Result1[2] = Result1[2] - 360;
//         }
//         while (0 != (Result1[2] <= -180))
//         {
//             Result1[2] += 360;
//         }
// 
//     QString printeddata;
//     for (int i = 0; i < 3; i++)
//     {
//         printeddata.append(QString("%1").arg(Result1[i]));
//         printeddata.append(",");
//     }
//    appendMessage(printeddata);
//     }
//     catch (...)
//      {
//          appendMessage(QString("Iamge not loaded"));
// 
//      }







	//add by xiao.s  2020/11/19


// 	HTuple  hv_Width, hv_Height;
// 	QString path = QFileDialog::getOpenFileName(this, tr("选择图像"), "", tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
// 	QTextCodec *code = QTextCodec::codecForName("GB2312");
// 	if (path == "") return;
// 	//定义变量
// 	HTuple FileNameModel;
// 	//qt  QString  转化成 halcon  tuple文件
// 	//FileNameModel      =  HTuple(path.toLatin1().data());
// 	FileNameModel = HTuple(code->fromUnicode(path).data());
// 	ReadImage(&Image2D2, FileNameModel);//只有读取图像，里面才有数据 ，halconimage
// 										// ReadImage(&ho_Image, "E:/LKF/KuangLing_2D/1.bmp");
// 	GetImageSize(Image2D2, &hv_Width, &hv_Height);
// 	SetPart(WindowHandle2, 0, 0, hv_Height, hv_Width); //设置窗口的大小
// 	DispObj(Image2D2, WindowHandle2);
// 	appendMessage("Image loaded successfully!");


	QString path = QCoreApplication::applicationDirPath();
	path += "/../SmartGREE/Model/gather_img/2D2/scene/";
	bool GrapImageOk = xiangjicaiji(objDevicePtr2, WindowHandle2, &Image2D2);
	if (GrapImageOk)
	{
	//	xiangjibaocun(Image2D2, path);
	}


	HObject  ho_ROI_0, ho_ImageReduced;
	HTuple  hv_RowTest, hv_ColTest, hv_ScoreTest;
	HTuple  hv_AngleTest;

	GenRectangle1(&ho_ROI_0, 1133.599, 517.038, 2601.02, 3064.02);
	ReduceDomain(Image2D2, ho_ROI_0, &ho_ImageReduced);
	DispObj(ho_ImageReduced, WindowHandle2);

	if (hv_ModelID2 != NULL)
	{
		FindShapeModel(Image2D2, hv_ModelID2, 0, HTuple(360).TupleRad(), 0, 1,
			0.5, "least_squares", 0, 0.9, &hv_RowTest, &hv_ColTest, &hv_AngleTest, &hv_ScoreTest);
		dev_display_shape_matching_results2(hv_ModelID2, "red", hv_RowTest, hv_ColTest, hv_AngleTest, 1, 1, 0);



		m_AdjustSceneImgPos = MyPose2D(hv_ColTest.D(), hv_RowTest.D(), -hv_AngleTest.D() * 180 / 3.14);

// 		tmplImgPos = MyPose2D(m_fAdjustTmplImgX2, m_fAdjustTmplImgY2, m_fAdjustTmplImgA2);
// 		sceneImgPos = MyPose2D(hv_ColTest.D(), hv_RowTest.D(), -hv_AngleTest.D() * 180 / 3.14);
// 		regPlacePos = { m_fRegCalibX2, m_fRegCalibY2, m_fRegCalibZ2, 0, 0, m_fRegCalibRz2 };
	}


	float dx, dy, da;
	guideRobotAdjust2D(m_AdjustTmplImgPos, m_AdjustSceneImgPos, m_AdjustCalib, m_RegAdjustPos, dx, dy, da);
	QString Adjust2DResult = QString("%1，%2，%3").arg(dx).arg(dy).arg(da);
	appendMessage(Adjust2DResult);

}
//2D2结果运算模块
double* CameraForm::ImageLocation(QString prodactID, HObject Image2D2)
{
    double *Result;
    try
    {
        Result = Match2D2(Image2D2, hv_ModelID2, &hv_AngleTest);
        //////////////////////////定位结果计算
        while (0 != (Result[2]>180))
        {
            Result[2] = Result[2] - 360;
        }
        while (0 != (Result[2] <= -180))
        {
            Result[2] += 360;
        }
        return Result;
    }
    catch (...)
    {
        appendMessage(QString("Match2D2 Fail!!"));
        Result[0] = 0.0;
        Result[1] = 0.0;
        Result[2] = 0.0;
        return Result;
    }
}
//2D1匹配定位算法
double* CameraForm::Match2D1(HObject ho_ImageTest2D1, HTuple hv_ModelID1, HTuple *hv_Angle)
{
    double  *result;
    result = new double[3];
    HObject  ho_ROI_0, ho_ImageReduced;
    HTuple  hv_RowTest, hv_ColTest, hv_ScoreTest;
    GenRectangle1(&ho_ROI_0, 133.599, 517.038, 2601.02, 3064.02);
    ReduceDomain(ho_ImageTest2D1, ho_ROI_0, &ho_ImageReduced);
    DispObj(ho_ImageReduced, WindowHandle);
    FindShapeModel(ho_ImageTest2D1, hv_ModelID1, 0, HTuple(360).TupleRad(), 0, 1,
                    0.5, "least_squares", 0, 0.9, &hv_RowTest, &hv_ColTest, &(*hv_Angle), &hv_ScoreTest);
     dev_display_shape_matching_results(hv_ModelID1, "red", hv_RowTest, hv_ColTest,
                                        (*hv_Angle), 1, 1, 0);
     Calibration2D cal2D1;
     QFile calFile1("C:/Users/Administrator/Desktop/Smart/2d1.calib2d");
     calFile1.open(QIODevice::ReadOnly);
     calFile1.read((char*)(&cal2D1), sizeof(Calibration2D));
     calFile1.close();


 //    tmplImgPos = MyPose2D (m_fPlaceTmplImgX1,m_fPlaceTmplImgY1,m_fPlaceTmplImgA1);
	
//      QString S1= QString("%1,%2,%3").arg(m_fPlaceTmplImgX1).arg(m_fPlaceTmplImgY1).arg(m_fPlaceTmplImgA1);
//      QString S= QString("%1,%2,%3").arg(hv_RowTest.D()).arg(hv_ColTest.D()).arg((*hv_Angle).D());
//      appendMessage(S);
//      appendMessage(S1);
//     regPickPos ={m_fRegCalibX1,m_fRegCalibY1,m_fRegCalibZ1,0,0,m_fRegCalibRz1};


	 float dx, dy, da;
	 m_PlaceSceneImgPos = MyPose2D(hv_RowTest.D(), hv_ColTest.D(), (*hv_Angle).D());
//	 guideRobotPick2D(m_PlaceTmplImgPos, m_PlaceSceneImgPos, cal2D1, m_RegPlacePos, dx, dy, da);
	 guideRobotPick2D(m_PlaceTmplImgPos, m_PlaceSceneImgPos, cal2D1, cal2D1.reg_pos, m_RegPlacePos, dx, dy, da);
     result[0] = dx;
     result[1] = dy;
     result[2] = da;
     return result;
}

// 2D2(打开图片)
void CameraForm::on_pushButton_5_clicked()
{
	//     HTuple  hv_Width, hv_Height;
	//     QString path = QFileDialog::getOpenFileName(this, tr("选择图像"), "", tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
	//     QTextCodec *code = QTextCodec::codecForName("GB2312");
	//     if (path == "") return;
	//     //定义变量
	//     HTuple FileNameModel;
	//     //qt  QString  转化成 halcon  tuple文件
	//     //FileNameModel      =  HTuple(path.toLatin1().data());
	//     FileNameModel = HTuple(code->fromUnicode(path).data());
	//     ReadImage(&Image2D2, FileNameModel);//只有读取图像，里面才有数据 ，halconimage
	//     // ReadImage(&ho_Image, "E:/LKF/KuangLing_2D/1.bmp");
	//     GetImageSize(Image2D2, &hv_Width, &hv_Height);
	//     SetPart(WindowHandle2, 0, 0, hv_Height, hv_Width); //设置窗口的大小
	//     DispObj(Image2D2, WindowHandle2);
	//     appendMessage("Image loaded successfully!");


		//add by xiao.s  2020/11/19
	QString path = QCoreApplication::applicationDirPath();
	path += "/../SmartGREE/Model/gather_img/2D2/tmpl/";
	bool GrapImageOk = xiangjicaiji(objDevicePtr2, WindowHandle2, &Image2D2);
	if (GrapImageOk)
	{
		xiangjibaocun(Image2D2, path);
	}


	HObject  ho_ROI_0, ho_ImageReduced;
	HTuple  hv_RowTest, hv_ColTest, hv_ScoreTest;
	HTuple  hv_AngleTest;

	GenRectangle1(&ho_ROI_0, 1133.599, 517.038, 2601.02, 3064.02);
	ReduceDomain(Image2D2, ho_ROI_0, &ho_ImageReduced);
	DispObj(ho_ImageReduced, WindowHandle2);

	if (hv_ModelID2 != NULL)
	{
		FindShapeModel(Image2D2, hv_ModelID2, 0, HTuple(360).TupleRad(), 0, 1,
			0.5, "least_squares", 0, 0.9, &hv_RowTest, &hv_ColTest, &hv_AngleTest, &hv_ScoreTest);
		dev_display_shape_matching_results2(hv_ModelID2, "red", hv_RowTest, hv_ColTest, hv_AngleTest, 1, 1, 0);

// 		m_fAdjustTmplImgX2 = hv_ColTest.D();
// 		m_fAdjustTmplImgY2 = hv_RowTest.D();
// 		m_fAdjustTmplImgA2 = -hv_AngleTest.D() * 180 / 3.14;



		m_AdjustTmplImgPos = MyPose2D(hv_ColTest.D(), hv_RowTest.D(), -hv_AngleTest.D() * 180 / 3.14);
	}
}

//2d1处理按钮
void CameraForm::on_pushButton_7_clicked()
{
//     double *Result;
//     try
//     {
//         Result = Match2D1(Image2D1, hv_ModelID1, &hv_AngleTest);
//         //////////////////////////定位结果计算
//         while (0 != (Result[2]>180))
//         {
//             Result[2] = Result[2] - 360;
//         }
//         while (0 != (Result[2] <= -180))
//         {
//             Result[2] += 360;
//         }
// 
//      QString printeddata;
//     for (int i = 0; i < 3; i++)
//     {
//         printeddata.append(QString("%1").arg(Result[i]));
//         printeddata.append(",");
//     }
//    appendMessage(printeddata);
//     }
//     catch (...)
//     {
//         appendMessage(QString("Iamge not loaded"));
//     }



	//add by xiao.s  2020/11/19

// 	HTuple  hv_Width, hv_Height;
// 	QString path = QFileDialog::getOpenFileName(this, tr("选择图像"), "", tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
// 	QTextCodec *code = QTextCodec::codecForName("GB2312");
// 	if (path == "") return;
// 	//定义变量
// 	HTuple FileNameModel;
// 	//qt  QString  转化成 halcon  tuple文件
// 	//FileNameModel      =  HTuple(path.toLatin1().data());
// 	FileNameModel = HTuple(code->fromUnicode(path).data());
// 	ReadImage(&Image2D2, FileNameModel);//只有读取图像，里面才有数据 ，halconimage
// 										// ReadImage(&ho_Image, "E:/LKF/KuangLing_2D/1.bmp");
// 	GetImageSize(Image2D2, &hv_Width, &hv_Height);
// 	SetPart(WindowHandle2, 0, 0, hv_Height, hv_Width); //设置窗口的大小
// 	DispObj(Image2D2, WindowHandle2);
// 	appendMessage("Image loaded successfully!");

	QString path = QCoreApplication::applicationDirPath();
	path += "/../SmartGREE/Model/gather_img/2D1/scene/";
	bool GrapImageOk = xiangjicaiji(objDevicePtr1, WindowHandle, &Image2D1);
	if (GrapImageOk)
	{
		//	xiangjibaocun(Image2D2, path);
	}


	HObject  ho_ROI_0, ho_ImageReduced;
	HTuple  hv_RowTest, hv_ColTest, hv_ScoreTest;
	HTuple  hv_AngleTest;

	GenRectangle1(&ho_ROI_0, 1133.599, 517.038, 2601.02, 3064.02);
	ReduceDomain(Image2D1, ho_ROI_0, &ho_ImageReduced);
	DispObj(ho_ImageReduced, WindowHandle2);

	if (hv_ModelID1 != NULL)
	{
		FindShapeModel(Image2D1, hv_ModelID1, 0, HTuple(360).TupleRad(), 0, 1,
			0.5, "least_squares", 0, 0.9, &hv_RowTest, &hv_ColTest, &hv_AngleTest, &hv_ScoreTest);
		dev_display_shape_matching_results(hv_ModelID1, "red", hv_RowTest, hv_ColTest, hv_AngleTest, 1, 1, 0);

		m_PlaceSceneImgPos = MyPose2D(hv_ColTest.D(), hv_RowTest.D(), -hv_AngleTest.D() * 180 / 3.14);
	}


	float dx, dy, da;
//	guideRobotPick2D(tmplImgPos, sceneImgPos, m_PlaceCalib, regPlacePos, dx, dy, da);

	guideRobotPick2D(m_PlaceTmplImgPos, m_PlaceSceneImgPos, m_PlaceCalib, m_PlaceCalib.reg_pos, m_RegPlacePos, dx, dy, da);
	QString place1DResult = QString("%1，%2，%3").arg(dx).arg(dy).arg(da);
	appendMessage(place1DResult);

}

//2D1图像处理
bool CameraForm::Detection2D1(QString prodactID, HObject HImage2D1) {
    if (prodactID == CurproductID) {
        //ImageProcess();//图像处理
        try
        {
            //Match2D1( HImage2D1, hv_ModelID1, hv_HomMat2D1, &(hv_Qx1Test), &(hv_Qy1Test), &(hv_HomMat2D));
            appendMessage(u8"处理成功");
            return 1;
        }
        catch (...)
        {
            appendMessage(QString("Match2D2 Fail!!"));
            return 0;
        }
    }
    else {
        //创建模板
        return 0;
    }
}
//2D2匹配定位算法
double* CameraForm::Match2D2(HObject ho_ImageTest2D2, HTuple hv_ModelID2, HTuple *hv_AngleTest)
{
	double  *result;
	result = new double[3];
	HObject  ho_ROI_0, ho_ImageReduced;
	HTuple  hv_RowTest, hv_ColTest, hv_ScoreTest;
	GenRectangle1(&ho_ROI_0, 1133.599, 517.038, 2601.02, 3064.02);
	ReduceDomain(ho_ImageTest2D2, ho_ROI_0, &ho_ImageReduced);
	DispObj(ho_ImageReduced, WindowHandle2);
	FindShapeModel(ho_ImageTest2D2, hv_ModelID2, 0, HTuple(360).TupleRad(), 0, 1,
		0.5, "least_squares", 0, 0.9, &hv_RowTest, &hv_ColTest, hv_AngleTest, &hv_ScoreTest);
	dev_display_shape_matching_results2(hv_ModelID2, "red", hv_RowTest, hv_ColTest,
		(*hv_AngleTest), 1, 1, 0);




	m_PlaceSceneImgPos = MyPose2D(hv_ColTest.D(), hv_RowTest.D(), hv_AngleTest->D());


//  guideRobotPick2D(tmplImgPos, sceneImgPos, m_AdjustCalib, regPickPos,  dx, dy, da);

	float dx, dy, da;
	guideRobotPick2D(m_PlaceTmplImgPos, m_PlaceSceneImgPos, m_PlaceCalib, m_PlaceCalib.reg_pos, m_RegPlacePos, dx, dy, da);

    result[0] = dx;
    result[1] = dy;
    result[2] = da;
    return result;
}

//底托正反检测
int CameraForm::DetectionPN(QString prodactID, HObject ho_Image2D1) {
    if (prodactID != CurproductID)
    {
        return -1;
    }
    HTuple   hv_MatchROI2D2, hv_DetectResult;
    hv_MatchROI2D2.Clear();
    hv_MatchROI2D2[0] = 770;
    hv_MatchROI2D2[1] = 740;
    hv_MatchROI2D2[2] = 1820;
    hv_MatchROI2D2[3] = 1720;
    DirectionDetect(ho_Image2D1, hv_NCCModel2D12, hv_NCCModel2D11, hv_MatchROI2D2, &hv_DetectResult);
    //*检测结果DetectResult:
    //0:报错,没有底托泡沫
    //1:ok,方向正确
    //2:NG,方向为反
    if(hv_DetectResult == HTuple(1))
    {
        return 1;
    }
    else if(hv_DetectResult == HTuple(2) )
    {
        return 2;
    }
    return 0;

}

//#pragma endregion

//#pragma region 设备二
//曝光二
void CameraForm::on_pushButton_Save_Cam_Exposure_2_clicked()
{
    if(open2) {
        double Expo = ui->lineEdit_Exposure_2->text().toDouble();
        objFeatureControlPtr2->GetFloatFeature("ExposureTime")->SetValue(Expo);
        //显示
        appendMessage("Set exposure successful!");
    }
    else
    {

        //显示
        appendMessage(QString("Set exposure failed!"));


    }
}


//#pragma endregion


//#pragma region 设备三
//#pragma endregion

//#pragma region 通信
void CameraForm::on_pushButton_Connect_clicked()
{

    if (ui->pushButton_Connect->text() == tr("连接"))
    {
        QString IP;
        int port;
        //获取IP地址
        IP = ui->lineEdit_IP->text();
        //获取端口号
        port = ui->lineEdit_Port->text().toInt();
        //取消已有的连接
        socket->abort();
        //（2）使用这个对象连接服务器
        //连接服务器
        socket->connectToHost(IP, port);
        //等待连接成功
        if (!socket->waitForConnected(3000))
        {
            appendMessage("Socket Connection failed!");

            return;
        }
        appendMessage("Socket Connect successfully!");

        //发送按键使能
        ui->pushButton_Send->setEnabled(true);
        //修改按键文字
        ui->pushButton_Connect->setText("断开连接");
    }
    else
    {
        //断开连接
        socket->disconnectFromHost();
        //修改按键文字
        ui->pushButton_Connect->setText("连接");
        ui->pushButton_Send->setEnabled(false);
    }

}
void CameraForm::on_pushButton_Send_clicked()
{
    //获取文本框内容并以ASCII码形式发送
    //（3）使用write函数向服务器发送数据
    socket->write(ui->textEdit_Send->toPlainText().toLatin1());
    socket->flush();
    appendMessage("Send:" + ui->textEdit_Send->toPlainText());

}
void CameraForm::on_pushButton_Open_serial_clicked()
{
    //串口通信
    serial = new QSerialPort;
    serial->setPortName((QString)"COM3");  // 指定端口
    serial->setBaudRate(QSerialPort::Baud9600); // 设置波特率
    serial->setDataBits(QSerialPort::Data8); // 设置位数
    serial->setParity(QSerialPort::NoParity);// 设置无校验
    //serial->setParity(QSerialPort::EvenParity); // 设置偶校验
    serial->setStopBits(QSerialPort::OneStop); // 设置一个停止位
    serial->setFlowControl(QSerialPort::NoFlowControl); // 设置无流控制
    serial->open(QIODevice::ReadWrite); // 读写模式打开串口
    connect(serial, SIGNAL(readyRead()), this, SLOT(readSerialData())); // 连接串口的信号和槽函数
    appendMessage("打开串口成功！");
}
//#pragma endregion


//#pragma region 模板
//打印日志 关闭设备
void CameraForm::closeEvent(QCloseEvent * event)
{
    closeCamera();
}

//加载模板数据
void CameraForm::on_pushButton_2_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "选择文件夹", file_Path);
    qDebug()<<__FUNCTION__<<"open file"<<  path;
    if(path.isEmpty() || !QFile::exists(path)){
        appendMessage(file_Path);

    }else{
        jiazai(path);
    }
}
//打开文件按钮
int CameraForm::jiazai(QString id){
    SetSystem("clip_region", "false");
    prodactName = id;
    file_Path = prodactName;
    string temp;
    string str;
    QString s;
    QString ss;
    str = file_Path.toStdString();
    ifstream myfile(str);
    if(!QFile::exists(file_Path)){
        qDebug()<<u8"產品沒有建立模板";
        return -1;
    };
    while (getline(myfile, temp)) //按行读取字符串
    {
        s = QString::fromStdString(temp);
        ss = s;
        appendMessage(s);

        if (temp.find("ProdatID") != string::npos) {
			CurproductID = QString::fromStdString(string_crop(temp));
        }
//         if (temp.find("ArmX_2D1") != string::npos) 
// 		{
// 			m_RegPlacePos.x = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("ArmY_2D1 ") != string::npos) 
// 		{
// 			m_RegPlacePos.y = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("ArmZ_2D1 ") != string::npos) 
// 		{
// 			m_RegPlacePos.z = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("RZ_2D1 ") != string::npos) 
// 		{
// 			m_RegPlacePos.Rz = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("ArmX_2D2") != string::npos) 
// 		{
// 			m_RegAdjustPos.x = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("ArmY_2D2 ") != string::npos) 
// 		{
// 			m_RegAdjustPos.y = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("ArmZ_2D2 ") != string::npos) 
// 		{
// 			m_RegAdjustPos.z = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("RZ_2D2 ") != string::npos) 
// 		{
// 			m_RegAdjustPos.Rz = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("Qx1") != string::npos) 
// 		{
// 			m_fPlaceTmplImgX1 = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("Qy1") != string::npos)
// 		{
// 			m_fPlaceTmplImgY1 = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("Rz2D1") != string::npos) 
// 		{
// 			m_fPlaceTmplImgA1 = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("Qx2") != string::npos)
// 		{
// 			m_fAdjustTmplImgX2 = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("Qy2") != string::npos) 
// 		{
// 			m_fAdjustTmplImgY2 = QString::fromStdString(string_crop(temp)).toFloat();
//         }
//         if (temp.find("Rz2D2") != string::npos) 
// 		{
// 			m_fAdjustTmplImgA2 = QString::fromStdString(string_crop(temp)).toFloat();
//         }
    }
    myfile.close();
    //获取lua文件根目录
    const size_t last_slash_idx = str.rfind('/');
    string Mbasepath = str.substr(0, last_slash_idx + 1);
    const char *Tempbasepath = Mbasepath.data();
    HTuple hv_modelpath(Tempbasepath);
    // Local control variables
    HTuple  hv_HomMat2D1path, hv_HomMat2D2path;
    HTuple  hv_RotateCenter1path, hv_RotateCenter2path;
    HTuple  hv_offsetDatapath, hv_D2SHMROIpath;
    HTuple  hv_nccmodelcenterpath;

//    hv_HomMat2D1path = hv_modelpath+"2D1_HomMat2D.txt";
//    hv_HomMat2D2path = hv_modelpath+"2D2_HomMat2D.txt";
//    hv_RotateCenter1path = hv_modelpath+"2D1_RotateCenter.txt";
//    hv_RotateCenter2path = hv_modelpath+"2D2_RotateCenter.txt";
//    hv_Model2D1path = hv_modelpath+"2D1_deformable_model.dfm";
//     hv_robotTeach2D1path = hv_modelpath+"robotTeach2D1.dat";
//   hv_offsetDatapath = hv_modelpath+"offset.dat";
//   hv_nccmodelcenterpath = hv_modelpath+"nccmodelcenter.dat";

    HTuple NCCModel2D1path1 = hv_modelpath +"2D1_NCCModle1.ncm";
    HTuple NCCModel2D1path2 = hv_modelpath+"2D1_NCCModle2.ncm";
    HTuple Model2D2path = hv_modelpath+"2D2_shapeModle.sfm";
    HTuple Model2D1path = hv_modelpath+"2D1_shapeModle.sfm";
    //****加载模板参数
    //2D1仿射变换矩阵
    //ReadTuple(hv_HomMat2D1path, &hv_HomMat2D1);
    //2D1空间坐标系下机械臂法兰盘中心坐标
    //ReadTuple(hv_RotateCenter1path, &hv_RotateCenter1);
    //ReadTuple(hv_HomMat2D2path, &hv_HomMat2D2);
    //ReadTuple(hv_RotateCenter2path, &hv_RotateCenter2);
    //读取模板



	ReadNccModel(NCCModel2D1path1, &hv_NCCModel2D11);
	ReadNccModel(NCCModel2D1path2, &hv_NCCModel2D12);

	HTuple tuple;
	ReadShapeModel(Model2D2path, &tuple);

	ReadShapeModel(Model2D2path, &hv_ModelID2);
    ReadShapeModel(Model2D1path, &hv_ModelID1);
   
   


	bool flag1 = hv_ModelID1 == NULL;

	bool flag2 = hv_ModelID2 == NULL;

    //读取2D1拍照位
    //ReadTuple(hv_robotTeach2D1path, &hv_robotTeach2D1);
    //读取参数文档   2D1空间坐标系再2D2空间坐标系坐标：Tx0,Ty0
    //坐标偏置：deltaX,deltaY
    //ReadTuple(hv_offsetDatapath, &hv_offsetData);
    //读取匹配ROI
    //hv_D2SHMROIpath = hv_modelpath+"2D2_SHMROI.hobj";
    //ReadRegion(&ho_SHMROI, hv_D2SHMROIpath);
    //读取ncc模板中心
    //ReadTuple(hv_nccmodelcenterpath, &hv_nccmodelCenter);
}

void CameraForm::appendMessage(const QString &msg)
{
    QString str=QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") + " " +msg;
    ui->textBrowser->append(str);
    qDebug()<< str;
}

void CameraForm::closeCamera()
{
    qDebug()<<__FUNCTION__"test===";
    if (open1)
        objDevicePtr1->Close();
    if (open2)
        objDevicePtr2->Close();
}
//字符串截取
string CameraForm::string_crop(string name) {
    unsigned int iSize = name.size();
    unsigned int i = 0;
    string name_;
    for (i = 0; i < iSize; i++) {
        if (' ' == name[i]) {
            name_ += &name[i + 1];
        }
    }
    return name_;
}
//replace 字符串替换
void CameraForm::string_replace(std::string &strBig, const std::string &strsrc, const std::string &strdst) {
    std::string::size_type pos = 0;
    std::string::size_type srclen = strsrc.size();
    std::string::size_type dstlen = strdst.size();

    while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
    {
        strBig.replace(pos, srclen, strdst);
        pos += dstlen;
    }
}
//显示结果
void CameraForm::dev_display_shape_matching_results (HTuple hv_ModelID, HTuple hv_Color, HTuple hv_Row,
                                                     HTuple hv_Column, HTuple hv_Angle, HTuple hv_ScaleR, HTuple hv_ScaleC, HTuple hv_Model)
{

    // Local iconic variables
    HObject  ho_ModelContours, ho_ContoursAffinTrans;

    // Local control variables
    HTuple  hv_NumMatches, hv_Index, hv_Match, hv_HomMat2DIdentity;
    HTuple  hv_HomMat2DScale, hv_HomMat2DRotate, hv_HomMat2DTranslate;

    //This procedure displays the results of Shape-Based Matching.
    //
    hv_NumMatches = hv_Row.TupleLength();
    if (0 != (hv_NumMatches>0))
    {
        if (0 != ((hv_ScaleR.TupleLength())==1))
        {
            TupleGenConst(hv_NumMatches, hv_ScaleR, &hv_ScaleR);
        }
        if (0 != ((hv_ScaleC.TupleLength())==1))
        {
            TupleGenConst(hv_NumMatches, hv_ScaleC, &hv_ScaleC);
        }
        if (0 != ((hv_Model.TupleLength())==0))
        {
            TupleGenConst(hv_NumMatches, 0, &hv_Model);
        }
        else if (0 != ((hv_Model.TupleLength())==1))
        {
            TupleGenConst(hv_NumMatches, hv_Model, &hv_Model);
        }
        {
            HTuple end_val15 = (hv_ModelID.TupleLength())-1;
            HTuple step_val15 = 1;
            for (hv_Index=0; hv_Index.Continue(end_val15, step_val15); hv_Index += step_val15)
            {
                GetShapeModelContours(&ho_ModelContours, HTuple(hv_ModelID[hv_Index]), 1);
                if (HDevWindowStack::IsOpen())
                    SetColor(HDevWindowStack::GetActive(),HTuple(hv_Color[hv_Index%(hv_Color.TupleLength())]));
                {
                    HTuple end_val18 = hv_NumMatches-1;
                    HTuple step_val18 = 1;
                    for (hv_Match=0; hv_Match.Continue(end_val18, step_val18); hv_Match += step_val18)
                    {
                        if (0 != (hv_Index==HTuple(hv_Model[hv_Match])))
                        {
                            HomMat2dIdentity(&hv_HomMat2DIdentity);
                            HomMat2dScale(hv_HomMat2DIdentity, HTuple(hv_ScaleR[hv_Match]), HTuple(hv_ScaleC[hv_Match]),
                                          0, 0, &hv_HomMat2DScale);
                            HomMat2dRotate(hv_HomMat2DScale, HTuple(hv_Angle[hv_Match]), 0, 0, &hv_HomMat2DRotate);
                            HomMat2dTranslate(hv_HomMat2DRotate, HTuple(hv_Row[hv_Match]), HTuple(hv_Column[hv_Match]),
                                              &hv_HomMat2DTranslate);
                            AffineTransContourXld(ho_ModelContours, &ho_ContoursAffinTrans, hv_HomMat2DTranslate);
                            DispObj(ho_ContoursAffinTrans, WindowHandle);
                        }
                    }
                }
            }
        }
    }
    return;
}

void CameraForm::dev_display_shape_matching_results2(HTuple hv_ModelID, HTuple hv_Color, HTuple hv_Row,
                                                     HTuple hv_Column, HTuple hv_Angle, HTuple hv_ScaleR, HTuple hv_ScaleC, HTuple hv_Model)
{

    // Local iconic variables
    HObject  ho_ModelContours, ho_ContoursAffinTrans;

    // Local control variables
    HTuple  hv_NumMatches, hv_Index, hv_Match, hv_HomMat2DIdentity;
    HTuple  hv_HomMat2DScale, hv_HomMat2DRotate, hv_HomMat2DTranslate;

    //This procedure displays the results of Shape-Based Matching.
    //
    hv_NumMatches = hv_Row.TupleLength();
    if (0 != (hv_NumMatches>0))
    {
        if (0 != ((hv_ScaleR.TupleLength())==1))
        {
            TupleGenConst(hv_NumMatches, hv_ScaleR, &hv_ScaleR);
        }
        if (0 != ((hv_ScaleC.TupleLength())==1))
        {
            TupleGenConst(hv_NumMatches, hv_ScaleC, &hv_ScaleC);
        }
        if (0 != ((hv_Model.TupleLength())==0))
        {
            TupleGenConst(hv_NumMatches, 0, &hv_Model);
        }
        else if (0 != ((hv_Model.TupleLength())==1))
        {
            TupleGenConst(hv_NumMatches, hv_Model, &hv_Model);
        }
        {
            HTuple end_val15 = (hv_ModelID.TupleLength())-1;
            HTuple step_val15 = 1;
            for (hv_Index=0; hv_Index.Continue(end_val15, step_val15); hv_Index += step_val15)
            {
                GetShapeModelContours(&ho_ModelContours, HTuple(hv_ModelID[hv_Index]), 1);
                if (HDevWindowStack::IsOpen())
                    SetColor(HDevWindowStack::GetActive(),HTuple(hv_Color[hv_Index%(hv_Color.TupleLength())]));
                {
                    HTuple end_val18 = hv_NumMatches-1;
                    HTuple step_val18 = 1;
                    for (hv_Match=0; hv_Match.Continue(end_val18, step_val18); hv_Match += step_val18)
                    {
                        if (0 != (hv_Index==HTuple(hv_Model[hv_Match])))
                        {
                            HomMat2dIdentity(&hv_HomMat2DIdentity);
                            HomMat2dScale(hv_HomMat2DIdentity, HTuple(hv_ScaleR[hv_Match]), HTuple(hv_ScaleC[hv_Match]),
                                          0, 0, &hv_HomMat2DScale);
                            HomMat2dRotate(hv_HomMat2DScale, HTuple(hv_Angle[hv_Match]), 0, 0, &hv_HomMat2DRotate);
                            HomMat2dTranslate(hv_HomMat2DRotate, HTuple(hv_Row[hv_Match]), HTuple(hv_Column[hv_Match]),
                                              &hv_HomMat2DTranslate);
                            AffineTransContourXld(ho_ModelContours, &ho_ContoursAffinTrans, hv_HomMat2DTranslate);
                            DispObj(ho_ContoursAffinTrans, WindowHandle2);
                        }
                    }
                }
            }
        }
    }
    return;
}
//打开相机
void CameraForm::openxiangji(int i, GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo)
{
    //打开链表中的第一个设备
    GxIAPICPP::gxstring MAC2 = "NR0180050026";
    GxIAPICPP::gxstring MAC1 = "00-21-49-02-57-9A";
    switch (i)
    {
    case 0:
		objDevicePtr1 = IGXFactory::GetInstance().OpenDeviceByMAC(MAC1, GX_ACCESS_EXCLUSIVE);
		objFeatureControlPtr1 = objDevicePtr1->GetRemoteFeatureControl();
		open1 = TRUE;
        break;
    case 1:
        objDevicePtr2 = IGXFactory::GetInstance().OpenDeviceBySN(MAC2, GX_ACCESS_EXCLUSIVE);
        objFeatureControlPtr2 = objDevicePtr2->GetRemoteFeatureControl();
        open2 = TRUE;
        break;
    }
    //显示
    appendMessage(QString("CameraopenSuccess!!!"));
}
//图片采集
bool CameraForm::xiangjicaiji(CGXDevicePointer objDevicePtr, HTuple window, HObject *imgReturn)
{
    CGXBitmap*  m_pBitmap = NULL;
    HObject img;
    HTuple  hv_Width, hv_Height;
    //MessageBox(strSN);
    CGXStreamPointer objStreamPtr;
    CGXFeatureControlPointer objFeatureControlPtr;
    CImageDataPointer objImageDataPtr;
    objStreamPtr = objDevicePtr->OpenStream(0);
    objFeatureControlPtr = objDevicePtr->GetRemoteFeatureControl();
    //objFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(100000);
    //开启流通道采集
    objStreamPtr->StartGrab();
    //给设备发送开采命令
    objFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
    //采单帧
    objImageDataPtr = objStreamPtr->GetImage(500);//超时时间使用500ms，用户可以自行设定
    if (objImageDataPtr->GetStatus() == GX_FRAME_STATUS_SUCCESS)
    {
        //std::string      m_strSavePath = "./";
        //SYSTEMTIME   sysTime;                   ///< 系统时间
        if (m_pBitmap == NULL)
        {
            m_pBitmap = new CGXBitmap(objDevicePtr);
        }
        m_pBitmap->ToBmp(objImageDataPtr, &img);
        GetImageSize(img, &hv_Width, &hv_Height);
        //设置对象显示的颜色
        SetColor(window, "blue");
        //通过改变图像的缩放来适应图像在窗口的正常显示
        SetPart(window, 0, 0, hv_Height, hv_Width);
        //在窗口上显示图像
        ClearWindow(window);
        DispObj(img, window);
        //停采
        objFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
        objStreamPtr->StopGrab();
        //关闭流通道
        objStreamPtr->Close();
        //关闭设备
        //objDevicePtr->Close();
        *imgReturn = img.Clone();

        return true;
    }
    return false;


}
//保存图片
    QString CameraForm::xiangjibaocun(HObject img, QString path) {
    QDateTime dateTime = QDateTime::currentDateTime();
    QString MyDataTime = dateTime.toString("yyyy-MM-dd-hh-mm-ss");
    QString str_path = path + MyDataTime + ".png";
    HTuple str = str_path.toStdString().c_str();
    WriteImage(img, "png", 0, str);
    return str_path;
}
//正反检测按钮三
void CameraForm::disp_message (HTuple hv_WindowHandle, HTuple hv_String, HTuple hv_CoordSystem,
                               HTuple hv_Row, HTuple hv_Column, HTuple hv_Color, HTuple hv_Box)
{
    HTuple  hv_GenParamName, hv_GenParamValue;


    if (0 != (HTuple(hv_Row==HTuple()).TupleOr(hv_Column==HTuple())))
    {
        return;
    }
    if (0 != (hv_Row==-1))
    {
        hv_Row = 12;
    }
    if (0 != (hv_Column==-1))
    {
        hv_Column = 12;
    }
    //
    //Convert the parameter Box to generic parameters.
    hv_GenParamName = HTuple();
    hv_GenParamValue = HTuple();
    if (0 != ((hv_Box.TupleLength())>0))
    {
        if (0 != (HTuple(hv_Box[0])==HTuple("false")))
        {
            //Display no box
            hv_GenParamName = hv_GenParamName.TupleConcat("box");
            hv_GenParamValue = hv_GenParamValue.TupleConcat("false");
        }
        else if (0 != (HTuple(hv_Box[0])!=HTuple("true")))
        {
            //Set a color other than the default.
            hv_GenParamName = hv_GenParamName.TupleConcat("box_color");
            hv_GenParamValue = hv_GenParamValue.TupleConcat(HTuple(hv_Box[0]));
        }
    }
    if (0 != ((hv_Box.TupleLength())>1))
    {
        if (0 != (HTuple(hv_Box[1])==HTuple("false")))
        {
            //Display no shadow.
            hv_GenParamName = hv_GenParamName.TupleConcat("shadow");
            hv_GenParamValue = hv_GenParamValue.TupleConcat("false");
        }
        else if (0 != (HTuple(hv_Box[1])!=HTuple("true")))
        {
            //Set a shadow color other than the default.
            hv_GenParamName = hv_GenParamName.TupleConcat("shadow_color");
            hv_GenParamValue = hv_GenParamValue.TupleConcat(HTuple(hv_Box[1]));
        }
    }
    //Restore default CoordSystem behavior.
    if (0 != (hv_CoordSystem!=HTuple("window")))
    {
        hv_CoordSystem = "image";
    }
    //
    if (0 != (hv_Color==HTuple("")))
    {
        //disp_text does not accept an empty string for Color.
        hv_Color = HTuple();
    }
    //
    DispText(hv_WindowHandle, hv_String, hv_CoordSystem, hv_Row, hv_Column, hv_Color,
             hv_GenParamName, hv_GenParamValue);
    return;
}
//正反检测模板二
void CameraForm::DirectionDetect (HObject ho_Image, HTuple hv_ShapeModelID, HTuple hv_ShapeModelID1, HTuple hv_MatchROI,
                                  HTuple *hv_DetectResult)
{
    HObject  ho_ROI_2, ho_ImageReduced1, ho_Region1;
    HObject  ho_MatchROIRe1, ho_matchImage1;
    // Local control variables
    HTuple  hv_Area, hv_Row, hv_Column, hv_Row1, hv_Column1;
    HTuple  hv_Angle1, hv_Score,hv_Score1;
    GenRectangle1(&ho_ROI_2,1240, 740, 2150, 1200);
    ReduceDomain(ho_Image, ho_ROI_2, &ho_ImageReduced1);
    Threshold(ho_ImageReduced1, &ho_Region1, 128, 255);
    AreaCenter(ho_Region1, &hv_Area, &hv_Row, &hv_Column);
    if (HDevWindowStack::IsOpen())
        SetColor(HDevWindowStack::GetActive(),"red");
    //需要根据实际情况调整
    if (0 != (hv_Area>330000))
    {
        GenRectangle1(&ho_MatchROIRe1, HTuple(hv_MatchROI[0]), HTuple(hv_MatchROI[1]),
                HTuple(hv_MatchROI[2]), HTuple(hv_MatchROI[3]));
        ReduceDomain(ho_Image, ho_MatchROIRe1, &ho_matchImage1);
        FindNccModel(ho_matchImage1, hv_ShapeModelID, -(HTuple(30).TupleRad()), HTuple(30).TupleRad(),
                     0.8, 1, 0.5, "true", 4, &hv_Row1, &hv_Column1, &hv_Angle1, &hv_Score);
        FindNccModel(ho_matchImage1, hv_ShapeModelID1, -(HTuple(30).TupleRad()), HTuple(30).TupleRad(),
                     0.8, 1, 0.5, "true", 4, &hv_Row1, &hv_Column1, &hv_Angle1, &hv_Score1);
        if (0 != ((hv_Score.TupleLength())>0) && ((hv_Score.TupleLength())==0))
        {
            (*hv_DetectResult) = 1;
            appendMessage(u8"底盘方向为正");
        }
        else
        {
            (*hv_DetectResult) = 2;
            appendMessage(u8"底盘方向为反");
        }
    }
    else
    {
        (*hv_DetectResult) = 0;
        appendMessage(u8"缺少泡沫底盘");
    }
    return;
}

//正反检测按钮
void CameraForm::on_pushButton_Dir_clicked()
{
    HTuple hv_MatchROI2D2, hv_DetectResult;
    hv_MatchROI2D2.Clear();
    hv_MatchROI2D2[0]= 770;
    hv_MatchROI2D2[1]= 740;
    hv_MatchROI2D2[2]= 1820;
    hv_MatchROI2D2[3]= 1720;
    DirectionDetect(Image2D2, hv_NCCModel2D11, hv_NCCModel2D12,hv_MatchROI2D2, &hv_DetectResult);
    //*检测结果DetectResult:
    //0:报错,没有底托泡沫
    //1:ok,方向正确
    //2:NG,方向为反
}

void CameraForm::on_new_from_clicked()
{
    myfm->show();
}
void CameraForm::setWt(QWidget *wt)
{
    myfm=(MyTemplate*)wt;
}

void CameraForm::on_new_from_2_clicked()
{
	float dx, dy, da;
	guideRobotAdjustPlace2D(m_AdjustTmplImgPos, m_AdjustSceneImgPos, m_AdjustCalib, m_RegAdjustPos,
		m_PlaceTmplImgPos, m_PlaceSceneImgPos, m_PlaceCalib, m_RegPlacePos, dx, dy, da);
	QString Adjust2DResult = QString("%1，%2，%3").arg(dx).arg(dy).arg(da);
	appendMessage(Adjust2DResult);
}

