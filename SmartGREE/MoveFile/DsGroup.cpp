#include "DsGroup.h"
#include <QDebug>
#include <ctime>

///###同时创建 日,月,年
/// 查今日 = 某日表统计条目
/// 查今月 = 某月表统计条目 +查今日
/// 查今年 = 某年表统计条目 +查今月
///
/// 查某日 = 某日表统计条目
/// 查某月 = 某月表统计条目
/// 查某年 = 某年表统计条目
///
///当前表仅仅用作开机之后的缓存,重启后清零
///
//// //思路1: 滚动
/// 如果当日有缓存呢?绝大部分情况有,直接插入当前分钟条目吧,不能使用缓存, 这种情况
/// 月表必须存当前总值,否则每天都不满的时候,下一天启动机器,统计月表的时候,不存在数据,统计重新插入? 月表没有数据
/// 滚动逻辑太复杂了
/// //思路2, 只建立日表,查询月的时候用数据库统计 31次
/// //思路3, 只建立月表,查询年和日的时候用数据库统计指令//4w条
/// //先用思路3做,简单
///
///###数据#  假设 每分钟插 1条,
/// 日 60*24 =1440条
/// 月            *30 =43200条
/// 年                      *12 =518400条
///
///###表# 假设按日建表,日表存每分钟数据
/// 日 1
/// 月  =30
/// 年     *12 =365个表
///
DsGroup::DsGroup(QObject *parent):QObject(parent)
{
    m_dsMonth.setIsLowestLevel( true );
    qsrand(time(NULL));//随机种子
}

void DsGroup::doRolling(const QDateTime &dt, const QDateTime &nextDt)
{

#ifdef D_TEST

        int n = qrand() % 30;
        int  output = 50+n;

        int m = qrand() % 12;
        int  output_ok = m>3?1:0;
        int  output_ng = output_ok?0:1;
        float power_consumption = 120.0+m;

        switch(n){
        case 0:
            historyDs()->doIdAdd("BBBBB");
            historyDs()->doIdAdd("AAAAA");
            historyDs()->doIdAdd("CCCCCC");
            break;
        case 1:
            historyDs()->doIdAdd("BBBBB");
            historyDs()->doIdDel("CCCCCC");
            break;
        case 2:
            historyDs()->doIdAdd("CCCCCC");
            break;
        case 3:
            historyDs()->doIdAdd("AAAAA");
            historyDs()->doIdDel("BBBBB");
            break;
        case 4:
            historyDs()->doIdAdd("BBBBB");
            historyDs()->doIdDel("AAAAA");
            break;
        case 5:
            historyDs()->doIdAdd("DDDDDDDDD");
            historyDs()->doIdAdd("EEEEEEE");
            historyDs()->doIdDel("BBBBB");
            break;
        case 6:
            historyDs()->doIdDel("CCCCCC");
            historyDs()->doIdDel("AAAAA");
            break;
        case 7:
            historyDs()->doIdDel("CCCCCC");
            historyDs()->doIdAdd("AAAAA");
            break;
        case 8:
            historyDs()->doIdClear();
            break;

        }

        //修改数据
        historyDs()->lock();
        S_ProductLineData*  pdat= historyDs()->getData();
        for(QString id : pdat->pdIDList){
            S_PdOutputData* ptr = pdat->product.value(id);
            ptr->output = output;
            ptr->output_ok = output_ok;
            ptr->output_ng = output_ng;
        }
        pdat->power_consumption  = power_consumption+n;
        historyDs()->unlock();

//        qDebug()<<__FUNCTION__<<n << dt.toString("yyyy-MM-dd HH:mm:ss") << output  ;
        insertPdOutput(D_SqlTablePdOutput, pdat->date, pdat->curID, 0, m>3?1:0);

        m_dsMonth.doCurrentDataChanged();
        m_dsMonth.doRolling(dt,nextDt);

#else
        if( m_dsMonth.canRolling(dt) ) {
            if( !m_dsMonth.isLowestLevel() ){
                //统计上一级数据
                //                selectDay();
            }
            m_dsMonth.doRolling(dt,nextDt);
        }
#endif
}

void DsGroup::stop()
{
    m_rollingObj.stop();
}

void DsGroup::start()
{
    creatTableWhenStart();

    QObject::connect( &m_rollingObj,  &RollingTime::rolling, this, &DsGroup::doRolling );
    m_rollingObj.start( D_DataRolling_SEC );

    historyDs()->setCycle( m_rollingObj.cycle() );

#ifdef D_TEST

    historyDs()->doIdAdd("CCCCCC");
    QTimer::singleShot( 1000, [=](){
        historyDs()->doIdClear();
        historyDs()->doIdAdd("AAAAA");
    });

    QTimer::singleShot( 5000, [=](){
        historyDs()->doIdAdd("BBBBB");
    });

    QTimer::singleShot( 11330, [=](){
        historyDs()->doIdClear();
        historyDs()->doIdAdd("CCCCCC");
    });

#endif
}
