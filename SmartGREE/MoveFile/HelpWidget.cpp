#include "HelpWidget.h"
#include "ui_HelpWidget.h"


HelpWidget::HelpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpWidget)
{
    ui->setupUi(this);

    ui->textEdit->setText(u8"帮助文档暂无无法使用！");
    ui->textEdit->setTextColor(QColor("#a6a6a6"));

}

HelpWidget::~HelpWidget()
{
    delete ui;
}

