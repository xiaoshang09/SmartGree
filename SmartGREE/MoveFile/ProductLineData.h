#ifndef PRODUCTLINEDATA_H
#define PRODUCTLINEDATA_H

#include <QDateTime>
#include <QMap>
#include <QPair>
#include <QVariant>

#include <QDebug>

//QList<S_PdData> pddata;

struct S_PdData{///产品结果
    int tindex;//时间INT
    QString time;//时间
    QString pdid;//产品
    int code;//预留
    int result;//1=ok, else=NG
    QByteArray data;//图片//源数据
};

//日期//
//QMap<quint64, QMap<QString, S_PdData> > productDataSrc;//所有产品源数据
struct S_PdOutputData{///产品产出
    QString id;//产品
    int output;//产量
    int output_ok;//合格量
    int output_ng;//不良数量
    float output_ratio;//良品率//这个应在统计中计算,不用保存数据库
    float output_taktTime;//节拍//这个应在统计中计算,不用保存数据库 //TODO大跨度计算节拍没有意义
    QString data;

    S_PdOutputData(){  init();  }
    void init(){
        clear();
    }
    void clear(){
        output=0;//产量
        output_ok=0;//合格量
        output_ng=0;//不良数量
        output_ratio=0;//良品率//无法读取保存的值,只能根据合格量产量重新计算
        output_taktTime=0;//节拍
    }

};



///    切换次数和故障次数记录当天的  发生时间点
struct S_ProductLineData{///生产线(支持多个产品同时生产)
    //辅助数据
    QDateTime date;//此周期开始时间
    QDateTime nextDate;//下一周期时间
    QMap<QString, S_PdOutputData*> product;//所有产品列表
    QList<QString> pdIDList;//此周期内产品列表, 小于或等于product.keys()

    //需保存的数据
    int error_count;//故障次数
    int productChangeCount;//切换次数
    float power_consumption;//耗电量, 不同产品耗电不太一样? 启动生产的时候为固定值,关闭生产时为0,
    QString curID;//当前产品

    S_ProductLineData(){  init();  }
    ~S_ProductLineData(){  init();  }
    void init(){
        foreach(S_PdOutputData *ptr,product){
            if( ptr ){
                ptr->init();
                delete ptr;
            }
        }
        product.clear();
        clear();
    }
    void clear(){
        foreach(S_PdOutputData *ptr,product){
            if( ptr ){
                ptr->clear();
            }
        }
        error_count=0;//故障次数
        productChangeCount=0;//产量
        power_consumption=0;//耗电量, 不同产品耗电不太一样?
    }
};


//统计时按产品,再按时间是最好的.
///统计细分-年月日 aggregate,subdivision
//struct QList< QVariantList >
//{
//    //统计数据一般都是遍历,没必要用map
//    QList< QVariantList >  data;//数据表

//    //原型:: QList< QList< QVariant > >
//};

typedef QMap<QString, QList< QVariantList > > TableDataSub;//(产品)数据分类统计表


enum ENUM_SumDataType{
    E_PUBLIC_DATA,//公用数据
    E_PRODUCT_DATA,//产品
};



#endif // PRODUCTLINEDATA_H
