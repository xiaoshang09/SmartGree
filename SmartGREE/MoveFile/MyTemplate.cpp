#include "MyTemplate.h"
#include "ui_MyTemplate.h"
#include "cameraform.h"
#include <QDebug>
//窗口
HTuple  WindowHandle4;
HTuple  WindowHandle5;
//图片
HObject  form_hv_HomMat2D1,form_hv_HomMat2D2,ho_ImageReduced2,ho_ROI_1;
//模板图片
HObject ho_ContoursProjTrans2D1,ho_ModelContours1,ho_ModelContours2;
HObject  ho_ImageReduced,ho_ImageReduced3,ho_ImageReduced1;
HTuple  hv_RowTrans, hv_ColTrans,hv_NCCModelID1,hv_NCCModelID2;
// Local iconic variables
 HObject  ho_ROI_0;
 HObject  ho_Rectangle;

 // Local control variables
HTuple  hv_ModelID1;
HTuple  hv_HomMat2D;
HTuple  hv_cameraMatrix;
HTuple  hv_Qx1, hv_Qy1;
HTuple  hv_NCCModelID;
HTuple  hv_ModelID2;
HTuple  hv_Qx2, hv_Qy2;
HTuple hv_Pose;
HTuple hv_Angle;
//路径
QString path = "./Model/";
QString ModelName;

MyTemplate::MyTemplate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyTemplate)
{
    ui->setupUi(this);
    form_Initialization_Interface();
//    c=(CameraForm*)c;

}

MyTemplate::~MyTemplate()
{
    delete ui;
}
//初始化窗口
void MyTemplate::form_Initialization_Interface(){
    //QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    Hlong windID_4 = (Hlong)this->ui->gvMain_4->winId();//3、将控件和halcon窗体绑定起来，设定父窗体
    Hlong windID_5 = (Hlong)this->ui->gvMain_5->winId();//3、将控件和halcon窗体绑定起来，设定父窗体
    int width = ui->gvMain_4->width();
    int height = ui->gvMain_4->height();
    OpenWindow(0, 0, width, height, windID_4, "visible", "", &WindowHandle4);//windID作为父窗口  ，与halcon WindowHandle绑定
    SetWindowAttr("background_color", "black");
    SetDraw(WindowHandle4, "margin");
    SetLineWidth(WindowHandle4, 1);
    SetColor(WindowHandle4, "red");

    OpenWindow(0, 0, width, height, windID_5, "visible", "", &WindowHandle5);//windID作为父窗口  ，与halcon WindowHandle2绑定
    SetWindowAttr("background_color", "black");
    SetDraw(WindowHandle5, "margin");
    SetLineWidth(WindowHandle5, 1);
    SetColor(WindowHandle5, "red");
}
void MyTemplate::setWt(QWidget *wt)
{
    this->wt= (CameraForm*)wt;
}
//拍照检测
void MyTemplate::form_checkProduct(int i){
    switch(i){
        case 4:{
            QString Str_path;
            if (wt->open1)
            {
                HTuple  hv_Exception;
                //采集图片
                bool isOK = wt->xiangjicaiji(wt->objDevicePtr1, WindowHandle4, &form_hv_HomMat2D1);

                Str_path = wt->basePath+"Model_img/2D1/";
                QString picture = wt->xiangjibaocun(form_hv_HomMat2D1, Str_path);
                 appendMessage2(u8"图片采集成功");
            }else{
                 appendMessage2(u8"相机未打开");
            }
            break;
        }
        case 5:{
            QString Str_path;
            if (wt->open2){
               //采集图片
                bool isOK = wt->xiangjicaiji(wt->objDevicePtr2, WindowHandle5, &form_hv_HomMat2D2);

                Str_path = wt->basePath+"Model_img/2D2/";
                QString picture = wt->xiangjibaocun(form_hv_HomMat2D2, Str_path);
                 appendMessage2(u8"图片采集成功");
            }else{
                 appendMessage2(u8"相机未打开");
            }
           break;
        }
    }
}
//保存物料按钮
int MyTemplate::on_form_Store_material_clicked()
{
    QString pathf = path;
    QString conding1 = ui->conding1->text();
    QString conding2 = ui->conding2->text();
    ModelName = conding1+conding2;
    QFile mFile(pathf+"/Model/"+ModelName);
    if (conding1=="" | conding2==""){
         appendMessage2(u8"编码为空");
        return 0;
    }
    if(!mFile.exists())
    {
       new_file(pathf);
      ui->label_4->setText("yes");
    }else{
        if(QMessageBox::Yes == QMessageBox::question(this, u8"提示", u8"模板已存在.确定更新模板吗?")){
            QString aaafile = path+ModelName;
            DelDir(aaafile);
            new_file(pathf);
             QPalette::ColorRole green;
              ui->label_4->setText("yes");
        }
    }
    return 1;

}


