#include "SettingRobotWidget.h"
#include "ui_SettingRobotWidget.h"
#include "SettingDatas.h"


SettingRobotWidget::SettingRobotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingRobotWidget)
{
    ui->setupUi(this);
//    ui->comboBox_RobotBrand->clear();

    connect(SettingDatas::instance(), SIGNAL(valueChanged(const char *) ), this, SLOT(onValueChanged(const char *) ) );
    connect(SettingDatas::instance(), SIGNAL(loadFinished() ), this, SLOT(onLoad()));
    connect(SettingDatas::instance(), SIGNAL(sigSetDefault() ), this, SLOT(setDefault()));
}


void SettingRobotWidget::removeRobot(const QString &robotName)
{
    if(_nameList.contains(robotName))
        _nameList.removeOne(robotName);
}

void SettingRobotWidget::onValueChanged(const char *key)
{

}

void SettingRobotWidget::onLoad()
{

}

void SettingRobotWidget::setDefault()
{
}

SettingRobotWidget::~SettingRobotWidget()
{
    delete ui;
}

void SettingRobotWidget::on_pushButton_Create_clicked()
{
}

void SettingRobotWidget::on_pushButton_Cancel_clicked()
{
    this->setVisible(false);
}
