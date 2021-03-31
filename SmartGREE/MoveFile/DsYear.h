#ifndef DSYEAR_H
#define DSYEAR_H


#include "DsCycleBase.h"
#include "SqlFunc.h"


#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  DsYear : public DsCycleBase
{
    Q_OBJECT
public:
    DsYear();
private:
    friend class DsGroup;

protected:
    virtual bool canRolling(const QDateTime& dt);

    virtual void doCurrentDataSave();

    virtual void doRollingSave();

};


#endif // DSYEAR_H