//2d1采图按钮
void MyTemplate::on_form_pushButton_2D1_clicked()
{
    //form_checkProduct(4);
    HTuple  hv_Width, hv_Height;
    //主体函数   获取地址
    QString path = QFileDialog::getOpenFileName(this, tr("选择图像"), "", tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    if (path == "") return;
    //定义变量
    HTuple  FileNameModel;
    //qt  QString  转化成 halcon  tuple文件
    //FileNameModel      =  HTuple(path.toLatin1().data());
    FileNameModel = HTuple(code->fromUnicode(path).data());
    ReadImage(&form_hv_HomMat2D1, FileNameModel);//只有读取图像，里面才有数据 ，halconimage
    // ReadImage(&ho_Image, "E:/LKF/KuangLing_2D/1.bmp");
    GetImageSize(form_hv_HomMat2D1, &hv_Width, &hv_Height);
    SetPart(WindowHandle4, 0, 0, hv_Height, hv_Width); //设置窗口的大小
    DispObj(form_hv_HomMat2D1, WindowHandle4);
    appendMessage2("Image loaded successfully!");
}
//2D1抓取
void MyTemplate::on_form_Capture_clicked()
{
    QString ArmX_2D1 = ui->ArmX_2D1->toPlainText();
    QString ArmY_2D1 = ui->ArmY_2D1->toPlainText();
    QString ArmZ_2D1 = ui->ArmZ_2D1->toPlainText();
    QString RZ_2D1 = ui->RZ_2D1->toPlainText();
    QFile file(QString(path)+"/" +ModelName+ "/"+ModelName+".lua");
   qDebug()<<QString(path);
   qDebug()<<ModelName;
   file.open(QIODevice::Append | QIODevice::Text);
   QTextStream out(&file);//写入
   out<<"ArmX_2D1 "<<ArmX_2D1<<"\n";
   out<<"ArmY_2D1 "<<ArmY_2D1<<"\n";
   out<<"ArmZ_2D1 "<<ArmZ_2D1<<"\n";
   out<<"RZ_2D1 "<<RZ_2D1<<"\n";
   file.close();
}
//2d1获取ROI
void MyTemplate::on_form_ROI_2D1_clicked()
{
    // Local iconic variables
    HObject  ho_ROI_1, ho_TMP_Region;
    // Local control variables
   SetWindowAttr("background_color","black");
   DrawRegion(&ho_ROI_1, WindowHandle4);
   DrawRegion(&ho_TMP_Region, WindowHandle4);
   Union2(ho_ROI_1, ho_TMP_Region, &ho_ROI_1);
   ReduceDomain(form_hv_HomMat2D1, ho_ROI_1, &ho_ImageReduced);
}
//2d1显示
void MyTemplate::on_form_show_2D1_clicked()
{
    ClearWindow(WindowHandle4);
    DispObj(ho_ImageReduced, WindowHandle4);
}
//2d1重置
void MyTemplate::on_form_reset_2D1_clicked()
{
    HTuple  hv_Width, hv_Height ;
    GetImageSize(form_hv_HomMat2D1, &hv_Width, &hv_Height);
    SetPart(WindowHandle4, 0, 0, hv_Height, hv_Width); //设置窗口的大小
    DispObj(form_hv_HomMat2D1, WindowHandle4);
}
//2d1保存模板
void MyTemplate::on_form_save_2D1_clicked()
{
    HTuple   hv_ModelID,hv_Row,hv_Column,hv_Angle,hv_Score;
    CreateShapeModel(ho_ImageReduced, "auto", 0, HTuple(360).TupleRad(), "auto", "auto",
        "use_polarity", "auto", "auto", &hv_ModelID);
    FindShapeModel(form_hv_HomMat2D1, hv_ModelID, 0, HTuple(360).TupleRad(), 0.5, 1, 0.5,
        "least_squares", 0, 0.9, &hv_Row, &hv_Column, &hv_Angle, &hv_Score);
    dev_display_shape_matching_results(hv_ModelID, "red", hv_Row, hv_Column, hv_Angle,
          1, 1, 0);
     QFile file(QString(path)+"/" +ModelName+ "/"+ModelName+".lua");
     QString str_path = path + "/" +ModelName+ "/" + "2D1_shapeModle.sfm";
     HTuple str = str_path.toStdString().c_str();
    //模板1中心在空间坐标系1中的坐标Qx1，Qy1
    qDebug()<<QString(path);
    qDebug()<<ModelName;
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);//写入
    out<<"Qx1 "<< hv_Column.D()<<"\n";
    out<<"Qy1 "<< hv_Row.D()<<"\n"; 
    out<<"Rz2D1 "<<hv_Angle.D()<<"\n";
    WriteShapeModel(hv_ModelID, str);
    file.close();
    appendMessage2("2D1已保存");
     ui->label_5->setText("yes");
}


