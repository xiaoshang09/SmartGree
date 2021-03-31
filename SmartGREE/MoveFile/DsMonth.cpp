#include "DsMonth.h"

DsMonth::DsMonth()
{
    setCycle(1); //统计周期/day
}

bool DsMonth::canRolling(const QDateTime &dt)
{
    int val = QDateTime::currentDateTime().date().day() - dt.date().day();
    return (val >= m_cycle);
}

void DsMonth::doCurrentDataSave()
{
//    QString tableSuf = dat.date.toString(  D_DATEFORMAT_MONTH_TableName );
//    creatTableWhenRolling(tableSuf);//自动建表//月表存每天的数据

//    QString date = dat.nextDate.toString(D_DATEFORMAT_SECOND_SQL);//nextDate
    insertWhenRolling(D_SqlTableProductLine, dat.nextDate, &dat);
}

void DsMonth::doRollingSave()
{
//    QString tableSuf = dat.date.toString(  D_DATEFORMAT_MONTH_TableName );
//    creatTableWhenRolling(tableSuf);//自动建表//月表存每天的数据

//    QString date = dat.date.toString(D_DATEFORMAT_SECOND_SQL);
    insertWhenRolling(D_SqlTableProductLine, dat.date, &dat);
}
