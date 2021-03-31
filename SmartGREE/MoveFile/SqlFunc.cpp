#include "SqlFunc.h"

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <QDebug>


//将细分统计合并
void SumProductLineData(const TableDataSub &from, TableDataSub &toData, const QString &date)
{
    QMap<QString, QList< QVariantList > >::const_iterator imap;
    for (imap = from.constBegin(); imap != from.constEnd(); ++imap) {

        QString key = imap.key();
        const QList< QVariantList >& val = imap.value();

        if(val.size()<1)
            continue;

        ENUM_SumDataType type=E_PUBLIC_DATA;
        if(key != D_PdLinePublicDataKey){
            type=E_PRODUCT_DATA;
        }

        //        QString dateStr ;
        QString pdid ;
        int output=0;
        int output_ok=0;
        int output_ng=0;
        float output_ratio=0;
        float output_taktTime=0;

        int error_count=0;//故障次数
        int productChangeCount=0;//切换产品
        float power_consumption=0;//耗电量, 不同产品耗电不太一样?
        QString curid ;

        //遍历所有行
        QList< QVariantList >::const_iterator ilist;
        for (ilist = val.constBegin(); ilist != val.constEnd(); ++ilist) {
            int len = (*ilist).size() ;

            switch(type){
            case E_PUBLIC_DATA:
                if( len!= 4){ //error
                    qDebug()<<__FUNCTION__<<__LINE__<<"ERROR:1 data count!"<<type<<key<<len << (*ilist);
                }else{
                    //                    dateStr = (*ilist).at(0).toString();//time
                    error_count += (*ilist).at(1).toInt();//错误
                    productChangeCount += (*ilist).at(2).toInt();//切换
                    power_consumption += (*ilist).at(3).toFloat();//功耗
                }
                break;
            case E_PRODUCT_DATA:
                if( len!= 7){//error
                    qDebug()<<__FUNCTION__<<__LINE__<<"ERROR:2 data count!"<<type<<key<<len << (*ilist);
                }else{
                    //                    dateStr = (*ilist).at(0).toString();//time
                    //                    id = (*ilist).at(1).toInt();//id
                    pdid = key;
                    output += (*ilist).at(2).toInt();//产量
                    output_ok += (*ilist).at(3).toInt();//合格量
                    output_ng += (*ilist).at(4).toInt();//不良数量
                    output_ratio += (*ilist).at(5).toFloat();//良品率
                    output_taktTime += (*ilist).at(6).toFloat();//节拍
                }
                break;
            default:
                break;
            }
        }

        QVariantList tData;
        switch(type){
        case E_PUBLIC_DATA:

            tData.append( date );
            tData.append( error_count );
            tData.append( productChangeCount );
            tData.append( power_consumption );

            break;
        case E_PRODUCT_DATA:
            //取平均
            output_ratio = output_ratio/val.size();
            output_taktTime = output_taktTime/val.size();

            tData.append( date );
            tData.append( pdid );
            tData.append( output );
            tData.append( output_ok );
            tData.append( output_ng );
            tData.append( output_ratio );
            tData.append( output_taktTime );
            break;
        default:
            break;
        }

        //        qDebug()<<__FUNCTION__ << key << tData;

        toData[ key ].append( tData );

    }
}

bool ProductLineDataToTableDataSub( const S_ProductLineData *indata, TableDataSub &outdata){

    QString date =indata->date.toString(D_DATEFORMAT_SECOND_SQL);

    for(QString id : indata->pdIDList){
        S_PdOutputData* ptr = indata->product.value(id);

        QVariantList row;
        row.append( date );
        row.append(ptr->id);//产品
        row.append(ptr->output);//产量
        row.append(ptr->output_ok);//合格量
        row.append(ptr->output_ng);//不良数量
        row.append(ptr->output_ratio);//良品率
        row.append(ptr->output_taktTime);//节拍

        outdata[ptr->id].append( row );
    }

    QVariantList row;
    row.append( date );
    row.append(indata->error_count);//
    row.append(indata->productChangeCount);//
    row.append(indata->power_consumption);//功耗

    outdata[D_PdLinePublicDataKey].append( row );

    return true;
}

//static QHash<QString, QString> productIdMapName;
//static void initProductIdMapName(){
////    productIdMapName.insert(D_PdLinePublicDataKey, "功耗");
//}

