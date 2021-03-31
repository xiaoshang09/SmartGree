#include "systembuttonwidget.h"
#include "ui_systembuttonwidget.h"
#include <QMouseEvent>
#include <QTextStream>
#include <QDebug>

//
SystemButtonWidget::SystemButtonWidget(QWidget *parent) :
    QWidget(parent),mParent(NULL),
    ui(new Ui::SystemButtonWidget)
{
    ui->setupUi(this);
    mCloseButtonSigEmit = false;
    auto layout = this->layout();
    if(layout)
    {
        layout->setAlignment(ui->pushButton_sysMin,Qt::AlignTop);
        layout->setAlignment(ui->pushButton_sysMax,Qt::AlignTop);
        layout->setAlignment(ui->pushButton_sysClose,Qt::AlignTop);
    }
    ui->pushButton_sysMin->clearFocus();
    ui->pushButton_sysMax->clearFocus();
    ui->pushButton_sysClose->clearFocus();
}

SystemButtonWidget::~SystemButtonWidget()
{
    delete ui;
}

void SystemButtonWidget::setNoMinButton()
{
    ui->pushButton_sysMin->setHidden(true);
}

void SystemButtonWidget::setNoMaxButton()
{
    ui->pushButton_sysMax->setHidden(true);
}

void SystemButtonWidget::setNoCloseButton()
{
    ui->pushButton_sysClose->setHidden(true);
}

void SystemButtonWidget::setCloseButtonSigEmit(bool isEmit)
{
    mCloseButtonSigEmit = isEmit;
}

void SystemButtonWidget::setParentWidget(QWidget *parent)
{
    mParent = parent;
    if(mParent)
    {
        mParent->setWindowFlags(Qt::FramelessWindowHint);
        mParent->installEventFilter(this);
    }
}

void SystemButtonWidget::changedMaxButtonState()
{
    if(mParent->isMaximized())
    {
        ui->pushButton_sysMax->setToolTip(tr(u8"还原"));
        ui->pushButton_sysMax->setChecked(true);
    }else
    {
        ui->pushButton_sysMax->setToolTip(tr(u8"最大化"));
        ui->pushButton_sysMax->setChecked(false);
    }
}

void SystemButtonWidget::on_pushButton_sysMin_clicked()
{
    if(mParent)
        mParent->showMinimized();
}

void SystemButtonWidget::on_pushButton_sysClose_clicked()
{
    if(mCloseButtonSigEmit)
    {
        emit closeButtonSig();
    }else
    {
        if(mParent)
            mParent->close();
    }

}

void SystemButtonWidget::on_pushButton_sysMax_clicked()
{
    if(mParent)
    {
        if(mParent->isMaximized())
        {
            ui->pushButton_sysMax->setToolTip(tr(u8"还原"));
            ui->pushButton_sysMax->setChecked(true);
            mParent->showNormal();
        }else
        {
            ui->pushButton_sysMax->setToolTip(tr(u8"最大化"));
            ui->pushButton_sysMax->setChecked(false);
            mParent->showMaximized();
        }
    }
}

bool SystemButtonWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == mParent)
    {
        if(event->type() == QEvent::Show ||
                event->type() == QEvent::Resize)
        {
            changedMaxButtonState();
        }

        if(event->type() == QEvent::Close)
        {
            mParent = NULL;
        }
    }
    return QObject::eventFilter(watched,event);
}

void SystemButtonWidget::setStyle()
{
    QString styleStr;
    QFile ff;
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

    this->setStyleSheet( styleStr );
}
