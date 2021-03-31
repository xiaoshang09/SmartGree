#include "DsDay.h"


DsDay::DsDay()
{
    setCycle(1);//统计周期/S
}

bool DsDay::canRolling(const QDateTime &dt)
{
    int val = dt.secsTo(QDateTime::currentDateTime());
    return (val >= m_cycle);
}

void DsDay::doCurrentDataSave()
{
//    QString tableSuf = dat.date.toString(  D_DATEFORMAT_DAY_TableName );
//    creatTableWhenRolling(tableSuf);//自动建表

//    QString date = dat.date.toString(D_DATEFORMAT_SECOND_SQL);
    insertWhenRolling(D_SqlTableProductLine, dat.nextDate, &dat);
}

void DsDay::doRollingSave()
{
//    QString tableSuf = dat.date.toString(  D_DATEFORMAT_DAY_TableName );
//    creatTableWhenRolling(tableSuf);//自动建表

//    QString date = dat.date.toString(D_DATEFORMAT_SECOND_SQL);
    insertWhenRolling(D_SqlTableProductLine, dat.date, &dat);
}

