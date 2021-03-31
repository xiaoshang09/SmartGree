#ifndef ROLLINGDATETIME_H
#define ROLLINGDATETIME_H

#include <QObject>
#include <QDateTime>


#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  RollingTime :  public QObject
{
    Q_OBJECT
public :
    RollingTime();

    //单位S
    virtual void setCycle(int roll);
    virtual int cycle();

    //单位S
    virtual void start(int roll=0);
    virtual void stop();

    QDateTime getDate(){return m_date;}

signals:
    void rolling(const QDateTime&dt,const QDateTime&next);//滚动

protected:
    virtual void timerEvent(QTimerEvent *event);

protected:
    QDateTime m_date;
    int m_checkInv;//检查间隔,单位毫秒
    int m_cycle;//滚动间隔,单位秒
    int m_timerId;
};

#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  RollingDate :  public RollingTime
{
    Q_OBJECT
public:
    RollingDate();

    //单位day
    virtual void setCycle(int roll) override;

protected:
    virtual void timerEvent(QTimerEvent *event);
};

#endif // ROLLINGDATETIME_H
