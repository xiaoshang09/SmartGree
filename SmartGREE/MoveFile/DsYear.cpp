#include "DsYear.h"

DsYear::DsYear()
{
    setCycle(1); //统计周期/month
}

bool DsYear::canRolling(const QDateTime &dt)
{
    int val = QDateTime::currentDateTime().date().month() - dt.date().month();
    return (val >= m_cycle);
}

void DsYear::doCurrentDataSave()
{
//    QString tableSuf = dat.date.toString(  D_DATEFORMAT_YEAR_TableName );
//    creatTableWhenRolling(tableSuf);//自动建表

//    QString date = dat.nextDate.toString(D_DATEFORMAT_SECOND_SQL);
    insertWhenRolling(D_SqlTableProductLine, dat.nextDate, &dat);
}

void DsYear::doRollingSave()
{
//    QString tableSuf = dat.date.toString(  D_DATEFORMAT_YEAR_TableName );
//    creatTableWhenRolling(tableSuf);//自动建表

//    QString date = dat.date.toString(D_DATEFORMAT_SECOND_SQL);
    insertWhenRolling(D_SqlTableProductLine, dat.date, &dat);
}

