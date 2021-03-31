#ifndef SQLPRODUCTLINE_H
#define SQLPRODUCTLINE_H


#include "XDatabase.h"
#include "ProductLineData.h"


//qt没有这些转换
//#define D_DATEFORMAT_HOUR_SHOW    QStringLiteral("HH")
//#define D_DATEFORMAT_DAY_SHOW     QStringLiteral("MM-dd")
//将日期缩短-显示在表格上
enum ENUM_DATEFORMAT_SHOW{ //特殊点的日期,QT无法自由转换,只能自己实现
    D_DATEFORMAT_NONE=0, //

    D_DATEFORMAT_SECOND_SHOW, //HH:mm:sss
    D_DATEFORMAT_HOUR_SHOW, //HH
    D_DATEFORMAT_DAY_SHOW,  //MM-dd
    D_DATEFORMAT_MONTH_SHOW,//yyyy-MM
    D_DATEFORMAT_YEAR_SHOW, //yyyy
};

#ifndef D_DATEFORMAT_SECOND_SQL
//Qt转SQL的日期格式//插入数据时全部使用统一格式方便统计,D_DATEFORMAT_SECOND_SQL
#define D_DATEFORMAT_TIME_SQL    QStringLiteral("HH:mm:ss")
#define D_DATEFORMAT_SECOND_SQL  QStringLiteral("yyyy-MM-dd HH:mm:ss")
#define D_DATEFORMAT_DAY_SQL     QStringLiteral("yyyy-MM-dd")
#define D_DATEFORMAT_MONTH_SQL   QStringLiteral("yyyy-MM")
#define D_DATEFORMAT_YEAR_SQL    QStringLiteral("yyyy")
//表格式
#define D_DATEFORMAT_DAY_TableName     QStringLiteral("yyyy_MM_dd")
#define D_DATEFORMAT_MONTH_TableName   QStringLiteral("yyyy_MM")
#define D_DATEFORMAT_YEAR_TableName    QStringLiteral("yyyy")
#endif

//数据库设置
#ifdef D_USE_MYSQL
#define D_DB_FILE_A     QStringLiteral("testGree.db")
#else
#define D_DB_FILE_A     QStringLiteral("./testGree.db")
#endif
#define D_DB_WAITTIME 5

////表前缀+日期
//#define PdOutput_        QStringLiteral("PdOutput_")
//#define ProductLine_     QStringLiteral("ProductLine_")

//杂项// 根据key来获取数据,直接存二进制
#define D_SqlTableKeyVal          "TableKeyVal"
//log
#define D_SqlTableLog             "TableLog"
//生产线/功耗等//这个需要滚动
#define D_SqlTableProductLine       "TableProductLine"
//错误
#define D_SqlTableErrorHappened     "TableErrorHappened"
//切换产品
#define D_SqlTableProductIdChange   "TableProductIdChange"
//产能
#define D_SqlTablePdOutput          "TablePdOutput"
//发生的错误
#define D_SqlTableReplacement       "TableReplacement"
//维护保养
#define D_SqlTableMaintenance       "TableMaintenance"

//公用数据关键字ID,这些公用数据当成一个产品ID,存入TableDataSub中
#define D_PdLinePublicDataKey      QStringLiteral("OthersData")


enum ENUM_SHOWDATE_TYPE{
    E_YEAR,
    E_MONTH,
    E_DAY,
    E_HOUR,
};

//void insertTable_ProductLine(int tindex, const QString& time,
//                             float data1,float data2,float data3,  int data4,int data5,int data6 );
//void insertTable_ErrorHappened(int tindex, const QString& time, int code,  const QString& msg);
//void insertTable_IdChanged(int tindex, const QString& time,  const QString& curid);
//void insertTable_Output(S_PdData* pd);

///================== func =================

extern QString changeDateFormatShow(const QString &from, ENUM_DATEFORMAT_SHOW toF);

