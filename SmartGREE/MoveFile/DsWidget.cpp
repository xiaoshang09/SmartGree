#include "DsWidget.h"
#include "ui_DsWidget.h"

#include <QApplication>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "MyWebView.h"

//#define m_web (MyWebView*)m_web

DsWidget::DsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DsWidget)
{
    ui->setupUi(this);

    m_web = new MyWebView();

    ui->gridLayout_2->addWidget(m_web, 0, 0, 2, 1);


    connect( m_web , SIGNAL(loadFinished(bool)), this , SLOT(loadFinished()));


    btGroup.addButton( ui->radioButton_year, E_YEAR );
    btGroup.addButton( ui->radioButton_month, E_MONTH );
    btGroup.addButton( ui->radioButton_day, E_DAY );

    connect( &btGroup, SIGNAL(buttonToggled(int, bool)), this , SLOT(buttonToggled(int, bool)) );
    m_id = btGroup.checkedId();
    m_date = ui->calendarWidget->selectedDate();

#if 1
    QString file = "dsData.html";
    m_web->setUrl( QUrl::fromLocalFile( qApp->applicationDirPath()+"/"+ file ) );
    qDebug()<<"file=="<< QUrl::fromLocalFile( qApp->applicationDirPath()+"/"+ file ) ;
#else
    m_web->setUrl(QUrl("qrc:/web/dsData.html"));
    qDebug()<<"file=="<< QUrl("qrc:/web/dsData.html");
#endif
}

DsWidget::~DsWidget()
{
    delete ui;
}


//void DsWidget::resizeEvent(QResizeEvent *event)
//{
//    QWidget::resizeEvent(event);
//    ui->widget->move( this->width()-ui->widget->width(),this->height()-ui->widget->height());
//    m_web->setGeometry(0,0,this->width(),this->height());
//}

void DsWidget::buttonToggled(int id, bool check)
{

    qDebug()<<__FUNCTION__<< id << check;
    switch( btGroup.checkedId() ){
    case E_YEAR:
    {
        if(m_id!=id){
            m_id=id;
            ui->pushButton_p->setText(u8"上一年");
            ui->pushButton_next->setText(u8"下一年");
        }
        pushButton_year_clicked();
    }
        break;
    case E_MONTH:
    {
        if(m_id!=id){
            m_id=id;
            ui->pushButton_p->setText(u8"上个月");
            ui->pushButton_next->setText(u8"下个月");
        }
        pushButton_month_clicked();
    }
        break;
    case E_DAY:
    {
        if(m_id!=id){
            m_id=id;
            ui->pushButton_p->setText("上一天");
            ui->pushButton_next->setText("下一天");
        }
        pushButton_day_clicked();
    }
        break;
    }
}

void DsWidget::loadFinished()
{
    qDebug()<<__FUNCTION__;
    isOK = true;

    pushButton_month_clicked();
}

void DsWidget::on_btnRefresh_clicked()
{
    m_web->reload();
}

void DsWidget::pushButton_day_clicked()
{
    if(!isOK){
        return;
    }


#ifdef D_USE_MYSQL
    // MySql的连接方式                    类型      连接名        IP        数据库    用户名        密码
    //      XDatabase::Control control2({ "QMYSQL", "TestDB", "localhost", "JasonDB", "root", "YourPassword" });
    static XDatabase::Control control( { "QMYSQL", QTime::currentTime().toString("hh:mm:ss.zzz"), "localhost", D_DB_FILE_A, "root", "YourPassword" } );
#else
    static XDatabase::Control control( { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A }  );
#endif

    auto query(control.query());

    QDate date = ui->calendarWidget->selectedDate();
    TableDataSub dat;
    selectDay( query.getQuery(), date, dat);

    QString dateStr = date.toString( D_DATEFORMAT_DAY_SQL );
    QString title = QString(u8"日表:") + dateStr;

    qDebug()<<__FUNCTION__<< title;


    QJsonObject jsObj;
    jsObj["tableName"]= title;

    QJsonObject js;
    TableDataSubToJson(dat, js, D_DATEFORMAT_HOUR_SHOW, true);
    jsObj["data"]=js;

    TableDataSub datSum;
    SumProductLineData(dat,datSum,dateStr);
    TableDataSubToJson(datSum, js, D_DATEFORMAT_DAY_SHOW);
    jsObj["dataSum"]=js;

    qDebug()<<"jsObj===="<< QString(QJsonDocument(jsObj).toJson(QJsonDocument::Compact)) ;

    m_web->setData(0, jsObj);
}

