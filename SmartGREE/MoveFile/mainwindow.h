#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ProductBaseForm.h"
#include "SettingBaseWidget.h"
#include "LogWidget.h"
//#include "HelpWidget.h"
#include "AboutWidget.h"
#include "MaintenanceWidget.h"
#include "logindialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void showEvent(QShowEvent *event);
    virtual void timerEvent(QTimerEvent *event);

private slots:
    void showMain();//主页
    void onStartUp();//每次启动后做一点事//5s
    void onLoad();
    void onValueChanged(const char*key);
    void titleClicked(int id);
    void on_toolButton_maten_clicked();
    void on_toolButton_eq_replace_clicked();

protected:
    void setStyle();

private:
    void initWin();

private:
    Ui::MainWindow *ui;
    ProductBaseForm* m_productFm;
    SettingBaseWidget* m_settingsFm;
    LogWidget* m_logFm;
    AboutWidget* aboutWt;
    MaintenanceWidget* matenWt;//保养
    MaintenanceWidget* repWt;//更换
};

#endif // MAINWINDOW_H
