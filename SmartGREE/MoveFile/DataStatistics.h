#ifndef DATASTATISTICS_H
#define DATASTATISTICS_H

//#include "RollingDateTime.h"

//#include "DsSecond.h"
//#include "DsDay.h"
//#include "DsMonth.h"
//#include "DsYear.h"



//1 当日监控：
//同样实时统计
//产量、合格量、不良数量、良品率、当前节拍、切换次数、故障次数、和耗电量（均已当天为单位开始重新计算）；
//产量、合格量、不良数量、节拍、耗电量以小时为单位，统计平均值；
//切换次数和故障次数记录当天的  发生时间点

//2 以周为单位统计：
//回顾统计产量、合格量、不良数量、良品率、当前节拍、切换次数、故障次数、和耗电量（以天为单位进行统计）；
//可以查看每一周的历史数据，通过点击“上一周”和“下一周”进行查看，并将日期显示在文本框中；

//3 以月为单位统计：
//回顾统计产量、合格量、不良数量、良品率、当前节拍、切换次数、故障次数、和耗电量（以月为单位进行统计）；
//可以查看某一年的历史数据，通过下拉列表选择年份；

//4 以年为单位统计：
//回顾统计产量、合格量、不良数量、良品率、当前节拍、切换次数、故障次数、和耗电量（以年为单位进行统计）；

//5 总的统计，可以统计该设备自投入使用以来，总的产量、合格量、不良数量、良品率、当前节拍、切换次数、故障次数、和耗电量：

//6 自定义时间的统计，可以统计某一段时间该设备使用时，总的产量、合格量、不良数量、良品率、当前节拍、切换次数、故障次数、和耗电量：

#include <QObject>
#include "DsGroup.h"
#include "SqlFunc.h"

#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  DataStatistics :public QObject
{
public:
    static DataStatistics* instance(){
        DataStatistics* m_obj = new DataStatistics();
        return m_obj;
    }

    DataStatistics(QObject *parent =nullptr);

    //切换产品
    void idChanged(const QString& id){
        DsGroup::instance()->historyDs();
    }

    //错误发生
    void errorHapend(const QString& msg){

    }

    //产量+1
    void addOutput(const QString& id, bool ok){

    }

    //已启动生产//功耗+
    void productStarted(){

    }

    void productWating(){//待机中
        //
    }

private:

};



#endif // DATASTATISTICS_H