void DsWidget::pushButton_month_clicked()
{
    if(!isOK){
        return;
    }
#ifdef D_USE_MYSQL
    static XDatabase::Control control( { "QMYSQL", QTime::currentTime().toString("hh:mm:ss.zzz"), "localhost", D_DB_FILE_A, "root", "YourPassword" } );
#else
    static XDatabase::Control control(
    { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A },
    { 3200, XDatabase::QueryAutoMode, D_DB_WAITTIME}
                );
#endif
    auto query(control.query());

    QDate date = ui->calendarWidget->selectedDate();
    TableDataSub dat;
    selectMonth( query.getQuery(), date, dat);

    QString dateStr = date.toString( D_DATEFORMAT_MONTH_SQL );
    QString title = QString(u8"月表:") + dateStr;

    qDebug()<<__FUNCTION__<< title;

    QJsonObject jsObj;
    jsObj["tableName"]= title;

    QJsonObject js;
    TableDataSubToJson(dat, js, D_DATEFORMAT_DAY_SHOW, true);
    jsObj["data"]=js;

    TableDataSub datSum;
    SumProductLineData(dat,datSum,dateStr);
    TableDataSubToJson(datSum, js, D_DATEFORMAT_MONTH_SHOW );
    jsObj["dataSum"]=js;

    qDebug()<<"jsObj===="<< QString(QJsonDocument(jsObj).toJson(QJsonDocument::Compact)) ;

    m_web->setData(0, jsObj);
}

void DsWidget::pushButton_year_clicked()
{
    if(!isOK){
        return;
    }
#ifdef D_USE_MYSQL
    static XDatabase::Control control( { "QMYSQL", QTime::currentTime().toString("hh:mm:ss.zzz"), "localhost", D_DB_FILE_A, "root", "YourPassword" } );
#else
    static XDatabase::Control control(
    { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A },
    { 3200, XDatabase::QueryAutoMode, D_DB_WAITTIME}
                );
#endif
    auto query(control.query());

    QDate date = ui->calendarWidget->selectedDate();
    TableDataSub dat;
    selectYear( query.getQuery(), date, dat);

    QString dateStr = date.toString( D_DATEFORMAT_YEAR_SQL );
    QString title = QString(u8"年表:") + dateStr;

    qDebug()<<__FUNCTION__<< title << dat.keys();

    QJsonObject jsObj;
    jsObj["tableName"]= title;

    QJsonObject js;
    TableDataSubToJson(dat, js, D_DATEFORMAT_MONTH_SHOW, true);
    jsObj["data"]=js;

    TableDataSub datSum;

    SumProductLineData(dat,datSum,dateStr);
    TableDataSubToJson(datSum, js, D_DATEFORMAT_YEAR_SHOW);
    jsObj["dataSum"]=js;

    qDebug()<<"jsObj===="<< QString(QJsonDocument(jsObj).toJson(QJsonDocument::Compact)) ;

    m_web->setData(0, jsObj);
}

void DsWidget::on_pushButton_tj_clicked()
{
    if(!isOK){
        return;
    }
    QDateTime start=ui->dateTimeEdit_start->dateTime();
    QDateTime end=ui->dateTimeEdit_end->dateTime();

#ifdef D_USE_MYSQL
    static XDatabase::Control control( { "QMYSQL", QTime::currentTime().toString("hh:mm:ss.zzz"), "localhost", D_DB_FILE_A, "root", "YourPassword" } );
#else
    static XDatabase::Control control(
    { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A },
    { 3200, XDatabase::QueryAutoMode, D_DB_WAITTIME}
                );
#endif
    auto query(control.query());

    TableDataSub dat;
    selectWithTime( query.getQuery(), start, end, dat);


    QString dateStr =  start.toString( D_DATEFORMAT_SECOND_SQL )
            +QString(u8" 到 ") +end.toString( D_DATEFORMAT_SECOND_SQL );
    QString title =  QString(u8"按时间统计:") + dateStr;

    qDebug()<<__FUNCTION__<< title << start << end;


    if(start.addMonths(12)<end){

        QJsonObject jsObj;
        jsObj["tableName"]= title;

        QJsonObject js;
        TableDataSubToJson(dat, js, D_DATEFORMAT_MONTH_SHOW , true);
        jsObj["data"]=js;

        TableDataSub datSum;
        SumProductLineData(dat, datSum, dateStr);
        TableDataSubToJson(datSum, js, D_DATEFORMAT_NONE );
        jsObj["dataSum"]=js;

        m_web->setData(0, jsObj);
    }else  if(start.addMonths(1)<end){

        QJsonObject jsObj;
        jsObj["tableName"]= title;

        QJsonObject js;
        TableDataSubToJson(dat, js, D_DATEFORMAT_DAY_SHOW , true);
        jsObj["data"]=js;

        TableDataSub datSum;
        SumProductLineData(dat, datSum, dateStr);
        TableDataSubToJson(datSum, js, D_DATEFORMAT_NONE );
        jsObj["dataSum"]=js;

        m_web->setData(0, jsObj);
    }else{

        QJsonObject jsObj;
        jsObj["tableName"]= title;

        QJsonObject js;
        TableDataSubToJson(dat, js, D_DATEFORMAT_NONE , true);
        jsObj["data"]=js;

        TableDataSub datSum;
        SumProductLineData(dat, datSum, dateStr);
        TableDataSubToJson(datSum, js, D_DATEFORMAT_NONE );
        jsObj["dataSum"]=js;

        m_web->setData(0, jsObj);
    }



#if 0
    //数据太多,无法打印,只能写到文档
    QFile ff("./dataJS.txt");
    ff.open(QFile::WriteOnly|QFile::Text);
    {
        ff.write( QJsonDocument(jsObj).toJson(QJsonDocument::Compact) );
    }
    ff.close();

    //    qDebug()<<"jsObj===="<< QString(QJsonDocument(jsObj).toJson(QJsonDocument::Compact)) ;
#endif

}

