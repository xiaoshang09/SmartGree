#ifndef DSMONTH_H
#define DSMONTH_H

#include "DsCycleBase.h"
#include "SqlFunc.h"


#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  DsMonth : public DsCycleBase
{
    Q_OBJECT
public:
    DsMonth();
private:
    friend class DsGroup;

protected:
    virtual bool canRolling(const QDateTime& dt);

    virtual void doCurrentDataSave();

    virtual void doRollingSave();

};

#endif // DSMONTH_H
