#ifndef SETTINGLOGWIDGET_H
#define SETTINGLOGWIDGET_H

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QWidget>
#endif


namespace Ui {
class SettingLogWidget;
}

class SettingLogWidget : public QWidget
{
    Q_OBJECT

public:
    enum SaveTime
    {
        SaveThreeDay,
        SaveOneMonth,
        SaveThreeMonth,
        SaveCustom,
    };

    explicit SettingLogWidget(QWidget *parent = 0);
    ~SettingLogWidget();

public slots:
    void onValueChanged(const char *key);
    void onLoad();
    void setDefault();

private:
    Ui::SettingLogWidget *ui;
};

#endif // SETTINGLOGWIDGET_H