void DsWidget::on_calendarWidget_activated(const QDate &date)
{
    qDebug()<<__FUNCTION__<<date;
}

void DsWidget::on_calendarWidget_selectionChanged()
{
    if(m_date == ui->calendarWidget->selectedDate()){
        return;
    }

    qDebug()<<__FUNCTION__<<  ui->calendarWidget->selectedDate();

    if(btGroup.checkedId()!=E_DAY){
        if( m_date.year() !=  ui->calendarWidget->selectedDate().year()
                || m_date.month() !=  ui->calendarWidget->selectedDate().month() )
        {
            qDebug()<<__FUNCTION__<< "test===1==" << ui->calendarWidget->selectedDate();
            buttonToggled(0,false);
            qDebug()<<__FUNCTION__<< "test===11==" << ui->calendarWidget->selectedDate();
        }else{

            qDebug()<<__FUNCTION__<< "do nothing;";
        }
    }else{
        qDebug()<<__FUNCTION__<< "test===2==" << ui->calendarWidget->selectedDate();
        buttonToggled(0,false);
        qDebug()<<__FUNCTION__<< "test===22==" << ui->calendarWidget->selectedDate();
    }

    m_date = ui->calendarWidget->selectedDate();
}

void DsWidget::on_calendarWidget_currentPageChanged(int year, int month)
{
    qDebug()<<__FUNCTION__<<"test==31=="<<   ui->calendarWidget->selectedDate();

    QDate dt =  ui->calendarWidget->selectedDate();
    bool ok = dt.setDate( year, month, dt.day() );
    if( !ok ){
        dt.setDate( year, month, 1 );
    }
    qDebug()<<__FUNCTION__<<"test==31=="<<   ui->calendarWidget->selectedDate();
    ui->calendarWidget->setSelectedDate( dt );
    qDebug()<<__FUNCTION__<<"test==33=="<<   ui->calendarWidget->selectedDate();
}

void DsWidget::on_pushButton_p_clicked()
{
    qDebug()<<__FUNCTION__<< ui->calendarWidget->selectionMode();

    switch( btGroup.checkedId() ){
    case E_YEAR:
    {
        QDate dt = ui->calendarWidget->selectedDate().addYears(-1);
        ui->calendarWidget->setSelectedDate( dt );
    }
        break;
    case E_MONTH:
    {
        QDate dt = ui->calendarWidget->selectedDate().addMonths(-1);
        ui->calendarWidget->setSelectedDate( dt );
    }
        break;
    case E_DAY:
    {
        QDate dt = ui->calendarWidget->selectedDate().addDays(-1);
        ui->calendarWidget->setSelectedDate( dt );
    }
        break;
    }
}

void DsWidget::on_pushButton_next_clicked()
{
    qDebug()<<__FUNCTION__<< ui->calendarWidget->selectionMode();

    switch( btGroup.checkedId() ){
    case E_YEAR:
    {
        QDate dt = ui->calendarWidget->selectedDate().addYears(1);
        ui->calendarWidget->setSelectedDate( dt );
    }
        break;
    case E_MONTH:
    {
        QDate dt = ui->calendarWidget->selectedDate().addMonths(1);
        ui->calendarWidget->setSelectedDate( dt );
    }
        break;
    case E_DAY:
    {
        QDate dt = ui->calendarWidget->selectedDate().addDays(1);
        ui->calendarWidget->setSelectedDate( dt );
    }
        break;
    }
}
