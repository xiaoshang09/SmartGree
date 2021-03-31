#include "ProductDataForm.h"
#include "ui_ProductDataForm.h"

ProductDataForm::ProductDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductDataForm)
{
    ui->setupUi(this);
}

ProductDataForm::~ProductDataForm()
{
    delete ui;
}