//2d2采图按钮
void MyTemplate::on_form_pushButton_2D1_2_clicked()
{
    //form_checkProduct(5);
    //form_checkProduct(4);
     HTuple  hv_Width, hv_Height;
     //主体函数   获取地址
     QString path = QFileDialog::getOpenFileName(this, tr("选择图像"), "", tr("Images (*.png *.bmp *.jpg *.tif *.GIF )"));
     QTextCodec *code = QTextCodec::codecForName("GB2312");
     if (path == "") return;
     //定义变量
     HTuple  FileNameModel;
     //qt  QString  转化成 halcon  tuple文件
     //FileNameModel      =  HTuple(path.toLatin1().data());
     FileNameModel = HTuple(code->fromUnicode(path).data());
     ReadImage(&form_hv_HomMat2D2, FileNameModel);//只有读取图像，里面才有数据 ，halconimage
     // ReadImage(&ho_Image, "E:/LKF/KuangLing_2D/1.bmp");
     GetImageSize(form_hv_HomMat2D2, &hv_Width, &hv_Height);
     SetPart(WindowHandle5, 0, 0, hv_Height, hv_Width); //设置窗口的大小
     DispObj(form_hv_HomMat2D2, WindowHandle5);
     appendMessage2("Image loaded successfully!");
}
//2d2获取ROI
void MyTemplate::on_from_ROI_2D2_clicked()
{
    HObject  ho_ROI_11, ho_TMP_Region1;
    // Local control variables
   SetWindowAttr("background_color","black");
   DrawRegion(&ho_ROI_11, WindowHandle5);
   DrawRegion(&ho_TMP_Region1, WindowHandle5);
   Union2(ho_ROI_11, ho_TMP_Region1, &ho_ROI_11);
   ReduceDomain(form_hv_HomMat2D2, ho_ROI_11, &ho_ImageReduced3);
}
//2d2显示
void MyTemplate::on_form_show_2D2_clicked()
{
    ClearWindow(WindowHandle5);
    DispObj(ho_ImageReduced3, WindowHandle5);
}
//2d2重置
void MyTemplate::on_form_reset_2D2_clicked()
{
    HTuple  hv_Width, hv_Height;
    GetImageSize(form_hv_HomMat2D2, &hv_Width, &hv_Height);
    SetPart(WindowHandle5, 0, 0, hv_Height, hv_Width); //设置窗口的大小
    DispObj(form_hv_HomMat2D2, WindowHandle5);
}
//2D2保存模板
void MyTemplate::on_form_save_2D2_clicked()
{
    //模板2中心在空间坐标系2中的坐标Qx2,Qy2
    HTuple   hv_ModelID,hv_Row,hv_Column,hv_Angle,hv_Score;
    CreateShapeModel(ho_ImageReduced3, "auto", 0, HTuple(360).TupleRad(), "auto", "auto",
        "use_polarity", "auto", "auto", &hv_ModelID);
    FindShapeModel(form_hv_HomMat2D2, hv_ModelID, 0, HTuple(360).TupleRad(), 0.5, 1, 0.5,
        "least_squares", 0, 0.9, &hv_Row, &hv_Column, &hv_Angle, &hv_Score);
    dev_display_shape_matching_results(hv_ModelID, "red", hv_Row, hv_Column, hv_Angle,
          1, 1, 0);
     QFile file(QString(path)+"/" +ModelName+ "/"+ModelName+".lua");
     QString str_path = path + "/" +ModelName+ "/" + "2D2_shapeModle.sfm";
     HTuple str = str_path.toStdString().c_str();
    //模板1中心在空间坐标系1中的坐标Qx1，Qy1
     QString ArmX_2D2 = ui->ArmX_2D2->toPlainText();
     QString ArmY_2D2 = ui->ArmY_2D2->toPlainText();
     QString ArmZ_2D2 = ui->ArmZ_2D2->toPlainText();
     QString RZ_2D2 = ui->RZ_2D2->toPlainText();
    file.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&file);//写入
	out << "Qx2 " << hv_Column.D() << "\n";
	out << "Qy2 " << hv_Row.D() << "\n";
    out<<"Rz2D2 "<<hv_Angle.D()<<"\n";
    out<<"ArmX_2D2 "<<ArmX_2D2<<"\n";
    out<<"ArmY_2D2 "<<ArmY_2D2<<"\n";
    out<<"ArmZ_2D2 "<<ArmZ_2D2<<"\n";
    out<<"RZ_2D2 "<<RZ_2D2<<"\n";
    WriteShapeModel(hv_ModelID, str);
    file.close();
    ui->label_22->setText("yes");
    appendMessage2("2D2保存成功");
}

