#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helper.h"
#include "SettingDatas.h"
#include "ProtocolHandller.h"
#include "CameraWidget.h"

#include <QDebug>
#include <QDateTime>
#include <QThread>
#include <QFile>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint );

    initWin();

//    startTimer(500);

    connect(SettingDatas::instance(), SIGNAL(loadFinished() ), this, SLOT(onLoad()));
    connect(SettingDatas::instance(), SIGNAL(valueChanged(const char*) ), this, SLOT(onValueChanged(const char*)));
//    QTimer::singleShot(5000,this,SLOT(onStartUp()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWin()
{
    ui->widget_title->setWin(this);
    ui->widget_title->setLogo( Helper::GetPixmapSvg( ":image/gree.svg", QSize(240,110) ) );
    ui->widget_title->setTitle( u8"底盘自动上线智能系统" );
    ui->widget_title->setMessage(u8"设备未连接!");
    connect(ui->widget_title, SIGNAL(clicked(int)), this, SLOT(titleClicked(int)) );

    m_productFm=new ProductBaseForm();
    m_logFm = new LogWidget();
    m_settingsFm = new SettingBaseWidget();
    aboutWt = new AboutWidget();
    matenWt = new MaintenanceWidget();
    matenWt->setMsg(u8"温馨提示:设备已到预设的保养时间");
    matenWt->setMode(MaintenanceWidget::E_MATEN);

    repWt = new MaintenanceWidget();//更换
    repWt->setMode(MaintenanceWidget::E_REP);
    repWt->setMsg(u8"温馨提示:设备已到预设的更换时间");

    ui->stackedWidget->addWidget(m_productFm);
    ui->stackedWidget->addWidget(m_logFm);
    ui->stackedWidget->addWidget(m_settingsFm);
    ui->stackedWidget->addWidget(aboutWt);

    //
    showMain();
}

void MainWindow::showEvent(QShowEvent *event)
{
    qDebug()<<"test=====912======";
//    showMain();
    qDebug()<<"test=====9121======";
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(this->isVisible()){
        QMainWindow::timerEvent(event);

        QString curtime=QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm ddd");
        ui->label_time->setText( curtime+" " );
    }
}

void MainWindow::showMain()
{
    if( m_productFm!=ui->stackedWidget->currentWidget() ){
        ui->stackedWidget->setCurrentWidget( m_productFm );
    }
}

void MainWindow::onLoad()
{
     QString matenT= GetVal(DK_MaintenanceTimeLast).toString();
     ui->toolButton_maten->setText(u8"上次设备保养时间:"+ matenT);

      QString repT = GetVal(DK_ReplacementTimeLast).toString();
      ui->toolButton_eq_replace->setText(u8"上次设备更换时间:"+repT);
}

