#include "ProductSqlForm.h"
#include "ui_ProductSqlForm.h"
#include "SqlFunc.h"

#include <XDatabase.h>
#include <QTableView>
#include <QHeaderView>
#include <QDebug>



ProductSqlForm::ProductSqlForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductSqlForm)
{
    ui->setupUi(this);
    model=NULL;

#ifdef D_USE_MYSQL
    _db = QSqlDatabase::addDatabase("QMYSQL","ProductSqlForm"+QTime::currentTime().toString("HH:mm:ss.zzz"));
    _db.setHostName("localhost");
    _db.setUserName("root");
    _db.setPassword("");
#else
    _db=QSqlDatabase::addDatabase("QSQLITE","ProductSqlForm"+QTime::currentTime().toString("HH:mm:ss.zzz"));
#endif
    _db.setDatabaseName(D_DB_FILE_A);
    if( !_db.open() ){
        qDebug()<<__FUNCTION__<<"db open error !!!"<<_db.lastError();
        isOK=false;
    }else{
        isOK=true;
    }
    model = new QSqlTableModel(this, _db);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // grid原本就是有多少格显示多少格，
    //    ui->tableView->setShowGrid(false); // 可隐藏grid
    // 只能单选
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // 以行作为选择标准
    ui->tableView->setSelectionBehavior(QAbstractItemView::QAbstractItemView::SelectRows);
    // 行头隐藏
    //    ui->tableView->verticalHeader()->hide();
    // 让列头可被点击，触发点击事件
    ui->tableView->horizontalHeader()->setSectionsClickable(true);
    // 去掉选中表格时，列头的文字高亮
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    ui->tableView->horizontalHeader()->setBackgroundRole(QPalette::Background);
    // 列头灰色
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(225,225,225)};");
    //
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    //自动列宽//效率很低
//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //手动
    ui->tableView->setColumnWidth(0, 100);
    ui->tableView->setColumnWidth(1, 100);
    ui->tableView->setColumnWidth(2, 100);
    ui->tableView->setColumnWidth(3, 100);
    ui->tableView->setColumnWidth(4, 100);
    //    ui->tableView->setColumnWidth(5, 85);
    //    ui->tableView->setColumnWidth(6, 85);
    //

    connect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sortByColumn(int)));

    btGroup.addButton( ui->radioButton_year, E_YEAR );
    btGroup.addButton( ui->radioButton_month, E_MONTH );
    btGroup.addButton( ui->radioButton_day, E_DAY );

    connect( &btGroup, SIGNAL(buttonToggled(int, bool)), this , SLOT(buttonToggled(int, bool)) );
    m_id = btGroup.checkedId();
    m_date = ui->calendarWidget->selectedDate();
}

ProductSqlForm::~ProductSqlForm()
{
    delete ui;
}

void ProductSqlForm::showEvent(QShowEvent *event)
{
    qDebug()<<"test=====913======";
    pushButton_day_clicked();
    qDebug()<<"test=====915======";
}

void ProductSqlForm::sortByColumn(int col)
{
//    QSqlTableModel *pMode = dynamic_cast<QSqlTableModel *>(ui->tableView->model());
//    if(pMode){
//        bool ascending = (ui->tableView->horizontalHeader()->sortIndicatorSection()==col
//                          && ui->tableView->horizontalHeader()->sortIndicatorOrder()==Qt::DescendingOrder);
//        Qt::SortOrder order = ascending ? Qt::AscendingOrder : Qt::DescendingOrder;
//        pMode->sort(col, order);
//    }else{
//        qDebug()<<"db model empty!";
//    }
}

void ProductSqlForm::buttonToggled(int id, bool check)
{
    qDebug()<<__FUNCTION__<< btGroup.checkedId() << check;
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
            ui->pushButton_p->setText(u8"上一天");
            ui->pushButton_next->setText(u8"下一天");
        }
        pushButton_day_clicked();
    }
        break;
    }
}

void ProductSqlForm::pushButton_day_clicked()
{
    if(!isOK)
        return;

    QDate start = ui->calendarWidget->selectedDate();
    QDate end = start.addDays(1);
    QString date = start.toString( D_DATEFORMAT_DAY_SQL );
    QString title =  QString(u8"日表:")+date;
    QString table(D_SqlTablePdOutput);

    qDebug()<<__FUNCTION__<< title<<table;

    ui->label->setText(title);
    selecTable( table, start, end );

}