//将多条合并统计为一条数据
extern void SumProductLineData(const TableDataSub& from, TableDataSub& toData, const QString& date);
//数据转json文本  //TODO 提供ID替换功能,变中文显示
extern bool TableDataSubToJson(TableDataSub &dat, QJsonObject& js, ENUM_DATEFORMAT_SHOW xDataFormat=D_DATEFORMAT_NONE, bool sortXData=false);

extern bool ProductLineDataToTableDataSub(const S_ProductLineData *indata, TableDataSub &outdata);

///================== write =================
//select * from sqlite_master where type = 'table' and name = 't_cmpt_cp'

//对某一行或某几行添加PRIMARY KEY或UNIQUE约束，那么数据库会自动为这些列创建索引
extern void creatTable(const QString& table,const QStringList& args);
extern void creatTableWhenStart();//软件启动时就建立
extern void creatTableWhenRolling(const QString& table);//这个表一直根据时间创建
// 写入数据库
extern void insertReplacement(int count, const QString &timeSet, const QString& name, const QString& note);//保存一条设备更换记录
extern void insertMaintenance(int count, const QString &timeSet, const QString& name, const QString& note);//保存一条保养记录
extern void insertProductIdChanged(const QDateTime &date, const QString& curid);//保存一条产品切换时间
extern void insertErrorHappened(const QDateTime &date, int code, const QString& msg);//保存一条错误时间
//extern void insertlog(const QString& key,const QString& msg);
extern void insertPdOutput(const QString& table, const QDateTime& date, const QString &pdid, int code, int result, const QByteArray &data="");//预留table用于分表
extern void insertWhenRolling(const QString& table, const QDateTime& date, S_ProductLineData* pdat);//预留table用于分表

///================== read =================


//统计
extern int selectPdOutputSum(QSqlQuery *query, ENUM_SHOWDATE_TYPE type,
                         const QString &table, quint64 start, quint64 end, TableDataSub &datSub);
//统计
extern int selectPowerSum(QSqlQuery *query, ENUM_SHOWDATE_TYPE type,
                         const QString &table, quint64 start, quint64 end, TableDataSub &datSub);
//细分
extern int selectPdOutput(QSqlQuery *query, ENUM_SHOWDATE_TYPE type, quint64 start, quint64 end, TableDataSub &datSub);

//细分
extern int selectPower(QSqlQuery *query, ENUM_SHOWDATE_TYPE type, quint64 start, quint64 end, TableDataSub &datSub);

//所有符合条件的条目//dat为出参//selectValues越少越快
extern int selectDetails(QSqlQuery *query, const QString &table, quint64 startIndex, quint64 endIndex, QList< QVector<QVariant> > &dat, const QString &selectValues="*");

//从数据库取数据细分, 注意: dat为出参
extern int selectDay(QSqlQuery *query, const QDate &dt, TableDataSub& dat);//显示所有条目
extern int selectMonth(QSqlQuery *query, const QDate &dt, TableDataSub& dat);//显示每日统计
extern int selectYear(QSqlQuery *query, const QDate &dt, TableDataSub& dat);//显示每月统计

////统计整日月年, 注意: dat为出参
//extern int selectDaySum(QSqlQuery *query, const QDateTime&dt, TableDataSub& dat);
//extern int selectMonthSum(QSqlQuery *query, const QDateTime& dt, TableDataSub& dat);
//extern int selectYearSum(QSqlQuery *query, const QDateTime& dt, TableDataSub& dat);

//按时间查,可跨表, 详细 start<= time <end,  注意: dat为出参, 数据量可能很大,所以自动优化统计  //同时,格式化时间
extern int selectWithTime(QSqlQuery *query, const QDateTime& start, const QDateTime& end, TableDataSub& dat);
//按时间查,可跨表, 按月统计 start<= time <end,  注意: dat为出参
extern int selectWithTimeSum(QSqlQuery *query, const QDateTime& start, const QDateTime& end, TableDataSub& dat);


//当前产量不需要写入数据库,保存到内存,重启为0
//extern void replaceNowData(S_ProductLineData *pdat);


#endif // SQLPRODUCTLINE_H