QString changeDateFormatShow(const QString &from, ENUM_DATEFORMAT_SHOW toF)
{
    QString tmp ;
    switch(from.length()){
    case 8://D_DATEFORMAT_TIME_SQL     QStringLiteral("HH:mm:ss")
    {
        switch(toF){
        case D_DATEFORMAT_SECOND_SHOW:
            return from;
            break;//HH:mm:ss
        case D_DATEFORMAT_HOUR_SHOW:
            tmp= from.mid(0,2);
            break;//HH
            //不支持
            //        case D_DATEFORMAT_DAY_SHOW:
            //            tmp= from.mid(5,5);
            //            break;//MM-dd
            //        case D_DATEFORMAT_MONTH_SHOW:
            //            tmp= from.mid(0,7);
            //            break;//yyyy-MM
            //        case D_DATEFORMAT_YEAR_SHOW:
            //            tmp= from.mid(0,4);
            //            break;//yyyy
        default:
            return from;
        }
    }
        break;

    case 19://D_DATEFORMAT_SECOND_SQL  QStringLiteral("yyyy-MM-dd HH:mm:ss")
    {
        switch(toF){
        case D_DATEFORMAT_SECOND_SHOW:
            tmp= from.mid(11);
            break;//HH:mm:ss
        case D_DATEFORMAT_HOUR_SHOW:
            tmp= from.mid(11,2);
            break;//HH
        case D_DATEFORMAT_DAY_SHOW:
            tmp= from.mid(5,5);
            break;//MM-dd
        case D_DATEFORMAT_MONTH_SHOW:
            tmp= from.mid(0,7);
            break;//yyyy-MM
        case D_DATEFORMAT_YEAR_SHOW:
            tmp= from.mid(0,4);
            break;//yyyy
        default:
            return from;
        }
    }
        break;

    case 10://D_DATEFORMAT_DAY_SQL     QStringLiteral("yyyy-MM-dd")
    {
        switch(toF){
        //        case D_DATEFORMAT_SECOND_SHOW:
        //            tmp= from.mid(11);
        //            break;//HH:mm:ss
        //        case D_DATEFORMAT_HOUR_SHOW:
        //            tmp= from.mid(11,2);
        //            break;//HH
        case D_DATEFORMAT_DAY_SHOW:
            tmp= from.mid(5,5);
            break;//MM-dd
        case D_DATEFORMAT_MONTH_SHOW:
            tmp= from.mid(0,7);
            break;//yyyy-MM
        case D_DATEFORMAT_YEAR_SHOW:
            tmp= from.mid(0,4);
            break;//yyyy
        default:
            return from;
        }
    }
        break;

    case 7://D_DATEFORMAT_MONTH_SQL   QStringLiteral("yyyy-MM")
    {
        switch(toF){
        //        case D_DATEFORMAT_SECOND_SHOW:
        //            tmp= from.mid(11);
        //            break;//HH:mm:ss
        //        case D_DATEFORMAT_HOUR_SHOW:
        //            tmp= from.mid(11,2);
        //            break;//HH
        //        case D_DATEFORMAT_DAY_SHOW:
        //            tmp= from.mid(5,5);
        //            break;//MM-dd
        case D_DATEFORMAT_MONTH_SHOW:
            tmp= from.mid(0,7);
            break;//yyyy-MM
        case D_DATEFORMAT_YEAR_SHOW:
            tmp= from.mid(0,4);
            break;//yyyy
        default:
            return from;
        }
    }
        break;

    case 4://D_DATEFORMAT_YEAR_SQL    QStringLiteral("yyyy")
    {
        switch(toF){
        //        case D_DATEFORMAT_SECOND_SHOW:
        //            tmp= from.mid(11);
        //            break;//HH:mm:ss
        //        case D_DATEFORMAT_HOUR_SHOW:
        //            tmp= from.mid(11,2);
        //            break;//HH
        //        case D_DATEFORMAT_DAY_SHOW:
        //            tmp= from.mid(5,5);
        //            break;//MM-dd
        case D_DATEFORMAT_MONTH_SHOW:
            tmp= from.mid(0,7);
            break;//yyyy-MM
        case D_DATEFORMAT_YEAR_SHOW:
            tmp= from.mid(0,4);
            break;//yyyy
        default:
            return from;
        }
    }
        break;

    default:
        return from;
        break;
    }

    return tmp;
}

bool TableDataSubToJson( TableDataSub &dat, QJsonObject& js, ENUM_DATEFORMAT_SHOW xDataFormat, bool sortXData){
    QVariantMap jsMap;
    QSet<QString > xDataSet;//解决时间轴问题

    QMap<QString, QList< QVariantList > >::iterator imap;
    for (imap = dat.begin(); imap != dat.end(); ++imap) {

        const QString& key = imap.key();
        QList< QVariantList >& val = imap.value();

        if(val.size()<1)
            continue;

        QVariantList vlist1; //一级
        QList< QVariantList >::iterator ilist;

        for (ilist = val.begin(); ilist != val.end(); ++ilist) {

            //            qDebug()<< "*ilist========="<<(*ilist).size()<< (*ilist) ;
            //第一个是时间
            QVariant qvar =(*ilist).at(0);
            QString time=qvar.toString();

            //            qDebug()<<"time===="<<time << xDataFormat << sortXData;
            if(xDataFormat){
                time=changeDateFormatShow(time,xDataFormat);
                (*ilist).replace(0,QVariant(time));
            }
            //            qDebug()<<"time22===="<<time ;
            if(sortXData){
                xDataSet.insert(time);
            }

            vlist1.append( QVariant(*ilist) );
        }
        jsMap.insert( key,vlist1 );
    }

    QJsonArray jsArry;
    if(sortXData){
        QStringList tmList= xDataSet.toList();
        qSort(tmList.begin(), tmList.end());
        jsArry = QJsonArray::fromStringList(tmList);
    }

    js["yData"]=QJsonObject::fromVariantMap(jsMap);
    js["xData"]=jsArry;

#ifdef D_TEST
//    qDebug()<< "sortXData === "<< sortXData;
    //    qDebug()<< "jsMap === "<< jsMap;
//    qDebug()<< "jsArry === "<< jsArry;
//    qDebug()<< "jsdoc === "<< js;
#endif

    return true;
}

//static QString getRandomString(int length)
//{
//    qsrand(QDateTime::currentMSecsSinceEpoch());//为随机值设定一个seed

