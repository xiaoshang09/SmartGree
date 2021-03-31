#ifndef SETTINGMAINTENANCE_H
#define SETTINGMAINTENANCE_H

#include <QWidget>
#include <QButtonGroup>
#include <QGroupBox>


namespace Ui {
class SettingMaintenance;
}
class SettingMaintenance : public QWidget
{
    Q_OBJECT

public:
    explicit SettingMaintenance(QWidget *parent = 0);
    ~SettingMaintenance();

    enum ENUM_MODE{
        E_MATEN,//保养
        E_REP,//更换
    };

    QString title();
    void setTitle(const QString& text);
    ENUM_MODE mode(){return m_mode;}
    void setMode(ENUM_MODE md){m_mode = md;}

    /**
     * @brief setNextMaintenance
     * @param isSet //=true 并不会验证当前时间是否到达维护时间,直接根据last和inv设置下一次保养时间
     * @return ok=0
     */
    static int setNextMaintenance( bool isSet );
    static int setNextReplacement( bool isSet );

public slots:
    void onValueChanged(const char *key);
    void onLoad();
    void setDefault();
    void updateUI();
    void buttonClicked(int inv);

private slots:
    void on_pushButton_default_clicked();
    void on_spinBox_val_valueChanged(int arg1);

private:
    QButtonGroup btnGroup;
    ENUM_MODE m_mode;


    enum ENUM_DATE_INV{
        E_CUSTOM=0,//
        E_DAY=1,
        E_DAY3=3,
        E_WEEK=7,
        E_MONTH=30,
        E_MONTH3=90,
        E_MONTH6=180,
        E_YEAR=365,
    };

private:
    Ui::SettingMaintenance *ui;
};

#endif // SETTINGMAINTENANCE_H
