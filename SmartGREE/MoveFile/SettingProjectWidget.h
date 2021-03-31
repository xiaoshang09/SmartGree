#ifndef SETTINGPROJECTWIDGET_H
#define SETTINGPROJECTWIDGET_H

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QWidget>
#endif

namespace Ui {
class SettingProjectWidget;
}

class SettingProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingProjectWidget(QWidget *parent = 0);
    ~SettingProjectWidget();

public slots:
    void onValueChanged(const char *key);
    void onLoad();
    void setDefault();

private:
    Ui::SettingProjectWidget *ui;
};

#endif // SETTINGPROJECTWIDGET_H