//    const char chrs[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
//    int chrs_size = sizeof(chrs);

//    char* ch = new char[length + 1];
//    memset(ch, 0, length + 1);
//    int randomx = 0;
//    for (int i = 0; i < length; ++i)
//    {
//        randomx= rand() % (chrs_size - 1);
//        ch[i] = chrs[randomx];
//    }

//    QString ret(ch);
//    delete[] ch;
//    return ret;
//}


void creatTable(const QString& table,const QStringList& args){
    QString SQL_createTable=QString(
                "CREATE TABLE IF NOT EXISTS %1("
                ).arg(table);
    for(int i=0;i<args.size();i++){
        SQL_createTable.append(args.at(i));
        if(i<args.size()-1)
            SQL_createTable.append(",");
        else
            SQL_createTable.append(")");
    }
#ifdef D_USE_MYSQL
    static XDatabase::Control control( { "QMYSQL", QTime::currentTime().toString("hh:mm:ss.zzz"), "localhost", D_DB_FILE_A, "root", "" } );
#else
    static XDatabase::Control control(
    { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A },
    { 3200, XDatabase::QueryAutoMode, D_DB_WAITTIME}
                );
#endif
    auto query(control.query());

    if(!query->exec(SQL_createTable))
    {
        qDebug() << "lastError==" << __FUNCTION__ <<__LINE__ << query->lastError();
    }
    query->clear();

//#ifdef D_TEST
//    qDebug()<<__FUNCTION__  << "lastQuery==" << query->last() << query->lastQuery();
//#endif

}

void creatTableWhenStart(){
    //sqlite 中,replace 无法跟where联合使用,为了使用replace,使用了time作为唯一的主键,就可以覆盖某条数据

    //杂项
    creatTable(D_SqlTableKeyVal ,
               QStringList()
               <<"tindex  INTEGER NOT NULL"
               <<"time  DATETIME"
               <<"key   CHAR(50)"
               <<"val   BLOB"
               <<"PRIMARY KEY (tindex,key)"
               );

    //log
    creatTable(D_SqlTableLog ,
               QStringList()
               <<"tindex  INTEGER NOT NULL"
               <<"type  INT"
               <<"time  DATETIME"
               <<"context  CHAR(64)"
               <<"msg   BLOB"
               <<"PRIMARY KEY (tindex,type)"
               );

    //维护
    creatTable(D_SqlTableReplacement ,
               QStringList()
               <<"tindex  INTEGER NOT NULL"
               <<"count   INT"
               <<"timeSet DATETIME"
               <<"timeOP  DATETIME"
               <<"name    CHAR(32)"
               <<"note    CHAR(256)"
               <<"PRIMARY KEY (tindex)"
               );

    //保养
    creatTable(D_SqlTableMaintenance ,
               QStringList()
               <<"tindex  INTEGER NOT NULL"
               <<"count   INT"
               <<"timeSet DATETIME"
               <<"timeOP  DATETIME"
               <<"name    CHAR(32)"
               <<"note    CHAR(256)"
               <<"PRIMARY KEY (tindex)"
               );

    //错误发生
    creatTable(D_SqlTableErrorHappened ,
               QStringList()
               <<"tindex  INTEGER NOT NULL"
               <<"time    DATETIME"
               <<"code    INT"
               <<"msg     CHAR(512)"
               <<"PRIMARY KEY (tindex)"
               );

    //产品切换
    creatTable(D_SqlTableProductIdChange ,
               QStringList()
               <<"tindex  INTEGER NOT NULL"
               <<"time    DATETIME"
               <<"curid   CHAR(50)"
               <<"PRIMARY KEY (tindex)"
               );
    //产能表
    creatTable(D_SqlTablePdOutput ,
               QStringList()
               <<"tindex  INTEGER NOT NULL"
               <<"time    DATETIME"
               <<"pdid    CHAR(50)"
               <<"code    INT"
               <<"result  INT"
               <<"data    BLOB"
               <<"PRIMARY KEY (tindex)"
               );

    //功耗表(滚动表) data1=power
    creatTable(D_SqlTableProductLine ,
               QStringList()
               <<"tindex  INTEGER NOT NULL"
               <<"time    DATETIME"
               <<"data1   float"
               <<"data2   float"
               <<"data3   float"
               <<"data4   int"
               <<"data5   int"
               <<"data6   int"
               <<"PRIMARY KEY (tindex)"
               );
}

void creatTableWhenRolling(const QString& table)//这个表一直根据时间创建
{
    //
}

void insertPdOutput(const QString& table, const QDateTime &date, const QString &pdid, int code, int result, const QByteArray &data)
{
#ifdef D_TEST
    QDateTime T1= QDateTime::currentDateTime();
#endif

#ifdef D_USE_MYSQL
    static XDatabase::Control control( { "QMYSQL", QTime::currentTime().toString("hh:mm:ss.zzz"), "localhost", D_DB_FILE_A, "root", "" } );
#else
    static XDatabase::Control control(
    { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A },
    { 3200, XDatabase::QueryAutoMode, D_DB_WAITTIME}
                );
#endif
    auto query(control.query());

    //产量数据
    QString SQL_Replace_PdOutput_N=QString(
                "INSERT INTO %1 (tindex,time,pdid,code,result,data)"
                " VALUES( ?, ?, ?, ?, ?, ? )"
                ).arg(table);

    int tindex = date.toSecsSinceEpoch();
    QString time = date.toString(D_DATEFORMAT_SECOND_SQL);

    //=================
    query->prepare( SQL_Replace_PdOutput_N );
    query->bindValue(0, tindex );//日期
    query->bindValue(1, time );
    query->bindValue(2, pdid );
    query->bindValue(3, code );
    query->bindValue(4, result?1:0 );
    query->bindValue(5, data  );

    if(!query->exec())
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }
    query->clear();

