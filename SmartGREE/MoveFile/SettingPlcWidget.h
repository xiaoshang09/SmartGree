#ifndef SETTINGPLCWIDGET_H
#define SETTINGPLCWIDGET_H

#include <QWidget>

namespace Ui {
class SettingPlcWidget;
}

class SettingPlcWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingPlcWidget(QWidget *parent = 0);
    ~SettingPlcWidget();

public slots:
    void onValueChanged(const char *key);
    void onLoad();
    void setDefault();

private slots:
    void on_lineEdit_ip_textChanged(const QString &arg1);
    void on_spinBox_port_valueChanged(int arg1);
    void on_checkBox_auto_toggled(bool checked);

private:
    Ui::SettingPlcWidget *ui;
};

#endif // SETTINGPLCWIDGET_H
