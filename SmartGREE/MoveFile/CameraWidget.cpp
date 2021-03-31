#include "CameraWidget.h"
#include "ui_CameraWidget.h"
#include <QInputDialog>
#include <QMessageBox>
//#include "SettingDatas.h"

#define D_USE_CAMERAFORM


CameraWidget::CameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraWidget)
{
    ui->setupUi(this);
}

CameraWidget::~CameraWidget()
{
    delete ui;
}

void CameraWidget::initCamera()
{
#ifdef D_USE_CAMERAFORM
    ui->camera->Initialization();
#else
#endif
}

void CameraWidget::setAppPath(const QString &path)
{
    qDebug()<<__FUNCTION__<< "setAppPath==="<< path;
#ifdef D_USE_CAMERAFORM
    ui->camera->setAppPath(path);
#else
#endif
}

int CameraWidget::setProductId(const QString &id)
{
#ifdef D_USE_CAMERAFORM
    return  ui->camera->setProductID(id);
#else
    return 0;
#endif
}

int CameraWidget::check2D1Dir(const QString &id, QString &picture)
{
    return  ui->camera->check2D1Dir(id, picture);
}

int CameraWidget::check2D2pos(const QString &id, QString &picture, float &x, float &y, float &z, float &Rx, float &Ry, float &Rz)
{
    return  ui->camera->check2D2pos(id, picture, x, y, z, Rx, Ry, Rz);
}

int CameraWidget::check2D1pos(const QString &id, QString &picture)
{
    return  ui->camera->check2D1pos(id, picture);
}


int CameraWidget::addTemplate(const QString &id, int type)
{
#ifdef D_USE_CAMERAFORM
    return  ui->camera->addTemplate(id,type);
#else
    return 0;
#endif
}

int CameraWidget::state()
{
#ifdef D_USE_CAMERAFORM
    return  ui->camera->state();
#else
    return 0;
#endif
}

void CameraWidget::showTemplates()
{
#ifdef D_USE_CAMERAFORM
    return  ui->camera->showTemplates();
#else
#endif
}

QWidget *CameraWidget::settingsWt()
{
//#ifdef D_USE_CAMERAFORM
//    return  ui->camera->settingsWt();
//#else
    return new QWidget;
//#endif
}

void CameraWidget::on_toolButton_add_clicked()
{
    static QString dlgTitle(u8"切换产品");//对话框标题
    static QString txtLabel(u8"请输产品ID：");//对话框Label显示内容
    static QString defaultInput(u8"");//对话框内输入框的默认内容
    static QLineEdit::EchoMode echoMode=QLineEdit::Normal;//输入框的文本内容为正常显示
    //QLineEdit::EchoMode echoMode=QLineEdit::Password;//输入框的文本内容为密码方式显示
    bool ok=false;

    QString text=QInputDialog::getText(this,dlgTitle,txtLabel,echoMode,defaultInput,&ok);
    if(ok && !text.isEmpty()){

        ui->lineEdit_id->setText(text);
        emit changeID(text);
        QMessageBox::information(this,u8"消息",u8"添加成功!" );

    }
}

void CameraWidget::on_pushButton_showTeps_clicked()
{
    showTemplates();
}

void CameraWidget::on_pushButton_addTep_clicked()
{

}