#ifdef D_TEST
    qDebug()<< __FUNCTION__ << "耗时test======" << T1 .msecsTo( QDateTime::currentDateTime() );
#endif
}

void insertWhenRolling(const QString& table, const QDateTime& date, S_ProductLineData* pdat)
{
    //    qDebug()<<__FUNCTION__ <<table << date;
#ifdef D_TEST
    QDateTime T1= QDateTime::currentDateTime();
#endif

#ifdef D_USE_MYSQL
    static XDatabase::Control control( { "QMYSQL", QTime::currentTime().toString("hh:mm:ss.zzz"), "localhost", D_DB_FILE_A, "root", "" } );
#else
    static XDatabase::Control control(
    { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A },
    { 3000, XDatabase::QueryAutoMode, D_DB_WAITTIME}
                );
#endif
    auto query(control.query());

    //其他数据
    QString SQL_Replace_ProductLine_N=QString(
                "REPLACE INTO  %1  (tindex,time,data1,data2,data3,data4,data5,data6)"
                " VALUES( ?, ?, ?, ?, ?, ?, ?, ? )"
                ).arg(table);

    int tindex = date.toSecsSinceEpoch();
    QString time = date.toString(D_DATEFORMAT_SECOND_SQL);

    //=================
    query->prepare( SQL_Replace_ProductLine_N );
    query->bindValue(0, tindex );//日期
    query->bindValue(1, time );
    query->bindValue(2, pdat->power_consumption  );
    query->bindValue(3, 0  );
    query->bindValue(4, 0  );
    query->bindValue(5, 0  );
    query->bindValue(6, 0  );
    query->bindValue(7, 0  );

    if(!query->exec())
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }
    query->clear();

#ifdef D_TEST
    qDebug()<< __FUNCTION__ << "耗时test======" << T1 .msecsTo( QDateTime::currentDateTime() );
#endif
}

void insertTable_ProductLine(int tindex, const QString& time,
                             float data1,float data2,float data3,  int data4,int data5,int data6 )
{
#ifdef D_USE_MYSQL
    static XDatabase::Control control( { "QMYSQL", QTime::currentTime().toString("hh:mm:ss.zzz"), "localhost", D_DB_FILE_A, "root", "" } );
#else
    static XDatabase::Control control(
    { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A },
    { 3000, XDatabase::QueryAutoMode, D_DB_WAITTIME}
                );
#endif
    auto query(control.query());

    //产品数据
    QString sqlcmd=QString(
                "INSERT INTO %1 (tindex,time,code,curid)"
                " VALUES( ?,?, "
                "?, ?, ?, ?, ?, ? )"
                ).arg(D_SqlTableProductLine);

    query->prepare( sqlcmd );
    query->bindValue(0, tindex );//日期
    query->bindValue(1, time );

    query->bindValue(2, data1 );
    query->bindValue(3, data2 );
    query->bindValue(4, data3 );
    query->bindValue(5, data4 );
    query->bindValue(6, data5 );
    query->bindValue(7, data6 );

    if(!query->exec())
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }
    query->clear();
}

