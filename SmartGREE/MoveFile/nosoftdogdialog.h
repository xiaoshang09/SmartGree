#ifndef NOSOFTDOGDIALOG_H
#define NOSOFTDOGDIALOG_H

#include <QDialog>

namespace Ui {
class NoSoftdogDialog;
}

class NoSoftdogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NoSoftdogDialog(QWidget *parent = 0);
    ~NoSoftdogDialog();

private slots:
    void on_pushButton_sysClose_clicked();

    void on_pushButton_confirm_clicked();
protected:
	bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::NoSoftdogDialog *ui;
};

#endif // NOSOFTDOGDIALOG_H