//保存模板
void MyTemplate::on_form_new_save_clicked()
{
    // Local control variables
    HTuple  hv_robotCalib2D1, hv_robotCalib2D2;
    HTuple  hv_robotTeach2D1, hv_robotTeach2D2, hv_Tx0, hv_Ty0;
    HTuple  hv_Area;
    HTuple  hv_Row3, hv_Column;
    HTuple  hv_nccmodelcenter;
    HTuple hv_X, hv_Y;
    HTuple  hv_Theta, hv_HomMat2DIdentity, hv_Qx3, hv_Qy3, hv_QxTemp;
    HTuple  hv_QyTemp, hv_deltaX, hv_deltaY, hv_deltaTheta;
    QString  qstr = path+ModelName+"/";
    string str = qstr.toStdString();
    const char *Tempbasepath = str.data();
    HTuple modelbasepath(Tempbasepath);
    //标定时机械手Start2D1，Start2D2数据[X, Y, 0.0]
    hv_robotCalib2D1.Clear();
    hv_robotCalib2D1[0] = 1122.5;
    hv_robotCalib2D1[1] = 37.5;
    hv_robotCalib2D2.Clear();
    hv_robotCalib2D2[0] = 1967.1;
    hv_robotCalib2D2[1] = 12.36;
    //示教数据[X, Y, Theta]
    hv_robotTeach2D1.Clear();
    hv_robotTeach2D1[0] = 1122.46;
    hv_robotTeach2D1[1] = 37.56;
    hv_robotTeach2D1[2] = 0.00;
    hv_robotTeach2D2.Clear();
    hv_robotTeach2D2[0] = 2376.74;
    hv_robotTeach2D2[1] = -131.73;
    hv_robotTeach2D2[2] = 4.55;
    //空间坐标系1原点在空间坐标系2中坐标:RotateCenter2-RotateCenter1-机械手移动距离
    hv_Tx0 = (HTuple(wt->hv_RotateCenter2[0])-HTuple(wt->hv_RotateCenter1[0]))-(HTuple(hv_robotCalib2D2[0])-HTuple(hv_robotCalib2D1[0]));
    hv_Ty0 = (HTuple(wt->hv_RotateCenter2[1])-HTuple(wt->hv_RotateCenter1[1]))-(HTuple(hv_robotCalib2D2[1])-HTuple(hv_robotCalib2D1[1]));

    AreaCenter(ho_ROI_0, &hv_Area, &hv_Row3, &hv_Column);

    hv_nccmodelcenter.Clear();
    hv_nccmodelcenter[0] = 10;
    hv_nccmodelcenter[1] = 10;
//    hv_nccmodelcenter.Append(hv_Row3);
//    hv_nccmodelcenter.Append(hv_Column);
    //*******************偏置运算
    hv_X = HTuple(hv_robotTeach2D2[0])-HTuple(hv_robotTeach2D1[0]);
    hv_Y = HTuple(hv_robotTeach2D2[1])-HTuple(hv_robotTeach2D1[1]);
    hv_Theta = HTuple(hv_robotTeach2D2[2])-HTuple(hv_robotTeach2D1[2]);
    HomMat2dIdentity(&hv_HomMat2DIdentity);
    HomMat2dRotate(hv_HomMat2DIdentity, hv_Theta.TupleRad(), HTuple(wt->hv_RotateCenter1[0]),
        HTuple(wt->hv_RotateCenter1[1]), &hv_HomMat2D);
    //将模板1中心以法兰盘中心为旋转中心旋转Theta（两个机械臂示教点角度差值），计算得出旋转后的坐标Qx3，Qy3
    AffineTransPoint2d(hv_HomMat2D, hv_Qx1, hv_Qy1, &hv_Qx3, &hv_Qy3);

    //（ Qx3 + Tx0）：模板1中心在坐标系2中的坐标
    //（Qx3 + Tx0 + X）：模板1中心平移到装配位时的坐标
    //QxTemp=（Qx3 + Tx0 + X - Qx2）：平移后模板1中心到模板2中心的偏移
    hv_QxTemp = ((hv_Qx3+hv_Tx0)+hv_X)-hv_Qx2;
    hv_QyTemp = ((hv_Qy3+hv_Ty0)+hv_Y)-hv_Qy2;
    HomMat2dRotate(hv_HomMat2DIdentity, -hv_Angle, 0, 0, &hv_HomMat2D);
    AffineTransPoint2d(hv_HomMat2D, hv_QxTemp, hv_QyTemp, &hv_deltaX, &hv_deltaY);
    hv_deltaTheta = ((hv_Angle.TupleDeg())+HTuple(hv_Pose[5]))-hv_Theta;

    WriteDeformableModel(hv_ModelID1, modelbasepath+"2D1_deformable_model.dfm");
    WriteNccModel(hv_NCCModelID, modelbasepath+"2D2_ncc_model.ncm");
    WriteShapeModel(hv_ModelID2, modelbasepath+"2D2_shape_model.shm");
    WriteTuple((((hv_deltaTheta.TupleConcat(hv_Tx0)).TupleConcat(hv_Ty0)).TupleConcat(hv_deltaX)).TupleConcat(hv_deltaY),
        modelbasepath+"offset.dat");
    WriteTuple(hv_cameraMatrix, modelbasepath+"cameraMatrix.dat");
    WriteTuple(hv_robotTeach2D1, modelbasepath+"robotTeach2D1.dat");
    WriteTuple(hv_nccmodelcenter, modelbasepath+"nccmodelcenter.dat");
    WriteRegion(ho_Rectangle, modelbasepath+"2D2_SHMROI.hobj");
    appendMessage2("新模板保存成功");
}

