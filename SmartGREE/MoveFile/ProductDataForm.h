#ifndef PRODUCTDATAFORM_H
#define PRODUCTDATAFORM_H

#include <QWidget>

namespace Ui {
class ProductDataForm;
}

class ProductDataForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProductDataForm(QWidget *parent = 0);
    ~ProductDataForm();

private:
    Ui::ProductDataForm *ui;
};

#endif // PRODUCTDATAFORM_H
