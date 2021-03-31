#ifndef SETTINGCAMERAWIDGET_H
#define SETTINGCAMERAWIDGET_H

#include <QWidget>

namespace Ui {
class SettingCameraWidget;
}

class SettingCameraWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingCameraWidget(QWidget *parent = 0);
    ~SettingCameraWidget();

public slots:
    void onValueChanged(const char *key);
    void onLoad();
    void setDefault();

private:
    Ui::SettingCameraWidget *ui;
};

#endif // SETTINGCAMERAWIDGET_H