void MainWindow::onValueChanged(const char* key)
{
    if(DK_COMP(key, DK_MaintenanceTimeLast)){
        QString matenT= GetVal(DK_MaintenanceTimeLast).toString();
        ui->toolButton_maten->setText(u8"上次设备保养时间:"+ matenT);
    }

    if(DK_COMP(key, DK_ReplacementTimeLast)){
        QString repT = GetVal(DK_ReplacementTimeLast).toString();
        ui->toolButton_eq_replace->setText(u8"上次设备更换时间:"+repT);
    }

    if(DK_COMP(key, DK_PlcState)){
        bool plcOK = GetVal( DK_PlcState ).toBool();
        if(plcOK){
            ui->widget_title->setState(TitleForm::E_OK,u8"设备正常");
            ui->widget_title->setMessage("");
        }else{
            ui->widget_title->setState(TitleForm::E_WARN,u8"PLC未连接");
            ui->widget_title->setMessage(u8"设备未连接!");
        }
    }

    if(DK_COMP(key, DK_StateSTS)){
        InstructionCode::U_PLCStatus sts;
        sts.state = GetVal( DK_StateSTS ).toUInt();

        qDebug()<<"解析PLC状态 chaged=="<< sts.state;


//        int MachineRunning      : 1;  //机台运行中
//        int         : 1;  //机台报警中
//        int       : 1;  //机台待机中

//        int         : 1;  //机台缺料中
//        int       : 1;  //气源供气不足报警
//        int         : 1;  //机器人报警中
//        int  : 1;  //安全光栅报警
//        int    : 1;  //夹具伺服报警
//        int      : 1;  //物料掉落报警

        QString msg;
        if(sts.STS.MachineRunning){
            msg += u8"/机台运行中/";
        }
        if(sts.STS.MachineAlarm){
            msg += u8"/机台报警中/";
        }
        if(sts.STS.MachineStandby){
            msg += u8"/机台待机中/";
        }
        if(sts.STS.MaterialLack){
            msg += u8"/机台缺料中/";
        }
        if(sts.STS.AirSupplyAlarm){
            msg += u8"/气源供气不足报警/";
        }
        if(sts.STS.RobotWarming){
            msg += u8"/机器人报警中/";
        }
        if(sts.STS.SafetyLigthCurtains){
            msg += u8"/安全光栅报警/";
        }
        if(sts.STS.FixtureServoAlarm){
            msg += u8"/夹具伺服报警/";
        }
        if(sts.STS.MeterialFalling){
            msg += u8"/物料掉落报警/";
        }
        ui->widget_title->setMessage(msg);

        int i=0;
        qDebug()<<"sts.STS=="<<i++<< sts.STS.MachineRunning;
        qDebug()<<"sts.STS=="<<i++<< sts.STS.MachineAlarm;
        qDebug()<<"sts.STS=="<<i++<< sts.STS.MachineStandby;
        qDebug()<<"sts.STS=="<<i++<< sts.STS.MaterialLack;
        qDebug()<<"sts.STS=="<<i++<< sts.STS.AirSupplyAlarm;
        qDebug()<<"sts.STS=="<<i++<< sts.STS.RobotWarming;
        qDebug()<<"sts.STS=="<<i++<< sts.STS.SafetyLigthCurtains;
        qDebug()<<"sts.STS=="<<i++<< sts.STS.FixtureServoAlarm;
        qDebug()<<"sts.STS=="<<i++<< sts.STS.MeterialFalling;

        qDebug()<<"sts.Reserve=="<<i++<< sts.STS.Reserve << msg;


        //运行状态
        if(sts.STS.MachineRunning){
            ui->widget_title->setState(TitleForm::E_OK,u8"机台正常");
        }else if(sts.STS.MachineStandby){
            ui->widget_title->setState(TitleForm::E_WARN,u8"待机中");
        }else if(sts.STS.MachineAlarm){
            ui->widget_title->setState(TitleForm::E_NG,u8"机台报警");
        }

        //机器人
        if(sts.STS.RobotWarming){
            ui->toolButton_st2->setIcon( QIcon(":/image/error.png") );
        }else{
            ui->toolButton_st2->setIcon( QIcon(":/image/ok.png") );
        }

        //夹具
        if(sts.STS.MaterialLack|sts.STS.FixtureServoAlarm|sts.STS.MeterialFalling){
            ui->toolButton_st3->setIcon( QIcon(":/image/error.png") );
        }else{
            ui->toolButton_st3->setIcon( QIcon(":/image/ok.png") );
        }

        //气源
        if(sts.STS.AirSupplyAlarm){
            ui->toolButton_st4->setIcon( QIcon(":/image/error.png") );
        }else{
            ui->toolButton_st4->setIcon( QIcon(":/image/ok.png") );
        }
    }

    //相机
#if 0
    if( CameraWidget::instance()->state() ){
#else
    if(DK_COMP(key, DK_StateCamera)){
#endif
        if(GetVal(DK_StateCamera).toBool()){
            ui->toolButton_st1->setIcon( QIcon(":/image/ok.png") );
        }else{
            ui->toolButton_st1->setIcon( QIcon(":/image/error.png") );
        }
    }

}

void MainWindow::titleClicked(int id)
{
    qDebug()<<__FUNCTION__<< id;

    switch(id){
    case TitleForm::E_LOGO:
    case TitleForm::E_TITLE:
        showMain();
        break;
    case TitleForm::E_MSG:
        break;
    case TitleForm::E_LOG:
        ui->stackedWidget->setCurrentWidget( m_logFm );
        break;
    case TitleForm::E_STATE:
        break;
    case TitleForm::E_SETTINGS:
        ui->stackedWidget->setCurrentWidget( m_settingsFm );
        break;
    case TitleForm::E_USR:
        LoginDialog::instance()->showMaximized();
        this->hide();
        break;
    case TitleForm::E_HELP:
        ui->stackedWidget->setCurrentWidget( aboutWt );
        break;
    case TitleForm::E_MIN:
        break;
    case TitleForm::E_MAX:
        break;
    case TitleForm::E_CLOSE:
        break;
    }
}

void MainWindow::onStartUp()
{
    matenWt->checkTime();
    repWt->checkTime();
}

void MainWindow::setStyle()
{
    QString styleStr;
    QFile ff;

    ff.setFileName(":gree.qss");

    if (ff.exists())
    {
        ff.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&ff);
        styleStr += (ts.readAll());
        ff.close();
    }else{
        qDebug()<<"qss not found !" << ff.fileName();
    }

    this->setStyleSheet( styleStr );
}

void MainWindow::on_toolButton_maten_clicked()
{
    Helper::MoveCenter(matenWt,true);
    matenWt->show();
}

void MainWindow::on_toolButton_eq_replace_clicked()
{
    Helper::MoveCenter(repWt,true);
    repWt->show();
}


