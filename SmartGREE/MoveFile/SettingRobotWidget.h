#ifndef SETTINGROBOTWIDGET_H
#define SETTINGROBOTWIDGET_H

#include <QWidget>

namespace Ui {
class SettingRobotWidget;
}

class SettingRobotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingRobotWidget(QWidget *parent = nullptr);
    ~SettingRobotWidget();

public slots:
    void onValueChanged(const char *key);
    void onLoad();
    void setDefault();
    void removeRobot(const QString &robotName);

private slots:
    void on_pushButton_Create_clicked();
    void on_pushButton_Cancel_clicked();

private:
    Ui::SettingRobotWidget *ui;
    static SettingRobotWidget*   _SettingRobotWidget;
    QStringList            _nameList;
};

#endif // SETTINGROBOTWIDGET_H