int selectPdOutputSum(QSqlQuery *query, ENUM_SHOWDATE_TYPE type,
                      const QString &table, quint64 start, quint64 end, TableDataSub &datSub)
{
    quint64 startIndex = start;
    quint64 endIndex = end;

    //可用于时间段计算
    QString sqlcmd_ok = QString(
                "SELECT pdid,count(*) FROM ("
                "SELECT pdid,result FROM %1 WHERE tindex>=%2 AND tindex<%3"
                ") WHERE result>0 GROUP BY pdid"
                ).arg( table ).arg( startIndex ).arg( endIndex );

    QString sqlcmd_ng = QString(
                "SELECT pdid,count(*) FROM ("
                "SELECT pdid,result FROM %1 WHERE tindex>=%2 AND tindex<%3"
                ")  WHERE result<1 GROUP BY pdid"
                ).arg( table ).arg( startIndex ).arg( endIndex );

    QString time = QDateTime::fromSecsSinceEpoch( startIndex ).toString(D_DATEFORMAT_SECOND_SQL);//后期显示前会剪裁短一点

    S_ProductLineData datSrc;
    //    datSrc.date=time;

    //ok
    if(!query->exec(sqlcmd_ok))
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }else{

#ifdef D_TEST
//        qDebug()<<"ok query=="<<query->size() <<time <<  startIndex << endIndex;
#endif

        while(query->next())
        {
            QString pdid =  query->value(0).toString();
            int result =  query->value(1).toInt();

//            qDebug()<<"result=="<<pdid<<result;

            S_PdOutputData* ptr = datSrc.product.value(pdid);
            if(!ptr){
                ptr = new S_PdOutputData();
                ptr->id=pdid;
                datSrc.product.insert(pdid,ptr);
            }
            ptr->output += result;
            ptr->output_ok += result;
        }
    }
    query->clear();

    //ng
    if(!query->exec(sqlcmd_ng))
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }else{

        //        qDebug()<<"ng query=="<<query->size() << startIndex << endIndex;

        while(query->next())
        {
            QString pdid =  query->value(0).toString();
            int result =  query->value(1).toInt();

            S_PdOutputData* ptr = datSrc.product.value(pdid);
            if(!ptr){
                ptr = new S_PdOutputData();
                ptr->id=pdid;
                datSrc.product.insert(pdid,ptr);
            }
            ptr->output += result;
            ptr->output_ng += result;
        }
    }
    query->clear();

    foreach( S_PdOutputData*ptr ,datSrc.product ){
        //良品率
        if(ptr->output==0){
            ptr->output_ratio = 0.0;
            ptr->output_taktTime = 0.0;
        }else{
            ptr->output_ratio = ptr->output_ok/ptr->output;  //节拍 //TODO大跨度计算节拍没有意义
            switch(type){
            case E_YEAR:
            {
                //使用365进行粗略计算
                ptr->output_taktTime = (float)(3600.0*24*365/(float)ptr->output);
            }
                break;
            case E_MONTH:
            {
                //使用30进行粗略计算
                ptr->output_taktTime = (float)(3600.0*24*30/(float)ptr->output);
            }
                break;
            case E_DAY:
            {
                ptr->output_taktTime = (float)(3600.0*24/(float)ptr->output);
            }
                break;
            case E_HOUR:
            {
                ptr->output_taktTime = (float)(3600.0/(float)ptr->output);
            }
                break;
            }
        }

        //        qDebug()<<"rowtime=="
        //               <<time
        //              <<ptr->id
        //             <<ptr->output
        //            <<ptr->output_ok
        //           <<ptr->output_ng
        //          <<ptr->output_ratio
        //         <<ptr->output_taktTime;

        QVariantList row;
        row.append( time );
        row.append( ptr->id );
        row.append( ptr->output );
        row.append( ptr->output_ok );
        row.append( ptr->output_ng );
        row.append( ptr->output_ratio );
        row.append( ptr->output_taktTime );

        datSub[ptr->id].append(row);
    }

    return 0;
}

//统计
int selectPowerSum(QSqlQuery *query, ENUM_SHOWDATE_TYPE type,
                   const QString &table, quint64 start, quint64 end, TableDataSub &datSub)
{
    quint64 startIndex = start;
    quint64 endIndex = end;
    QString time = QDateTime::fromSecsSinceEpoch( startIndex ).toString(D_DATEFORMAT_SECOND_SQL);//后期显示前会剪裁短一点

    QString sqlcmd_power = QString(
                "SELECT sum(data1) FROM %1 WHERE tindex>=%2 AND tindex<%3"
                ).arg( table ).arg( startIndex ).arg( endIndex );

    //功耗
    float power  = 0.0;
    if(!query->exec(sqlcmd_power))
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }else{
        while(query->next())
        {//理论上只有一条
            power  =  query->value(0).toInt();
        }
    }
    query->clear();

    QString sqlcmd_error = QString(
                "SELECT count(*) FROM %1 WHERE tindex>=%2 AND tindex<%3"
                ).arg( D_SqlTableErrorHappened ).arg( startIndex ).arg( endIndex );

    QString sqlcmd_idc = QString(
                "SELECT count(*) FROM %1 WHERE tindex>=%2 AND tindex<%3"
                ).arg( D_SqlTableProductIdChange ).arg( startIndex ).arg( endIndex );

    //错误发生
    int errorCount = 0;
    if(!query->exec(sqlcmd_error))
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }else{
        if( query->size()>0 ){
            errorCount = query->size();
        }
    }
    query->clear();

    //产品切换
    int idChangeCount = 0;
    if(!query->exec(sqlcmd_idc))
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }else{
        if( query->size()>0 ){
            idChangeCount = query->size();
        }
    }
    query->clear();

    QVariantList row;
    row.append( time );
    row.append( errorCount );
    row.append( idChangeCount );
    row.append( power );
    datSub[ D_PdLinePublicDataKey ].append(row);

    return 0;
}

