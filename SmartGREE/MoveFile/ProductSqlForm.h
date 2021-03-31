#ifndef PRODUCTSQLFORM_H
#define PRODUCTSQLFORM_H

#include <QWidget>
#include <QButtonGroup>
#include <QDate>
#include <QSqlDatabase>

namespace Ui {
class ProductSqlForm;
}

class QSqlQuery;
class QSqlTableModel;
class ProductSqlForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProductSqlForm(QWidget *parent = 0);
    ~ProductSqlForm();

    enum ENUM_SELECT_TYPE{
        E_YEAR,
        E_MONTH,
        E_DAY,
    };

protected:
    virtual void showEvent(QShowEvent *event);
    int selecTable(const QString &table, const QDate &start, const QDate &end);

private slots:
    void pushButton_day_clicked();
    void pushButton_month_clicked();
    void pushButton_year_clicked();
    void buttonToggled(int id, bool check);
    void sortByColumn(int col);

    void on_calendarWidget_activated(const QDate &date);
    void on_calendarWidget_selectionChanged();
    void on_calendarWidget_currentPageChanged(int year, int month);
    void on_pushButton_p_clicked();
    void on_pushButton_next_clicked();
private:
    Ui::ProductSqlForm *ui;
    QSqlTableModel *model;
    QSqlDatabase _db ;
    QButtonGroup btGroup;
    int m_id;
    QDate m_date;
    bool isOK;
    QString m_table;
    QList< QVector<QVariant> > dat;
};

#endif // PRODUCTSQLFORM_H
