#include "nosoftdogdialog.h"
#include "ui_nosoftdogdialog.h"
#include "uihelper.h"
#include <QtCore>
#include <QtWidgets>
NoSoftdogDialog::NoSoftdogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoSoftdogDialog)
{
    ui->setupUi(this);

	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setWindowTitle(tr("启动失败"));
	this->installEventFilter(this);

	UiLayer::moveFormToCenter(this);
}

NoSoftdogDialog::~NoSoftdogDialog()
{
    delete ui;
}

void NoSoftdogDialog::on_pushButton_sysClose_clicked()
{
    this->close();
}

void NoSoftdogDialog::on_pushButton_confirm_clicked()
{
    this->close();
}
bool NoSoftdogDialog::eventFilter(QObject *watched, QEvent *event)
{
	if (watched == this)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->buttons() == Qt::LeftButton)
			{
				this->setProperty("PressPos", mouseEvent->globalPos());
				this->setProperty("IsLeftPress", true);
				return true;
			}
		}
		else if (event->type() == QEvent::MouseMove)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->buttons() == Qt::LeftButton)
			{
				bool isLeftPress = this->property("IsLeftPress").toBool();
				if (isLeftPress)
				{
					if (this->isMaximized())
					{
						return true;
					}
					auto pressPos = this->property("PressPos").toPoint();
					auto offsetPos = mouseEvent->globalPos() - pressPos;
					this->move(this->geometry().topLeft() + offsetPos);
					this->setProperty("PressPos", mouseEvent->globalPos());
					return true;
				}
			}
		}
		else if (event->type() == QEvent::MouseButtonRelease)
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->buttons() == Qt::LeftButton)
			{
				this->setProperty("IsLeftPress", false);
				this->setProperty("PressPos", QPoint(0, 0));
				return true;
			}
		}
	}

	return QWidget::eventFilter(watched, event);
}