void ProductSqlForm::pushButton_month_clicked()
{
    if(!isOK)
        return;

    QDate start = ui->calendarWidget->selectedDate();
    start.setDate(start.year(),start.month(),1);
    QDate end = start.addMonths(1);
    QString date = start.toString( D_DATEFORMAT_MONTH_SQL );
    QString title =  QString(u8"月表:")+date;
    QString table(D_SqlTablePdOutput);

    ui->label->setText(title);
    selecTable( table, start, end );

    qDebug()<<__FUNCTION__<< title<<table;

}

void ProductSqlForm::pushButton_year_clicked()
{
    if(!isOK)
        return;

    QDate start = ui->calendarWidget->selectedDate();
    start.setDate(start.year(),1,1);
    QDate end = start.addMonths(12);
    QString date = start.toString( D_DATEFORMAT_MONTH_SQL );
    QString title =  QString(u8"年表:")+date;
    QString table(D_SqlTablePdOutput);

    ui->label->setText(title);
    selecTable( table, start, end );

    qDebug()<<__FUNCTION__<< title<<table;
}


void ProductSqlForm::on_calendarWidget_activated(const QDate &date)
{
    qDebug()<<__FUNCTION__<<date;
}

void ProductSqlForm::on_calendarWidget_selectionChanged()
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

void ProductSqlForm::on_calendarWidget_currentPageChanged(int year, int month)
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

void ProductSqlForm::on_pushButton_p_clicked()
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

void ProductSqlForm::on_pushButton_next_clicked()
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

int ProductSqlForm::selecTable(const QString &table, const QDate &start, const QDate &end)
{
    if(m_table!=table){
        m_table=table;
        model->setTable(m_table);
    }

    QTime T1 = QTime::currentTime();

#if 0

    //TODO 优化显示效率

    if(!isOK){
        return -1;
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

    QDate dt = ui->calendarWidget->selectedDate();
//    TableDataSub dat;
//    selectMonth( query.getQuery(), date, dat);

    QDate startx = dt ;
    bool ok = startx.setDate(dt.year(),1,1) ;

    QDate endx = startx.addMonths(36);
    quint64 startIndex=QDateTime( startx ).toSecsSinceEpoch();
    quint64 endIndex=QDateTime(endx).toSecsSinceEpoch();

    T1 = QTime::currentTime();

    dat.clear();
    selectDetails(query.getQuery(), D_SqlTablePdOutput,  startIndex, endIndex, dat, "time,pdid,result");

#else
    int startIndex = QDateTime(start).toSecsSinceEpoch();
    int endIndex = QDateTime(end).toSecsSinceEpoch();

//    QString filter = QString( "SELECT %0 FROM %1 WHERE tindex>=%2 AND tindex<%3" ).arg( "time,pdid,result" ).arg( table ).arg( startIndex ).arg( endIndex );
    QString filter = QString("tindex>=%1 AND tindex<%2").arg(startIndex).arg(endIndex);

//    model->query().exec( filter );
    model->setFilter( filter );
    model->sort(0, Qt::AscendingOrder);
    model->select();
    //当数据库超过256条数据时，model->rowCount 就会出现问题
    // 解决方法: 在查询语句后面，添加以下几行代码。
    while(model->canFetchMore())
    {
        model->fetchMore();
    }

//    qDebug()<<"test==========="<< start << end <<  model->query().numRowsAffected()<<  model->rowCount();

    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(3, true);
    ui->tableView->setColumnHidden(5, true);

    model->setHeaderData(0, Qt::Horizontal, u8"tindex");
    model->setHeaderData(1, Qt::Horizontal, u8"时间");
    model->setHeaderData(2, Qt::Horizontal, u8"产品ID");
    model->setHeaderData(3, Qt::Horizontal, u8"预留");
    model->setHeaderData(4, Qt::Horizontal, u8"结果");
    model->setHeaderData(5, Qt::Horizontal, u8"图片");

//    ui->tableView->setColumnWidth(0, 100);
    ui->tableView->setColumnWidth(1, 140);
    ui->tableView->setColumnWidth(2, 140);
//    ui->tableView->setColumnWidth(3, 100);
//    ui->tableView->setColumnWidth(4, 100);

#endif

    qDebug()<< __FUNCTION__ << "time used====="<< T1.msecsTo( QTime::currentTime() ) << model->rowCount() << T1 << QTime::currentTime(); // 2699 2131 坑  2181 18880

    ui->label_count->setText(QString(u8"总行数:%1").arg(model->rowCount()));
    model->query().clear();
    return 0;
}
