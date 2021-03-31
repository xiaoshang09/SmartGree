#ifndef SettingSystemWidget_H
#define SettingSystemWidget_H

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QWidget>
#endif
#include <QEvent>

namespace Ui {
class SettingSystemWidget;
}

class SettingSystemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingSystemWidget(QWidget *parent = 0);
    ~SettingSystemWidget();

public slots:
    void onValueChanged(const char *key);
    void onLoad();
    void setDefault();

private slots:
    void on_radioButton_zh_toggled(bool checked);
    void on_radioButton_en_toggled(bool checked);

private:
    Ui::SettingSystemWidget *ui;
};

#endif // SettingSystemWidget_H
