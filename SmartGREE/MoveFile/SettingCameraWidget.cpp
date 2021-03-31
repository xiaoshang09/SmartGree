#include "SettingCameraWidget.h"
#include "ui_SettingCameraWidget.h"
#include "SettingDatas.h"

SettingCameraWidget::SettingCameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingCameraWidget)
{
    ui->setupUi(this);

    connect(SettingDatas::instance(), SIGNAL(valueChanged(const char *) ), this, SLOT(onValueChanged(const char *) ) );
    connect(SettingDatas::instance(), SIGNAL(loadFinished() ), this, SLOT(onLoad()));
    connect(SettingDatas::instance(), SIGNAL(sigSetDefault() ), this, SLOT(setDefault()));
}

SettingCameraWidget::~SettingCameraWidget()
{
    delete ui;
}

void SettingCameraWidget::onValueChanged(const char *key)
{

}

void SettingCameraWidget::onLoad()
{

}

void SettingCameraWidget::setDefault()
{
    //camera
    SetVal( DK_CameraDir,    "./camera" ) ;
    SetVal( DK_CameraImgDir, "./camera/img" ) ;
    SetVal( DK_CameraImgType, "bmp" ) ;
}