//细分
int selectPdOutput(QSqlQuery *query, ENUM_SHOWDATE_TYPE type, quint64 start, quint64 end, TableDataSub &datSub)
{
    //TODO.. //1.优化统计计算方式,用C++减少查询时间//2.分表,减少查询时间

    QString table(D_SqlTablePdOutput);

    switch(type){
    case E_YEAR:
    {
        QDateTime startTime = QDateTime::fromSecsSinceEpoch(start);
        QDateTime tmpST=startTime;
        QDateTime tmpEND=startTime.addMonths(12);

        quint64 startIndex= tmpST.toSecsSinceEpoch();
        quint64 endIndex  = tmpEND.toSecsSinceEpoch();
        while( startIndex < end ){
            //            qDebug()<<__FUNCTION__<<"显示年图" <<  tmpST.toString(Qt::ISODate) << tmpEND.toString(D_DATEFORMAT_SECOND_SQL);

            selectPdOutputSum( query, E_YEAR, table, startIndex, endIndex+1, datSub );
            if(endIndex>=end){
                break;
            }

            tmpST=tmpST.addMonths(12);
            tmpEND=tmpEND.addMonths(12);
            startIndex= tmpST.toSecsSinceEpoch();
            endIndex  = tmpEND.toSecsSinceEpoch();
        };
    }
        break;
    case E_MONTH:
    {
        QDateTime startTime = QDateTime::fromSecsSinceEpoch(start);
        QDateTime tmpST=startTime;
        QDateTime tmpEND=startTime.addMonths(1);

        quint64 startIndex= tmpST.toSecsSinceEpoch();
        quint64 endIndex  = tmpEND.toSecsSinceEpoch();
        while( startIndex < end ){
            //            qDebug()<<__FUNCTION__<<"显示月图" <<  tmpST.toString(Qt::ISODate) << tmpEND.toString(D_DATEFORMAT_SECOND_SQL);

            selectPdOutputSum( query, E_MONTH, table, startIndex, endIndex+1, datSub );
            if(endIndex>=end){
                break;
            }

            tmpST=tmpST.addMonths(1);
            tmpEND=tmpEND.addMonths(1);
            startIndex= tmpST.toSecsSinceEpoch();
            endIndex  = tmpEND.toSecsSinceEpoch();
        };
    }
        break;
    case E_DAY:
    {
        quint64 startIndex= start;
        quint64 endIndex  = startIndex+3600*24;
        while( startIndex < end ){
            //            qDebug()<<"显示日图=="<<startIndex<< QDateTime::fromSecsSinceEpoch( startIndex );

            selectPdOutputSum( query, E_DAY, table, startIndex, endIndex+1, datSub );
            if(endIndex>=end){
                break;
            }

            startIndex += 3600*24;
            endIndex += 3600*24;
        };
    }
        break;
    case E_HOUR:
    {
        quint64 startIndex= start;
        quint64 endIndex  = startIndex+3600;
        while( startIndex < end ){
            //          qDebug()<<"显示小时图=="<<startIndex<< QDateTime::fromSecsSinceEpoch( startIndex );

            selectPdOutputSum( query, E_HOUR, table, startIndex, endIndex+1, datSub );
            if(endIndex>=end){
                break;
            }

            startIndex += 3600;
            endIndex += 3600;
        };
    }
        break;
    }

    return 0;
}

//细分
int selectPower(QSqlQuery *query, ENUM_SHOWDATE_TYPE type, quint64 start, quint64 end, TableDataSub &datSub)
{
    //TODO.. //1.优化统计计算方式,用C++减少查询时间//2.分表,减少查询时间

    QString table(D_SqlTableProductLine);

    switch(type){
    case E_YEAR:
    {
        //        qDebug()<<__FUNCTION__<<"显示年图" <<  start.toString(Qt::ISODate) << end.toString(D_DATEFORMAT_SECOND_SQL);

        QDateTime startTime = QDateTime::fromSecsSinceEpoch(start);
        QDateTime tmpST=startTime;
        QDateTime tmpEND=startTime.addMonths(12);

        quint64 startIndex= tmpST.toSecsSinceEpoch();
        quint64 endIndex  = tmpEND.toSecsSinceEpoch();
        while( startIndex < end ){
            selectPowerSum( query, E_YEAR, table, startIndex, endIndex+1, datSub );
            if(endIndex>=end){
                break;
            }

            tmpST=tmpST.addMonths(12);
            tmpEND=tmpEND.addMonths(12);
            startIndex= tmpST.toSecsSinceEpoch();
            endIndex  = tmpEND.toSecsSinceEpoch();
        };
    }
        break;
    case E_MONTH:
    {
        //        qDebug()<<__FUNCTION__<<"显示月图" <<  start.toString(Qt::ISODate) << end.toString(D_DATEFORMAT_SECOND_SQL);

        QDateTime startTime = QDateTime::fromSecsSinceEpoch(start);
        QDateTime tmpST=startTime;
        QDateTime tmpEND=startTime.addMonths(1);

        quint64 startIndex= tmpST.toSecsSinceEpoch();
        quint64 endIndex  = tmpEND.toSecsSinceEpoch();
        while( startIndex < end ){
            selectPowerSum( query, E_MONTH, table, startIndex, endIndex+1, datSub );
            if(endIndex>=end){
                break;
            }

            tmpST=tmpST.addMonths(1);
            tmpEND=tmpEND.addMonths(1);
            startIndex= tmpST.toSecsSinceEpoch();
            endIndex  = tmpEND.toSecsSinceEpoch();
        };
    }
        break;
    case E_DAY:
    {
        //        qDebug()<<__FUNCTION__<<"显示日图" <<  start.toString(Qt::ISODate) << end.toString(D_DATEFORMAT_SECOND_SQL);

        quint64 startIndex= start;
        quint64 endIndex  = startIndex+3600*24;
        while( startIndex < end ){
            selectPowerSum( query, E_DAY, table, startIndex, endIndex+1, datSub );
            if(endIndex>=end){
                break;
            }

            startIndex += 3600*24;
            endIndex += 3600*24;
        };
    }
        break;
    case E_HOUR:
    {
        quint64 startIndex= start;
        quint64 endIndex  = startIndex+3600;
        while( startIndex < end ){
            selectPowerSum( query, E_HOUR, table, startIndex, endIndex+1, datSub );
            if(endIndex>=end){
                break;
            }

            startIndex += 3600;
            endIndex += 3600;
        };
    }
        break;
    }

    return 0;
}