//重置
void MyTemplate::on_form_reset_clicked()
{
    QString aaafile = path+ModelName;
    DelDir(aaafile);
    appendMessage2("重置成功");
}
//递归删除文件
bool MyTemplate::DelDir(const QString &afile)
{
    if (afile.isEmpty())
    {
        return false;
    }
    QDir dir(afile);
    if (!dir.exists())
    {
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach(QFileInfo file, fileList)
    { //遍历文件信息
        if (file.isFile()) { // 是文件，删除
            file.dir().remove(file.fileName());
        }
        else
        { // 递归删除
            DelDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath()); // 删除文件夹
}

//创建文件
void MyTemplate::new_file(QString str){
    //创建文件
    QFileInfo info(ModelName);
    QDir dir;
    dir.cd(str);
    dir.mkdir(info.baseName());
    str+=info.baseName();

    //"yyyy-MM-dd-hh-mm-ss"
    QFile file(QString(str)+"/" +ModelName+ ".lua");
    file.open(QIODevice::Append | QIODevice::Text);//append追加，不会覆盖之前的文件
    QTextStream out(&file);//写入
    out<<"ProdatID "+ModelName<<"\n";
     appendMessage2(u8"文件已保存");
    file.close();
}

void MyTemplate::dev_display_ncc_matching_results (HTuple hv_ModelID, HTuple hv_Color, HTuple hv_Row,
    HTuple hv_Column, HTuple hv_Angle, HTuple hv_Model)
{

  // Local iconic variables
  HObject  ho_ModelRegion,ho_ContoursAffinTrans;
  HObject  ho_Cross;

  // Local control variables
  HTuple  hv_NumMatches, hv_Index, hv_Match, hv_HomMat2DIdentity;
  HTuple  hv_HomMat2DRotate, hv_HomMat2DTranslate, hv_RowTrans;
  HTuple  hv_ColTrans;

  //This procedure displays the results of Correlation-Based Matching.
  //
  hv_NumMatches = hv_Row.TupleLength();
  if (0 != (hv_NumMatches>0))
  {
    if (0 != ((hv_Model.TupleLength())==0))
    {
      TupleGenConst(hv_NumMatches, 0, &hv_Model);
    }
    else if (0 != ((hv_Model.TupleLength())==1))
    {
      TupleGenConst(hv_NumMatches, hv_Model, &hv_Model);
    }
    {
    HTuple end_val9 = (hv_ModelID.TupleLength())-1;
    HTuple step_val9 = 1;
    for (hv_Index=0; hv_Index.Continue(end_val9, step_val9); hv_Index += step_val9)
    {
      GetNccModelRegion(&ho_ModelRegion, HTuple(hv_ModelID[hv_Index]));
      GenContourRegionXld(ho_ModelRegion, &ho_ModelContours1, "border_holes");
      if (HDevWindowStack::IsOpen())
        SetColor(HDevWindowStack::GetActive(),HTuple(hv_Color[hv_Index%(hv_Color.TupleLength())]));
      {
      HTuple end_val13 = hv_NumMatches-1;
      HTuple step_val13 = 1;
      for (hv_Match=0; hv_Match.Continue(end_val13, step_val13); hv_Match += step_val13)
      {
        if (0 != (hv_Index==HTuple(hv_Model[hv_Match])))
        {
          HomMat2dIdentity(&hv_HomMat2DIdentity);
          HomMat2dRotate(hv_HomMat2DIdentity, HTuple(hv_Angle[hv_Match]), 0, 0, &hv_HomMat2DRotate);
          HomMat2dTranslate(hv_HomMat2DRotate, HTuple(hv_Row[hv_Match]), HTuple(hv_Column[hv_Match]),
              &hv_HomMat2DTranslate);
          AffineTransContourXld(ho_ModelContours1, &ho_ContoursAffinTrans, hv_HomMat2DTranslate);
          if (HDevWindowStack::IsOpen())
            DispObj(ho_ContoursAffinTrans, HDevWindowStack::GetActive());
          AffineTransPixel(hv_HomMat2DTranslate, 0, 0, &hv_RowTrans, &hv_ColTrans);
          GenCrossContourXld(&ho_Cross, hv_RowTrans, hv_ColTrans, 6, HTuple(hv_Angle[hv_Match]));
          if (HDevWindowStack::IsOpen())
            DispObj(ho_Cross, HDevWindowStack::GetActive());
        }
      }
      }
    }
    }
  }
  return;
}

void MyTemplate::dev_display_shape_matching_results (HTuple hv_ModelID, HTuple hv_Color, HTuple hv_Row,
    HTuple hv_Column, HTuple hv_Angle, HTuple hv_ScaleR, HTuple hv_ScaleC, HTuple hv_Model)
{

  // Local iconic variables
  HObject  ho_ContoursAffinTrans;

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
      GetShapeModelContours(&ho_ModelContours2, HTuple(hv_ModelID[hv_Index]), 1);
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
          AffineTransContourXld(ho_ModelContours2, &ho_ContoursAffinTrans, hv_HomMat2DTranslate);
          if (HDevWindowStack::IsOpen())
            DispObj(ho_ContoursAffinTrans, HDevWindowStack::GetActive());
        }
      }
      }
    }
    }
  }
  return;
}

