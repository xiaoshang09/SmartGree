
#include <QApplication>
#include "SplashScreen.h"
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QScreen>
#include <QTimer>

#include "logger.h"
#include "propertyconfigurator.h"
#include "loggerrepository.h"
#include "basicconfigurator.h"
#include "logmanager.h"
#include "logindialog.h"
#include "XDatabase.h"
#include "mainwindow.h"
#include "helper.h"
#include "ProtocolHandller.h"
#include "SettingDatas.h"
#include "DsGroup.h"
#include "CameraWidget.h"

static void initializeRootLogger();
static void shutdownRootLogger();
static void logStartup();
static void logShutdown();
static void setupRootLogger(const QString &introMessage);
static void shutDownRootLogger(const QString &extroMessage);
void setStyle();

void loadSqlDriver()
{
     QPluginLoader loader;
     loader.setFileName("D:/QT5.9/5.9.2/mingw53_32/plugins/sqldrivers/qsqlmysqld.dll");
     qDebug() << loader.load();
}

void creatDB(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",QTime::currentTime().toString("HH:mm:ss.zzz"));
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("");
    if (!db.isValid())
    {
        qDebug()<<__FUNCTION__<< db.lastError();
        return ;
    }
    if (!db.open())
    {
        qDebug()<<"Cannot open database!!!"<<db.lastError();
    }
    else
    {
        qDebug()<<" open database success!!!";
        QSqlQuery query = db.exec( QString("Create Database If  Not Exists %1 Character Set UTF8;").arg(D_DB_FILE_A) );

//        bool ok = query.exec();

        db.setDatabaseName(D_DB_FILE_A);
    }
}

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseOpenGLES);//webview奔溃BUG解决方案

    QApplication a(argc, argv);
    qDebug ()<<"sql==="<<QSqlDatabase::drivers();

    qDebug()<<"app path=="<<QCoreApplication::applicationFilePath();

    QCoreApplication::setOrganizationName("SmartEye");
    QCoreApplication::setOrganizationDomain("smarteyetec.com");
    QCoreApplication::setApplicationName("SmartGree");
    QCoreApplication::setApplicationVersion("1.0.0");

    //    App::setShowConsole(false);
    initializeRootLogger();
    logStartup();
    setStyle();
#ifdef D_USE_MYSQL
    creatDB();
#endif

    //
//    Helper::MoveCenter( SplashScreen::instance(), true );
    SplashScreen::instance()->resize(900,600);
    SplashScreen::instance()->move(540,220);
    SplashScreen::instance()->setText( u8"加载模块.日志...");
    SplashScreen::instance()->progressBarAdd(5);
    SplashScreen::instance()->show();
    //    SplashScreen::instance()->showMaximized();

    //
    SplashScreen::instance()->setText( u8"加载模块.登录...");

//    LoginDialog::instance()->resize(1300,850);
    SplashScreen::instance()->progressBarAdd(10);
    ///==========================================

    //
    SplashScreen::instance()->setText( u8"加载模块.相机..." );
    CameraWidget::instance();
//    CameraWidget::instance()->initCamera();
    SplashScreen::instance()->progressBarAdd(10);

    //
    SplashScreen::instance()->setText( u8"加载模块.产能统计..." );
    DsGroup::instance()->start();
    SplashScreen::instance()->progressBarAdd(10);

    //
    SplashScreen::instance()->setText( u8"加载模块.PLC监控..." );
    ProtocolHandller::instance();
    ProtocolHandller::instance()->start();
    SplashScreen::instance()->progressBarAdd(10);

    //
    SplashScreen::instance()->setText( u8"加载UI组件...");
    MainWindow w2;
    w2.resize(1300,850);
    w2.move(10,100);
    SplashScreen::instance()->progressBarAdd(10);

    //
    SplashScreen::instance()->setText( u8"加载配置..." );
    SettingDatas::instance()->load();//
    SplashScreen::instance()->progressBarAdd(10);

//    SplashScreen::instance()->setText( u8"加载模块.机器人监控..." );
//    SplashScreen::instance()->progressBarAdd(5);


    //在此区域添加代码
    //    maintest();

    //    QTimer timer;
    //    QTimer::connect(&timer, &QTimer::timeout,[=](){
    //        qDebug()<<"timer==========";
    //    } );
    //    timer.start(3000);


    ///=====================================

    SplashScreen::instance()->progressBarAdd(100);

    //w2.resize();
    static bool iscameraInit=false;



    QObject::connect(LoginDialog::instance(), &LoginDialog::sig_login_return , [=,&w2](bool ok){
        if(ok){
            w2.showMaximized();
            if(!iscameraInit){
                iscameraInit=true;
                CameraWidget::instance()->initCamera();
            }
        }else{
            LoginDialog::instance()->showMaximized();
        }
    });
#if 1
    w2.show();
    if(!iscameraInit){
        iscameraInit=true;
        CameraWidget::instance()->initCamera();
    }
#else
    SplashScreen::instance()->finish(LoginDialog::instance());
    LoginDialog::instance()->showMaximized();
#endif

    int ret = a.exec();

    logShutdown();
    shutdownRootLogger();

    //TODO close camera
//    CameraWidget::instance()->close()

    return ret;
}

void setStyle(){

    QString styleStr;
    QFile ff;

    //  =========================================
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

    //    =========================================
    ff.setFileName(":logindialog.qss");
    if (ff.exists())
    {
        ff.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&ff);
        styleStr += (ts.readAll());
        ff.close();
    }else{
        qDebug()<<"qss not found !" << ff.fileName();
    }

    //    =========================================
    ff.setFileName(":systembuttons.qss");
    if (ff.exists())
    {
        ff.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&ff);
        styleStr += (ts.readAll());
        ff.close();
    }else{
        qDebug()<<"qss not found !" << ff.fileName();
    }

    qApp->setStyleSheet( styleStr );

}


void initializeRootLogger()
{
    setupRootLogger(QStringLiteral("Root logger is setup."));
}

void shutdownRootLogger()
{
    shutDownRootLogger(QStringLiteral("Root logger was shutdown."));
}

void logStartup()
{
    auto logger = Log4Qt::Logger::rootLogger();

    logger->info(QStringLiteral("################################################################"));
    logger->info(QStringLiteral("#                          START                               #"));
    logger->info(QStringLiteral("################################################################"));
}

void logShutdown()
{
    auto logger = Log4Qt::Logger::rootLogger();

    logger->info(QStringLiteral("################################################################"));
    logger->info(QStringLiteral("#                          STOP                                #"));
    logger->info(QStringLiteral("################################################################"));
}

void setupRootLogger(const QString &introMessage)
{
    QString configFile = QStringLiteral("./log4qt.properties");
    if (QFile::exists(configFile))
        Log4Qt::PropertyConfigurator::configureAndWatch(configFile);
    if (!introMessage.isEmpty())
        Log4Qt::Logger::rootLogger()->info(introMessage);
    Log4Qt::LogManager::setHandleQtMessages(true);
}

void shutDownRootLogger(const QString &extroMessage)
{
    auto logger = Log4Qt::Logger::rootLogger();

    if (!extroMessage.isEmpty())
        logger->info(extroMessage);
    logger->removeAllAppenders();
    logger->loggerRepository()->shutdown();
}
