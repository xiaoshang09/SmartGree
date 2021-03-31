#include "aboutwidget.h"
#include "ui_aboutwidget.h"
#include <QDesktopServices>
#include <QUrl>

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWidget)
{
    ui->setupUi(this);
    ui->label_webUrl->installEventFilter(this);

    auto version = qApp->applicationVersion();
    ui->label_name->setText(tr(u8"GREE 底盘自动上线智能系统 V%1").arg(version));

    QString builtType = "";
    QString builtTime = QString("Built on %1 %2 ").arg(__DATE__).arg(__TIME__);

#ifdef _DEBUG
    builtType = "(Debug)";
#else
    builtType = "(Release)";
#endif
    ui->label_builtTime->setText(builtTime + builtType);
}

AboutWidget::~AboutWidget()
{
    delete ui;
}

bool AboutWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->label_webUrl)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QDesktopServices::openUrl(QUrl(ui->label_webUrl->text()));
        }
    }
    return QWidget::eventFilter(watched,event);
}