//所有符合条件的条目
int selectDetails(QSqlQuery *query, const QString &table, quint64 startIndex, quint64 endIndex, QList< QVector<QVariant> > &dat, const QString &selectValues)
{
    QString sqlcmd = QString( "SELECT %0 FROM %1 WHERE tindex>=%2 AND tindex<%3" ).arg( selectValues ).arg( table ).arg( startIndex ).arg( endIndex );

    if(!query->exec(sqlcmd))
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }else{

        QVector<QVariant> row;
        while(query->next())
        {
            QSqlRecord& record = query->record();
            int count = record.count();
            if(row.size() != count){
                row.resize( count );
            }
            for(int i=0;i<count;i++){
                row[i]= record.value(i);
            }
            dat.append(row);
        }
    }

#ifdef D_TEST
    //query->size();//这个不准的
     qDebug()<< "selectDetails count==="<< dat.count();
#endif

    query->clear();

    return 0;
}

//显示日表
int selectDay(QSqlQuery *query, const QDate&dt, TableDataSub &dat){

    QDate start = dt ;
    QDate end = start.addDays(1);

//    qDebug()<<__FUNCTION__<<"==="<< start<<end ;
    return selectWithTime(query, QDateTime(start), QDateTime(end), dat );
}

//显示月表
int selectMonth(QSqlQuery *query, const QDate& dt, TableDataSub &dat){

    QDate start = dt ;
    bool ok = start.setDate(dt.year(),dt.month(),1) ;
    QDate end = start.addMonths(1);

//    qDebug()<<__FUNCTION__<<"==="<< start<<end<<ok ;
    return selectWithTime(query, QDateTime(start), QDateTime(end), dat );
}

//显示年表
int selectYear(QSqlQuery *query, const QDate& dt, TableDataSub &dat){

    QDate start = dt ;
    bool ok = start.setDate(dt.year(),1,1) ;
    QDate end = start.addMonths(12);

//    qDebug()<<__FUNCTION__<<"==="<< start<<end<<ok ;
    return selectWithTime(query, QDateTime(start), QDateTime(end), dat );
}

//按时间查 可跨表
int selectWithTime(QSqlQuery *query, const QDateTime &start, const QDateTime &end, TableDataSub &dat)
{
//    qDebug()<< __FUNCTION__<< start.date() << end.date();

    if( start >= end ){
        return -1;//error
    }

    quint64 startIndex=start.toSecsSinceEpoch();
    quint64 endIndex=end.toSecsSinceEpoch();

    if(start.addMonths(12)<end){
//        qDebug()<<__FUNCTION__<<"超过一年的数据,只显示年图" <<  start.toString(Qt::ISODate) << end.toString(D_DATEFORMAT_SECOND_SQL);

        selectPdOutput( query, E_YEAR, startIndex, endIndex, dat );
        selectPower( query, E_YEAR, startIndex, endIndex, dat );

    }else  if(start.addMonths(1)<end){
//        qDebug()<<__FUNCTION__<<"超过一个月的数据,只显示月图" <<  start.toString(Qt::ISODate) << end.toString(D_DATEFORMAT_SECOND_SQL);

        selectPdOutput( query, E_MONTH, startIndex, endIndex, dat );
        selectPower( query, E_MONTH, startIndex, endIndex, dat );

    }else  if(start.addDays(1)<end){
//        qDebug()<<__FUNCTION__<<"超过一天的数据,只显示日图" <<  start.toString(Qt::ISODate) << end.toString(D_DATEFORMAT_SECOND_SQL);

        selectPdOutput( query, E_DAY, startIndex, endIndex, dat );
        selectPower( query, E_DAY, startIndex, endIndex, dat );
    }else{

//        qDebug()<<__FUNCTION__<<"一天内的数据,显示时图" <<  start.toString(Qt::ISODate) << end.toString(D_DATEFORMAT_SECOND_SQL);

        selectPdOutput( query, E_HOUR, startIndex, endIndex, dat );
        selectPower( query, E_HOUR, startIndex, endIndex, dat );
    }

    return 0;
}

int selectWithTimeSum(QSqlQuery *query, const QDateTime &start, const QDateTime &end, TableDataSub &dat)
{
    if( start >= end ){
        return -1;//error
    }

    quint64 startIndex=start.toSecsSinceEpoch();
    quint64 endIndex=end.toSecsSinceEpoch();

    if(start.addMonths(12)<end){

        qDebug()<<__FUNCTION__<<"超过一年的数据,只显示年图" <<  start.toString(Qt::ISODate) << end.toString(D_DATEFORMAT_SECOND_SQL);

        QString tablePdOutput(D_SqlTablePdOutput);
        QString tablePower(D_SqlTableProductLine);
        selectPdOutputSum( query, E_MONTH, tablePdOutput, startIndex, endIndex, dat );
        selectPowerSum( query, E_MONTH, tablePower, startIndex,  endIndex, dat );

    }else  if(start.addMonths(1)<end){
        qDebug()<<__FUNCTION__<<"超过一个月的数据,只显示月图" <<  start.toString(Qt::ISODate) << end.toString(D_DATEFORMAT_SECOND_SQL);

        QString tablePdOutput(D_SqlTablePdOutput);
        QString tablePower(D_SqlTableProductLine);
        selectPdOutputSum( query, E_MONTH, tablePdOutput, startIndex, endIndex, dat );
        selectPowerSum( query, E_MONTH, tablePower, startIndex,  endIndex, dat );

    }else  if(start.addDays(1)<end){
        qDebug()<<__FUNCTION__<<"超过一天的数据,只显示日图" <<  start.toString(Qt::ISODate) << end.toString(D_DATEFORMAT_SECOND_SQL);

        QString tablePdOutput(D_SqlTablePdOutput);
        QString tablePower(D_SqlTableProductLine);
        selectPdOutputSum( query, E_DAY, tablePdOutput, startIndex, endIndex, dat );
        selectPowerSum( query, E_DAY, tablePower, startIndex,  endIndex, dat );
    }else{

        qDebug()<<__FUNCTION__<<"一天内的数据,显示时图" <<  start.toString(Qt::ISODate) << end.toString(D_DATEFORMAT_SECOND_SQL);

        QString tablePdOutput(D_SqlTablePdOutput);
        QString tablePower(D_SqlTableProductLine);
        selectPdOutputSum( query, E_HOUR, tablePdOutput, startIndex, endIndex, dat );
        selectPowerSum( query, E_HOUR, tablePower, startIndex,  endIndex, dat );
    }

    return 0;
}

