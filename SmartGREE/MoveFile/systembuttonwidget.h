#ifndef SYSTEMBUTTONWIDGET_H
#define SYSTEMBUTTONWIDGET_H

#include <QWidget>

namespace Ui {
class SystemButtonWidget;
}

class SystemButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SystemButtonWidget(QWidget *parent = 0);
    ~SystemButtonWidget();

    void setNoMinButton();
    void setNoMaxButton();
    void setNoCloseButton();

    void setCloseButtonSigEmit(bool isEmit);
    void setParentWidget(QWidget *parent);
    void changedMaxButtonState();
private slots:
    void on_pushButton_sysMin_clicked();
    void on_pushButton_sysClose_clicked();
    void on_pushButton_sysMax_clicked();
signals:
    void closeButtonSig();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void setStyle();
private:
    Ui::SystemButtonWidget *ui;
    QWidget *mParent;
    bool mCloseButtonSigEmit;
};

#endif // SYSTEMBUTTONWIDGET_H
