#include "SettingMaintenance.h"
#include "ui_SettingMaintenance.h"
#include "SettingDatas.h"
#include <SqlFunc.h>
#include <QDate>

SettingMaintenance::SettingMaintenance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingMaintenance)
{
    ui->setupUi(this);
    setMode(E_MATEN);

    btnGroup.addButton(ui->radioButton_custom,E_CUSTOM );
    btnGroup.addButton(ui->radioButton_day,E_DAY );
    btnGroup.addButton(ui->radioButton_week,E_WEEK );
    btnGroup.addButton(ui->radioButton_month, E_MONTH );
    btnGroup.addButton(ui->radioButton_month3, E_MONTH3 );
    btnGroup.addButton(ui->radioButton_month6, E_MONTH6 );
    btnGroup.addButton(ui->radioButton_year, E_YEAR );

    connect(&btnGroup, SIGNAL(buttonClicked(int)) , this ,SLOT(buttonClicked(int)) );

    connect(SettingDatas::instance(), SIGNAL(valueChanged(const char *) ), this, SLOT(onValueChanged(const char *) ) );
    connect(SettingDatas::instance(), SIGNAL(loadFinished() ), this, SLOT(onLoad()));
    connect(SettingDatas::instance(), SIGNAL(sigSetDefault() ), this, SLOT(setDefault()));
}

SettingMaintenance::~SettingMaintenance()
{
    delete ui;
}

void SettingMaintenance::setTitle(const QString &text)
{
    ui->label_title->setText(text);
}

QString SettingMaintenance::title()
{
    return ui->label_title->text();
}

void SettingMaintenance::on_pushButton_default_clicked()
{
    setDefault();
}

void SettingMaintenance::onValueChanged(const char *key)
{
//    qDebug()<<__FUNCTION__<<"test=0==" << m_mode << key << (key==DK_MaintenanceInv) << (key==DK_ReplacementInv);
    switch(m_mode){
    case E_REP:
    {
        if( DK_COMP(key, DK_ReplacementInv) ){
            setNextReplacement(true);
        }else if( DK_COMP(key, DK_ReplacementTimeNext) ){
            updateUI();
        }
    }
        break;
    case E_MATEN:
    default:
    {
        if( DK_COMP(key, DK_MaintenanceInv) ){
            setNextMaintenance(true);
        }else if( DK_COMP(key, DK_MaintenanceTimeNext) ){
            updateUI();
        }
    }
        break;
    }
}

void SettingMaintenance::onLoad()
{
    updateUI();
}

void SettingMaintenance::setDefault()
{
    switch(m_mode){
    case E_REP:
    {
        SetVal( DK_ReplacementInv,  7 ) ;
    }
        break;
    case E_MATEN:
    default:
    {
        SetVal( DK_MaintenanceInv,  7 ) ;
    }
        break;
    }
}

void SettingMaintenance::updateUI()
{
    int inv = 1;
    QString last;
    QString next;

    switch(m_mode){
    case E_REP:
    {
        inv  = GetVal( DK_ReplacementInv ).toUInt() ;
        last = GetVal( DK_ReplacementTimeLast ).toString() ;
        next = GetVal( DK_ReplacementTimeNext ).toString() ;
    }
        break;
    case E_MATEN:
    default:
    {
        inv  = GetVal( DK_MaintenanceInv ).toUInt() ;
        last = GetVal( DK_MaintenanceTimeLast ).toString() ;
        next = GetVal( DK_MaintenanceTimeNext ).toString() ;
    }
        break;
    }

    QAbstractButton *btn = btnGroup.button( inv );
    if(btn){
        btn->setChecked(true);
    }else{
        ui->spinBox_val->setValue( inv );
        ui->radioButton_custom->setChecked(true);
    }

    ui->label_last->setText(last);
    ui->label_next->setText(next);

    qDebug()<<__FUNCTION__<< "last,next===" << last << next;
}

void SettingMaintenance::buttonClicked(int inv)
{
    qDebug()<<"buttonClicked== "<< inv;
    switch(m_mode){
    case E_REP:
    {
        if(inv<1){
            SetVal( DK_ReplacementInv, ui->spinBox_val->value() ) ;
        }else{
            SetVal( DK_ReplacementInv, inv ) ;
        }
    }
        break;
    case E_MATEN:
    default:
    {
        if(inv<1){
            SetVal( DK_MaintenanceInv, ui->spinBox_val->value() ) ;
        }else{
            SetVal( DK_MaintenanceInv, inv ) ;
        }
    }
        break;
    }

}

int SettingMaintenance::setNextMaintenance(bool isSet)
{

    QString last= GetVal(DK_MaintenanceTimeLast).toString();
    if(last.isEmpty()){//重置
        qDebug(u8"WARN,保养时间未设置! 已重置");
        last=QDate::currentDate().toString( D_DATEFORMAT_DAY );
        SetVal( DK_MaintenanceTimeLast, last ) ;
    }

    int inv = GetVal(DK_MaintenanceInv).toInt();
    QDate nextDt = QDate::fromString(last, D_DATEFORMAT_DAY);
    nextDt=nextDt.addDays( inv );

    if(isSet){//不滚动
        QString next= nextDt.toString( D_DATEFORMAT_DAY );
        SetVal( DK_MaintenanceTimeNext, next ) ;
    }else{//滚动
        if(nextDt>QDate::currentDate()){
            return -1;//"未到保养时间!"
        }
        nextDt=QDate::currentDate();

        //这次
        QString next= nextDt.toString( D_DATEFORMAT_DAY );
        SetVal( DK_MaintenanceTimeLast, next ) ;

        //下次
        nextDt=nextDt.addDays( inv );
        next= nextDt.toString( D_DATEFORMAT_DAY );
        SetVal( DK_MaintenanceTimeNext, next ) ;
    }


    return 0;
}

int SettingMaintenance::setNextReplacement(bool isSet)
{
    QString last= GetVal(DK_ReplacementTimeLast).toString();
    if(last.isEmpty()){//重置
        qDebug(u8"WARN,维护时间未设置! 已重置");
        last=QDate::currentDate().toString( D_DATEFORMAT_DAY );
        SetVal( DK_ReplacementTimeLast, last ) ;
    }

    int inv = GetVal(DK_ReplacementInv).toInt();
    QDate nextDt = QDate::fromString(last, D_DATEFORMAT_DAY);
    nextDt=nextDt.addDays( inv );

    if(isSet){//不滚动
        QString next= nextDt.toString( D_DATEFORMAT_DAY );
        SetVal( DK_ReplacementTimeNext, next ) ;
    }else{//滚动
        if(nextDt>QDate::currentDate()){
            return -1;//"未到维护更换时间!"
        }
        nextDt=QDate::currentDate();

        //这次
        QString next= nextDt.toString( D_DATEFORMAT_DAY );
        SetVal( DK_ReplacementTimeLast, next ) ;

        //下次
        nextDt=nextDt.addDays( inv );
        next= nextDt.toString( D_DATEFORMAT_DAY );
        SetVal( DK_ReplacementTimeNext, next ) ;
    }

    return 0;
}

void SettingMaintenance::on_spinBox_val_valueChanged(int arg1)
{
    if(ui->radioButton_custom->isChecked())
    {
        switch(m_mode){
        case E_REP:
        {
            SetVal( DK_ReplacementInv, arg1 ) ;
        }
            break;
        case E_MATEN:
        default:
        {
            SetVal( DK_MaintenanceInv, arg1 ) ;
        }
            break;
        }
    }

}
