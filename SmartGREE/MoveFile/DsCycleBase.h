#ifndef DSCYCLEBASE_H
#define DSCYCLEBASE_H

#include "ProductLineData.h"

#include <QDateTime>
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QDebug>

///如果不清空的话,往后几天,甚至跳月份的的数据怎么关联计算呢,也挺麻烦的
#define D_CLEAR_WHEN_ROLLING

//#define D_zhengshu  以整点时间滚动

#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT   DsCycleBase : public QObject
{
    /// 一个产品一个表,还是多个表??? 一个表多个产品吧,表多了麻烦
    /// 每一条数据(=当前周期的统计数据,这些数据每次保存后都清零,重新算)
    ///
    /// 定时器到,会调用 doRolling()
    /// 产品变化调用 doIdChanged()
    /// 产出数据变动调用 lock(); getData(); 改数据..., doCurrentDataChanged(); unlock();
    ///

    Q_OBJECT
    Q_PROPERTY(bool isLowestLevel READ isLowestLevel WRITE setIsLowestLevel NOTIFY isLowestLevelChanged)

public:

    DsCycleBase(QObject* parent=nullptr);

    void setCycle(int val){m_cycle = val;}
    int cycle(){return m_cycle;}

    void lock(){
        qlock.lock();
    }

    void unlock(){
        qlock.unlock();
    }

    S_ProductLineData* getData(){ return &dat; }//需酌情使用加锁

    bool isLowestLevel() const
    {
        return m_isLowestLevel;
    }

protected:
    bool m_isLowestLevel;//级别最低
    int m_cycle;//统计周期
    QMutex qlock;//锁dat, getData()
    S_ProductLineData dat; //此周期数据
    float m_power;

signals:
    void isLowestLevelChanged(bool isLowestLevel);

public slots:

    void setIsLowestLevel(bool isLowestLevel)
    {
        if (m_isLowestLevel == isLowestLevel)
            return;

        m_isLowestLevel = isLowestLevel;
        emit isLowestLevelChanged(m_isLowestLevel);
    }

    virtual void doCurrentDataChanged();//每当有数据变更//调用此函数前请加锁//默认使用dat.nextDate时间

    //产品变更//id为空时,停止当前产品
    virtual void doIdAdd(const QString& id);

    virtual void doIdDel(const QString& id);

    virtual void doIdClear();

    //已启动生产//功耗+
    virtual void doProductStarted();

    //待机中//功耗-
    virtual void doProductWating();

    //计算节拍等
    virtual void doMath(S_ProductLineData* pdat);
    virtual float math_ratio(S_PdOutputData* ptr);//计算方法
    virtual float math_taktTime(S_PdOutputData* ptr);//计算方法//(一片耗时多少)节拍=总时间/总量
    virtual float math_power(S_ProductLineData* ptr);//计算方法

    //滚动
    virtual void doRolling(const QDateTime& dt,const QDateTime& nextDt);

protected:
    //TODO
    virtual bool canRolling(const QDateTime& dt)=0;//判断是否能滚动
    virtual void doCurrentDataSave()=0;//当前最新的数据保存
    virtual void doRollingSave()=0;//滚动保存

private:
    friend class DsGroup;


};


#endif // DSCYCLEBASE_H
