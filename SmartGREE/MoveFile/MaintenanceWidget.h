#ifndef MAINTENANCEWIDGET_H
#define MAINTENANCEWIDGET_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class MaintenanceWidget;
}

class MaintenanceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MaintenanceWidget(QWidget *parent = 0);
    ~MaintenanceWidget();

    enum ENUM_MODE{
        E_MATEN,//设备保养
        E_REP,//设备更换
    };
    void setMode(ENUM_MODE mode);
    void setMsg(const QString& msg);

protected:
    virtual void showEvent(QShowEvent *event);
    virtual void timerEvent(QTimerEvent *event);

public slots:
    void checkTime(); //判断时间到,弹窗
private slots:
    void initTable();
    void showTable();
    void on_pushButton_add_clicked();
    void sortByColumn(int col);

private:
    Ui::MaintenanceWidget *ui;
    QSqlTableModel* model;
    QSqlDatabase _db;
    bool isOK=false;
    ENUM_MODE m_mode;
};

#endif // MAINTENANCEWIDGET_H
