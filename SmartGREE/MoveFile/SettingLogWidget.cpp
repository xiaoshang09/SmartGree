#include "SettingLogWidget.h"
#include "ui_settinglogwidget.h"
#include "SettingDatas.h"

SettingLogWidget::SettingLogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingLogWidget)
{
    ui->setupUi(this);

    connect(SettingDatas::instance(), SIGNAL(valueChanged(const char *) ), this, SLOT(onValueChanged(const char *) ) );
    connect(SettingDatas::instance(), SIGNAL(loadFinished() ), this, SLOT(onLoad()));
    connect(SettingDatas::instance(), SIGNAL(sigSetDefault() ), this, SLOT(setDefault()));
}

SettingLogWidget::~SettingLogWidget()
{
    delete ui;
}

void SettingLogWidget::onValueChanged(const char *key)
{

}

void SettingLogWidget::onLoad()
{

}

void SettingLogWidget::setDefault()
{
    //log
    SetVal( DK_LogDir,   "./log" ) ;
    SetVal( DK_LogFType, "log" ) ;
    SetVal( DK_LogDateType, "yyyy-MM-dd" ) ;
    SetVal( DK_LogAutoCreatDir, true ) ;
}

