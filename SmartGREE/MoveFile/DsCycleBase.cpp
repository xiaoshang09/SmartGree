#include "DsCycleBase.h"
#include "SqlFunc.h"


DsCycleBase::DsCycleBase(QObject *parent)
    :QObject(parent)
{
    m_power=50.0;
    m_isLowestLevel=false;
    m_cycle=10;
    dat.nextDate=dat.date=QDateTime::currentDateTime();
}

void DsCycleBase::doCurrentDataChanged()
{
    doCurrentDataSave();
}

void DsCycleBase::doIdAdd(const QString &id)
{
    if(id.isEmpty())
        return;

    lock();
    qDebug()<< __FUNCTION__ << "id==" << id;

    //添加某产品
    S_PdOutputData* ptr= dat.product.value(id,NULL);
    if( !ptr ){
        ptr= new S_PdOutputData();
        ptr->id = id;
        dat.product.insert( id, ptr );
    }

    if(!dat.pdIDList.contains(id)){
        dat.pdIDList.append( id );
    }
    if( dat.curID != id ){
        dat.curID = id;
        dat.productChangeCount++;
        insertProductIdChanged(QDateTime::currentDateTime(),id);
    }
    unlock();
}

void DsCycleBase::doIdDel(const QString &id)
{
    if(id.isEmpty())
        return;

    //要强制滚一次保证保存了数据//用dat.date,所以不会有间隔问题
    doRolling(dat.date,dat.nextDate);

    //停止某产品
    lock();
    int index = dat.pdIDList.indexOf( id );
    if(index>=0 && index<dat.pdIDList.size()){
        dat.pdIDList.removeAt( index );
        if(dat.pdIDList.isEmpty()){
            dat.curID.clear();
        }
    }
    unlock();
}

void DsCycleBase::doIdClear()
{
    //要强制滚一次保证保存了数据//用dat.date,所以不会有间隔问题
    doRolling(dat.date,dat.nextDate);

    lock();
    dat.pdIDList.clear();
    dat.curID.clear();
    unlock();
}

void DsCycleBase::doProductStarted()
{
    m_power = 1200.0;
}

void DsCycleBase::doProductWating()
{
    m_power = 50.0;
}

void DsCycleBase::doMath(S_ProductLineData *pdat)
{
    if( !(pdat->pdIDList.isEmpty()) ){
        for(QString id : pdat->pdIDList){
            S_PdOutputData* ptr = pdat->product.value(id);
            ptr->output_ratio = math_ratio(ptr);
            ptr->output_taktTime = math_taktTime(ptr);
        }
        pdat->power_consumption = math_power(pdat);
    }
}

void DsCycleBase::doRolling(const QDateTime &dt, const QDateTime &nextDt)
{
    if( dat.curID.isEmpty() ){
        qDebug()<<"dat.curID is empty!" ;
        return;
    }

    lock();

    dat.date = dt;
    dat.nextDate = nextDt;

    if( !dat.pdIDList.isEmpty() ){
        doMath(&dat);
        doRollingSave();//保存数据库
//            emit hadSaveData(dat);//让更高级保存当前统计数据

#ifdef D_CLEAR_WHEN_ROLLING
        //清空产量
        dat.clear();

        //清空当前条目
        doCurrentDataSave();
#endif

    }else{
        qDebug()<<__FUNCTION__<< "no data!";
    }

    unlock();
}

float DsCycleBase::math_ratio(S_PdOutputData *ptr)
{
    if(ptr->output_ok + ptr->output_ng<1){
        return 0.0;
    }
    return (float)ptr->output_ok/(ptr->output);
}

float DsCycleBase::math_power(S_ProductLineData *ptr)
{
    return m_power + qrand() % 8;
}

float DsCycleBase::math_taktTime(S_PdOutputData *ptr)
{
    return (float)ptr->output_ok*60.0/m_cycle;//节拍=总时间/总量
}
