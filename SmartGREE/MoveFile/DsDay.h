#ifndef DSDAY_H
#define DSDAY_H


#include "DsCycleBase.h"
#include "SqlFunc.h"

#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  DsDay : public DsCycleBase
{
    Q_OBJECT
public:
    DsDay();
private:
    friend class DsGroup;

protected:
    virtual bool canRolling(const QDateTime& dt);

    virtual void doCurrentDataSave();

    virtual void doRollingSave();
};


#endif // DSDAY_H
