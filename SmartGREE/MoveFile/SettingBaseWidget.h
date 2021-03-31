#ifndef SETTINGBASEWIDGET_H
#define SETTINGBASEWIDGET_H


#include <QWidget>
#include <QGridLayout>
#include <QButtonGroup>
#include <QMap>

namespace Ui {
class SettingBaseWidget;
}

class SettingBaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingBaseWidget(QWidget *parent = 0);
    ~SettingBaseWidget();

    void addItem(QWidget *wt,const QString &parent,const QString& text, const QString& icon="");
    void addGroup(const QString& text, const QString& icon="");

private slots:
    void btnClicked(QAbstractButton *btn);
    void on_toolBox_currentChanged(int index);
    void on_toolButton_reset_clicked();

private:
    Ui::SettingBaseWidget *ui;
    QMap<QString, QWidget*> m_groupMap;
    QMap<QWidget*, QAbstractButton*> m_pageMapBtns;
    QMap<QAbstractButton*, QWidget*> m_btnMapWt;
    QButtonGroup m_btnGroup;
};

#endif // SETTINGBASEWIDGET_H
