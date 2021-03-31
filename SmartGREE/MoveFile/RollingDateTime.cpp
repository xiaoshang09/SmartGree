#include "RollingDateTime.h"
#include <QDateTime>
#include <QDebug>

//最大间隔 每分钟检查一次
#define D_CHECK_TIME_MS_MAX 60*1000

///#############################################################
/// \brief RollingTime::RollingTime
///#############################################################
RollingTime::RollingTime()
{
    m_timerId=0;
}

void RollingTime::start(int roll){

    if( roll > 0 ){
        setCycle( roll );
    }

    if( m_cycle<1 ){
        setCycle(1);
    }

#ifdef D_TEST
    //TODO 修改这里可以虚构数据
    m_date= QDateTime::fromString("2019-11-28 08:03:00", "yyyy-MM-dd hh:mm:ss");
    m_timerId = startTimer(100);
    qDebug()<<"start===m_date==="<< m_date;
#else
    m_date=QDateTime::currentDateTime();
    m_timerId = startTimer(m_checkInv);
#endif
}

void RollingTime::stop()
{
    killTimer(m_timerId);
}

void RollingTime::setCycle(int roll)
{
    m_cycle = roll;
    m_checkInv = roll*1000/2;
    if(m_checkInv>D_CHECK_TIME_MS_MAX){
        m_checkInv=D_CHECK_TIME_MS_MAX;
    }
}

int RollingTime::cycle()
{
    return m_cycle;
}

void RollingTime::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

#ifdef D_TEST
    static QDateTime ttt= m_date;
    //直接改时间,直接滚动
    QDateTime nextT = m_date.addSecs( 13 );

    int days = ttt.daysTo(nextT);
    if( days>0 ){
        qDebug()<<"过去几天=="<< days << ttt.toString( "yyyy-MM-dd hh:mm:ss" ) ;
        qDebug()<<"nextT=="<< nextT.toString( "yyyy-MM-dd hh:mm:ss" ) ;
        ttt=nextT;
    }

    emit rolling( m_date, nextT );
    m_date = nextT;
#else
    int sec = m_date.secsTo(  QDateTime::currentDateTime() );
    if(sec >= m_cycle){
        QDateTime nextT = QDateTime::currentDateTime();//会有抖动
        emit rolling( m_date, nextT );
        m_date = nextT;
    }
#endif
}


///#############################################################
/// \brief RollingDate::RollingDate
///#############################################################
RollingDate::RollingDate()
{

}

void RollingDate::setCycle(int roll)
{
    m_cycle = roll;
    m_checkInv = D_CHECK_TIME_MS_MAX;
}

void RollingDate::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    int day = m_date.daysTo(  QDateTime::currentDateTime() );
    if(day >= m_cycle){
        QDateTime nextT = m_date.addDays( m_cycle );
        emit rolling( m_date, nextT );
        m_date = nextT;
    }
}