void insertReplacement(int count, const QString &timeSet, const QString &name, const QString &note)
{
    QString SQL_cmd=QString(
                "REPLACE INTO  %1 (count,timeSet,timeOP,name,note) "
                " VALUES(?, ?, ?, ?, ?)"
                ).arg(D_SqlTableReplacement);

#ifdef D_USE_MYSQL
    static XDatabase::Control control( { "QMYSQL", QTime::currentTime().toString("hh:mm:ss.zzz"), "localhost", D_DB_FILE_A, "root", "" } );
#else
    static XDatabase::Control control(
    { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A },
    { 3200, XDatabase::QueryAutoMode, D_DB_WAITTIME}
                );
#endif
    auto query(control.query());

    query->prepare(SQL_cmd);
    query->bindValue( 0,count );
    query->bindValue( 1,timeSet );
    query->bindValue( 2,QDateTime::currentDateTime().toString(D_DATEFORMAT_SECOND_SQL) );
    query->bindValue( 3,name );
    query->bindValue( 4,note );

    if(!query->exec())
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }
    query->clear();
}

void insertMaintenance(int count, const QString &timeSet, const QString &name, const QString &note)
{
    QString SQL_cmd=QString(
                "REPLACE INTO  %1 (count,timeSet,timeOP,name,note) "
                " VALUES(?, ?, ?, ?, ?)"
                ).arg(D_SqlTableMaintenance);
#ifdef D_USE_MYSQL
    static XDatabase::Control control( { "QMYSQL", QTime::currentTime().toString("hh:mm:ss.zzz"), "localhost", D_DB_FILE_A, "root", "" } );
#else
    static XDatabase::Control control(
    { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A },
    { 3200, XDatabase::QueryAutoMode, D_DB_WAITTIME}
                );
#endif
    auto query(control.query());

    query->prepare(SQL_cmd);
    query->bindValue( 0,count );
    query->bindValue( 1,timeSet );
    query->bindValue( 2,QDateTime::currentDateTime().toString(D_DATEFORMAT_SECOND_SQL) );
    query->bindValue( 3,name );
    query->bindValue( 4,note );

    if(!query->exec())
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }
    query->clear();

}

void insertProductIdChanged(const QDateTime &date, const QString &curid)
{
    QString sqlcmd=QString(
                "INSERT INTO %1 (tindex,time,curid)"
                " VALUES( ?, ?, ? )"
                ).arg(D_SqlTableProductIdChange);
#ifdef D_USE_MYSQL
    static XDatabase::Control control( { "QMYSQL", QTime::currentTime().toString("hh:mm:ss.zzz"), "localhost", D_DB_FILE_A, "root", "" } );
#else
    static XDatabase::Control control(
    { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A },
    { 3200, XDatabase::QueryAutoMode, D_DB_WAITTIME}
                );
#endif
    auto query(control.query());

    quint64 tindex=date.toSecsSinceEpoch();
    QString time = date.toString(D_DATEFORMAT_SECOND_SQL);

    query->prepare( sqlcmd );
    query->bindValue(0, tindex );
    query->bindValue(1, time );
    query->bindValue(2, curid );

    if(!query->exec())
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }
    query->clear();
}

void insertErrorHappened(const QDateTime &date, int code, const QString &msg)
{
    QString sqlcmd=QString(
                "INSERT INTO %1 (tindex,time,code,msg)"
                " VALUES( ?, ?, ?, ? )"
                ).arg(D_SqlTableErrorHappened);
#ifdef D_USE_MYSQL
    static XDatabase::Control control( { "QMYSQL", QTime::currentTime().toString("hh:mm:ss.zzz"), "localhost", D_DB_FILE_A, "root", "" } );
#else
    static XDatabase::Control control(
    { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A },
    { 3200, XDatabase::QueryAutoMode, D_DB_WAITTIME}
                );
#endif
    auto query(control.query());

    quint64 tindex=date.toSecsSinceEpoch();
    QString time = date.toString(D_DATEFORMAT_SECOND_SQL);

    query->prepare( sqlcmd );
    query->bindValue(0, tindex );
    query->bindValue(1, time );
    query->bindValue(2, code );
    query->bindValue(3, msg );

    if(!query->exec())
    {
        qDebug() << "Error" << __FUNCTION__ <<__LINE__ << query->lastError() << query->lastQuery() ;
    }
    query->clear();

}

