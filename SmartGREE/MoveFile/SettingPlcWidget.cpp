#include "SettingPlcWidget.h"
#include "ui_SettingPlcWidget.h"
#include "SettingDatas.h"

#include <QHostAddress>

SettingPlcWidget::SettingPlcWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingPlcWidget)
{
    ui->setupUi(this);

    connect(SettingDatas::instance(), SIGNAL(valueChanged(const char *) ), this, SLOT(onValueChanged(const char *) ) );
    connect(SettingDatas::instance(), SIGNAL(loadFinished() ), this, SLOT(onLoad()));
    connect(SettingDatas::instance(), SIGNAL(sigSetDefault() ), this, SLOT(setDefault()));

}

SettingPlcWidget::~SettingPlcWidget()
{
    delete ui;
}

void SettingPlcWidget::onValueChanged(const char *key)
{

//    if(key==DK_PlcIp){

//    }else
//    if(key==DK_PlcPort){

//    }else
//    if(key==DK_PlcAutoConet){

//    }
    //    updateUI();

    if(DK_COMP(DK_PlcState,key)){
        ui->label_state->setText( GetVal( DK_PlcState ).toBool()?u8"已连接":u8"已断开" );
    }
}

void SettingPlcWidget::onLoad(){
    ui->lineEdit_ip->setText( GetVal( DK_PlcIp ).toString() );
    ui->spinBox_port->setValue( GetVal( DK_PlcPort ).toInt() );
    ui->checkBox_auto->setChecked( GetVal( DK_PlcAutoConet ).toBool() );
}

void SettingPlcWidget::setDefault(){
    //plc
    SetVal( DK_PlcIp , "192.168.1.20" ) ;
    SetVal( DK_PlcPort, 4096 ) ;
    SetVal( DK_PlcAutoConet, true ) ;
    //
}

void SettingPlcWidget::on_lineEdit_ip_textChanged(const QString &arg1)
{
//    static QPalette defaultPalette = ui->lineEdit_ip->palette();

    QHostAddress test;
    if (arg1.split(".").size()!=4 || !test.setAddress( arg1 ))
    {//127.0.0.1
        ui->lineEdit_ip->setStyleSheet("color:red");
        qWarning()<<"非法IP:"<<ui->lineEdit_ip->text() ;
    }else{
        ui->lineEdit_ip->setStyleSheet("");
        SetVal(DK_PlcIp, arg1 );
    }
}

void SettingPlcWidget::on_spinBox_port_valueChanged(int arg1)
{
    if(arg1<1000){
        ui->spinBox_port->setStyleSheet("color:red");
        qWarning()<<u8"非法端口:"<<ui->spinBox_port->value();
    }else{
        ui->spinBox_port->setStyleSheet("");
        SetVal(DK_PlcPort, arg1 );
    }
}

void SettingPlcWidget::on_checkBox_auto_toggled(bool checked)
{
    SetVal(DK_PlcAutoConet, checked );
}
