#include "settingbasewidget.h"
#include "ui_SettingBaseWidget.h"

#include "SettingDatas.h"
#include "SettingPlcWidget.h"
#include "SettingImageWidget.h"
#include "SettingLogWidget.h"
#include "SettingProjectWidget.h"
#include "SettingSystemWidget.h"
#include "SettingRobotWidget.h"
#include "SettingCameraWidget.h"
#include "SettingMaintenance.h"
#include "CameraWidget.h"


#include <QToolbox>
#include <QVBoxLayout>
#include <QToolButton>
#include <QDebug>


SettingBaseWidget::SettingBaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingBaseWidget)
{
    ui->setupUi(this);

    //clear
    while(ui->toolBox->count()){
        ui->toolBox->removeItem(0);
    }
    connect( &m_btnGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(btnClicked(QAbstractButton *)));


    addGroup(u8"工程配置",   QStringLiteral(":/image/user2.png"));
    addGroup(u8"PLC配置",   QStringLiteral(":/image/plc-on.png"));
//    addGroup(u8"机器人配置", QStringLiteral(":/image/robot-on.png"));
    addGroup(u8"相机配置",   QStringLiteral(":/image/camera-on.png"));
    addGroup(u8"保养维护",   QStringLiteral(":/image/rep.png"));
    addGroup(u8"日志配置",   QStringLiteral(":/image/log2.png"));
    addGroup(u8"系统配置",   QStringLiteral(":/image/set2.png"));


    SettingProjectWidget* projectWt=new SettingProjectWidget;
    SettingPlcWidget* plcWt=new SettingPlcWidget;
//    SettingRobotWidget* robWt=new SettingRobotWidget;
//    SettingImageWidget* imgWt=new SettingImageWidget;
//    SettingCameraWidget* cameraWt=new SettingCameraWidget;
    QWidget* cameraWt = CameraWidget::instance()->settingsWt();

    SettingLogWidget* logWt=new SettingLogWidget;
    SettingSystemWidget* sysWt=new SettingSystemWidget;
    SettingMaintenance* maten= new SettingMaintenance();
    maten->setMode(SettingMaintenance::E_MATEN);
    maten->setTitle( "设备保养时间" );
    SettingMaintenance* rep= new SettingMaintenance();
    rep->setMode(SettingMaintenance::E_REP);
    rep->setTitle( "设备维护更换时间" );


    addItem(projectWt,  u8"工程配置",   u8"基本配置" );
    addItem(plcWt,      u8"PLC配置",   u8"基本配置" );
    addItem(maten,      u8"保养维护",   u8"保养时间设置", QStringLiteral(":/image/clean.png"));
    addItem(rep,        u8"保养维护",   u8"设备更换设置", QStringLiteral(":/image/rep2.png"));
    addItem(cameraWt,   u8"相机配置",   u8"基本配置" );
    addItem(logWt,      u8"日志配置",   u8"基本配置" );
    addItem(sysWt,      u8"系统配置",   u8"基本配置" );

    ui->stackedWidget->setCurrentWidget(projectWt);
}

SettingBaseWidget::~SettingBaseWidget()
{
    delete ui;
}


void SettingBaseWidget::addItem(QWidget *wt, const QString &parent, const QString &text, const QString &icon)
{
    QWidget* page= m_groupMap.value(parent);
    if(!page){
        return;
    }

    QToolButton* btn = new QToolButton(page);
    if(!icon.isEmpty()){
        QIcon _icon(icon);
        btn->setIcon(_icon);
        btn->setIconSize(QSize(16, 16));
    }
    if(!text.isEmpty()){
        btn->setText(text);
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    }

    QVBoxLayout *layout = static_cast<QVBoxLayout*>( page->layout());
    layout->addWidget( btn );

    ui->stackedWidget->addWidget(wt);
    m_btnMapWt.insert( btn, wt );
    m_btnGroup.addButton( btn );
    if(!m_pageMapBtns.contains(page)){
        m_pageMapBtns.insert( page, btn );
    }
}

void SettingBaseWidget::addGroup(const QString &text, const QString &icon)
{
    QWidget* page = new QWidget();
    page->setObjectName(text);
    QVBoxLayout *verticalLayout = new QVBoxLayout(page);

    QIcon _icon(icon);
    ui->toolBox->addItem(page, _icon, text );

    m_groupMap.insert(text, page);

    Q_UNUSED(verticalLayout)
}

void SettingBaseWidget::btnClicked(QAbstractButton *btn)
{
    ui->stackedWidget->setCurrentWidget(m_btnMapWt.value(btn));
}

void SettingBaseWidget::on_toolBox_currentChanged(int index)
{
    Q_UNUSED(index)

    QWidget* page= ui->toolBox->currentWidget();
    if(!page){
        return;
    }

    QAbstractButton *btn= m_pageMapBtns.value(page);
    emit m_btnGroup.buttonClicked( btn );
}

void SettingBaseWidget::on_toolButton_reset_clicked(){

}