void MyTemplate::appendMessage2(const QString &msg){
    QString str=QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") + " " +msg;
    ui->form_textBrowser->append(str);
}
void MyTemplate::on_form_ROI_2D1_2_clicked()
{
   HObject  ho_ROI_1, ho_TMP_Region;
   SetWindowAttr("background_color","black");
   DrawRegion(&ho_ROI_1, WindowHandle4);
   DrawRegion(&ho_TMP_Region, WindowHandle4);
   ReduceDomain(form_hv_HomMat2D1, ho_TMP_Region, &ho_ImageReduced1);
   ReduceDomain(form_hv_HomMat2D1, ho_ROI_1, &ho_ImageReduced2);
}

void MyTemplate::on_form_show_2D1_2_clicked()
{
    ClearWindow(WindowHandle4);
    DispObj(ho_ImageReduced1, WindowHandle4);
    DispObj(ho_ImageReduced2, WindowHandle4);
}

void MyTemplate::on_form_save_2D1_2_clicked()
{
    CreateNccModel(ho_ImageReduced1, "auto", -(HTuple(30).TupleRad()), HTuple(30).TupleRad(),
        "auto", "use_polarity", &hv_NCCModelID1);
     CreateNccModel(ho_ImageReduced2, "auto", -(HTuple(30).TupleRad()), HTuple(30).TupleRad(),
         "auto", "use_polarity", &hv_NCCModelID2);
     QString str_path1 = path + "/" +ModelName+ "/" + "2D1_NCCModle1.ncm";
     QString str_path = path + "/" +ModelName+ "/" + "2D1_NCCModle2.ncm";
     HTuple str = str_path.toStdString().c_str();
     HTuple str1 = str_path1.toStdString().c_str();
      WriteNccModel(hv_NCCModelID1, str);
      WriteNccModel(hv_NCCModelID2, str1);
      appendMessage2("NCC模板已保存");
      ui->label_10->setText("yes");
}

