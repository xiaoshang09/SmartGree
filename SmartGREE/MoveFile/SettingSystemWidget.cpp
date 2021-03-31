#include "SettingSystemWidget.h"
#include "ui_SettingSystemWidget.h"
#include "SettingDatas.h"


SettingSystemWidget::SettingSystemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingSystemWidget)
{
    ui->setupUi(this);

    connect(SettingDatas::instance(), SIGNAL(valueChanged(const char *) ), this, SLOT(onValueChanged(const char *) ) );
    connect(SettingDatas::instance(), SIGNAL(loadFinished() ), this, SLOT(onLoad()));
    connect(SettingDatas::instance(), SIGNAL(sigSetDefault() ), this, SLOT(setDefault()));
}

SettingSystemWidget::~SettingSystemWidget()
{
    delete ui;
}

void SettingSystemWidget::onValueChanged(const char *key)
{

}

void SettingSystemWidget::onLoad()
{

}

void SettingSystemWidget::setDefault()
{
    //system
    SetVal( DK_SysLang,   u8"简体中文" ) ;
    SetVal( DK_SysName,   u8"windows" ) ;
    SetVal( DK_SysStart,  false ) ;
}

void SettingSystemWidget::on_radioButton_zh_toggled(bool checked)
{
    if(checked)
    {
        SetVal( DK_SysLang,   u8"简体中文" ) ;
    }
}

void SettingSystemWidget::on_radioButton_en_toggled(bool checked)
{
    if(checked)
    {
        SetVal( DK_SysLang,   u8"English" ) ;
    }
}
