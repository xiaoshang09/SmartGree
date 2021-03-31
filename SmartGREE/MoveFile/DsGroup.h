#ifndef DSGROUP_H
#define DSGROUP_H

#include "DsDay.h"
#include "DsMonth.h"
#include "DsYear.h"
#include "RollingDateTime.h"
#include <QDebug>

#ifdef D_TEST
#include <ctime>
#endif

//分钟为单位保存
//#define D_DataRolling_SEC 60
//小时为单位保存
#define D_DataRolling_SEC 60*60

/**
 * @brief The DsGroup class //管理滚动,滚动数据,当前数据
 */
#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  DsGroup : public QObject
{
    Q_OBJECT
public:
    static DsGroup* instance(){
        DsGroup* m_obj = new DsGroup();
        return m_obj;
    }
    explicit DsGroup(QObject* parent=nullptr);

    //操作数据需酌情加锁,正常情况只有UI线程用
    DsCycleBase* historyDs(){ return &m_dsMonth; }

public slots:
    void start();
    void stop();

private slots:
    void doRolling(const QDateTime& dt,const QDateTime& nextDt);

private:

    RollingTime m_rollingObj;//滚动
    //    DsDay m_dsDay;//滚动//历史回溯
    DsMonth m_dsMonth;//滚动//历史回溯
    //    DsYear m_dsYear;//滚动//历史回溯
};

#endif // DSGROUP_H
