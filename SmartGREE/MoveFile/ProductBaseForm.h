#ifndef PRODUCTBASEFORM_H
#define PRODUCTBASEFORM_H

#include <QWidget>

#include "ProductDataForm.h"
#include "ProductNowForm.h"
#include "ProductSqlForm.h"


namespace Ui {
class ProductBaseForm;
}

class ProductBaseForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProductBaseForm(QWidget *parent = 0);
    ~ProductBaseForm();

protected:
    virtual void showEvent(QShowEvent *event);

private slots:
    void showMain();
    void on_pushButton_now_clicked();
    void on_pushButton_data_clicked();
    void on_pushButton_sql_clicked();

private:
    void initWin();

private:
    Ui::ProductBaseForm *ui;
    ProductNowForm* m_nowFm;
    ProductDataForm* m_dataFm;
    ProductSqlForm* m_sqlFm;
};

#endif // PRODUCTBASEFORM_H
