#include "settingprojectwidget.h"
#include "ui_settingprojectwidget.h"
#include "SettingDatas.h"
#include <QDebug>


SettingProjectWidget::SettingProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingProjectWidget)
{
    ui->setupUi(this);

    connect(SettingDatas::instance(), SIGNAL(valueChanged(const char *) ), this, SLOT(onValueChanged(const char *) ) );
    connect(SettingDatas::instance(), SIGNAL(loadFinished() ), this, SLOT(onLoad()));
    connect(SettingDatas::instance(), SIGNAL(sigSetDefaultDir() ), this, SLOT(setDefault()));
}

SettingProjectWidget::~SettingProjectWidget()
{
    delete ui;
}

void SettingProjectWidget::onValueChanged(const char *key)
{
    if(DK_COMP(key,DK_ProjectDir)){
        onLoad();
    }
}

void SettingProjectWidget::onLoad(){
    ui->lineEdit_projectPath->setText( GetVal( DK_ProjectDir ).toString() );
    ui->lineEdit_projectName->setText( GetVal( DK_ProjectName ).toString() );
    ui->checkBox_setStartProject->setChecked( GetVal( DK_ProjectStart ).toBool() );

    qDebug()<<__FUNCTION__<< GetVal( DK_ProjectDir ).toString() ;
}

void SettingProjectWidget::setDefault()
{
    //有个问题,这个函数调用比其他setDefault()落后,路径不存在怎么办? 用独立的信号触发
    QString name = "project_1";
    QString dir = QApplication::applicationDirPath() + "/" +name+"/";

    //自动创建文件夹
    QDir tDir;
    if(!tDir.exists(dir))
        tDir.mkpath(dir);

    qDebug()<<__FUNCTION__<<dir;

    //project
    SetVal( DK_ProjectName, name ) ;
    SetVal( DK_ProjectDir, dir ) ;
    SetVal( DK_ProjectStart, true ) ;

}
