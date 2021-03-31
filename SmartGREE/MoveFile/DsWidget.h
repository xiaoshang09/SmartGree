#ifndef DSWIDGET_H
#define DSWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include <QDate>

namespace Ui {
class DsWidget;
}

class MyWebView;
#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  DsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DsWidget(QWidget *parent = 0);
    ~DsWidget();

    enum ENUM_SELECT_TYPE{
        E_YEAR,
        E_MONTH,
        E_DAY,
    };

public slots:
    void buttonToggled(int id, bool check);

private slots:
    void loadFinished();

    void on_btnRefresh_clicked();

    void pushButton_day_clicked();

    void pushButton_month_clicked();

    void pushButton_year_clicked();

    void on_pushButton_tj_clicked();

    void on_calendarWidget_activated(const QDate &date);

    void on_calendarWidget_selectionChanged();

    void on_calendarWidget_currentPageChanged(int year, int month);

    void on_pushButton_p_clicked();

    void on_pushButton_next_clicked();

private:
    Ui::DsWidget *ui;
    bool isOK=false;
    QButtonGroup btGroup;
    int m_id;
    QDate m_date;
    MyWebView* m_web;

};

#endif // DSWIDGET_H
