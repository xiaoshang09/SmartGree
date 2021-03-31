#include "MaintenanceWidget.h"
#include "ui_MaintenanceWidget.h"
#include "SqlFunc.h"
#include "SettingDatas.h"

#include <QMessageBox>

MaintenanceWidget::MaintenanceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaintenanceWidget)
{
    ui->setupUi(this);
    setMode(E_MATEN);
    this->resize( 1100, 700 );

    initTable();
    startTimer(3000);
}

MaintenanceWidget::~MaintenanceWidget()
{
    delete ui;
}

void MaintenanceWidget::initTable()
{

#ifdef D_USE_MYSQL
    _db = QSqlDatabase::addDatabase("QMYSQL","MaintenanceWidget"+QTime::currentTime().toString("HH:mm:ss.zzz"));
    _db.setHostName("localhost");
    _db.setUserName("root");
    _db.setPassword("");
#else
    _db=QSqlDatabase::addDatabase("QSQLITE","MaintenanceWidget"+QTime::currentTime().toString("HH:mm:ss.zzz"));
#endif
    _db.setDatabaseName(D_DB_FILE_A);
    if( !_db.open() ){
        qDebug()<<__FUNCTION__<<"db open error !!!"<<_db.lastError();
        isOK=false;
    }
    model = new QSqlTableModel(this, _db);

    ui->tableView->setModel(model);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // grid原本就是有多少格显示多少格，
//    ui->tableView->setShowGrid(true); // 可隐藏grid
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
    //        ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnWidth(0, 85);
    ui->tableView->setColumnWidth(1, 160);
    ui->tableView->setColumnWidth(2, 160);
    ui->tableView->setColumnWidth(3, 85);

}

void MaintenanceWidget::showTable()
{
    switch(m_mode){
    case E_MATEN:
    {
        model->setTable(D_SqlTableMaintenance);
//        model->sort(1, Qt::AscendingOrder);
        model->select();
        model->query().clear();

        ui->tableView->setColumnHidden(0,true);

        ui->tableView->setColumnWidth(0, 85);
        ui->tableView->setColumnWidth(1, 160);
        ui->tableView->setColumnWidth(2, 160);
        ui->tableView->setColumnWidth(3, 85);

        model->setHeaderData(0, Qt::Horizontal, u8"ID");
        model->setHeaderData(1, Qt::Horizontal, u8"预设保养时间");
        model->setHeaderData(2, Qt::Horizontal, u8"执行时间");
        model->setHeaderData(3, Qt::Horizontal, u8"保养员");
        model->setHeaderData(4, Qt::Horizontal, u8"备注");

//        qDebug()<<"rowCount=="<<model->rowCount();
    }
        break;
    case E_REP:
    {
        model->setTable(D_SqlTableReplacement);
//        model->sort(1, Qt::AscendingOrder);
        model->select();
        model->query().clear();

        ui->tableView->setColumnHidden(0,true);

        ui->tableView->setColumnWidth(0, 85);
        ui->tableView->setColumnWidth(1, 160);
        ui->tableView->setColumnWidth(2, 160);
        ui->tableView->setColumnWidth(3, 85);

        model->setHeaderData(0, Qt::Horizontal, u8"ID");
        model->setHeaderData(1, Qt::Horizontal, u8"预设更换时间");
        model->setHeaderData(2, Qt::Horizontal, u8"执行时间");
        model->setHeaderData(3, Qt::Horizontal, u8"维护员");
        model->setHeaderData(4, Qt::Horizontal, u8"备注");

//        qDebug()<<"rowCount=="<<model->rowCount();
    }
        break;
    }

}

void MaintenanceWidget::setMode(MaintenanceWidget::ENUM_MODE mode)
{
    m_mode = mode;
    switch(m_mode){
    case E_MATEN:
        this->setWindowTitle(u8"设备保养");
        break;
    case E_REP:
        this->setWindowTitle(u8"设备维护更换");
        break;
    }
}

void MaintenanceWidget::sortByColumn(int col)
{
    QSqlTableModel *pMode = dynamic_cast<QSqlTableModel *>(ui->tableView->model());
    if(pMode){
        bool ascending = (ui->tableView->horizontalHeader()->sortIndicatorSection()==col
                          && ui->tableView->horizontalHeader()->sortIndicatorOrder()==Qt::DescendingOrder);
        Qt::SortOrder order = ascending ? Qt::AscendingOrder : Qt::DescendingOrder;
        pMode->sort(col, order);
    }else{
        qDebug()<<"db model empty!";
    }
}

void MaintenanceWidget::setMsg(const QString &msg)
{
    ui->label_msg->setText(msg);
}

void MaintenanceWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    showTable();
}

void MaintenanceWidget::timerEvent(QTimerEvent *event)
{
    QWidget::timerEvent(event);

    checkTime();
}

void MaintenanceWidget::checkTime()
{
//    保养时间到== "2020-10-17" QDate("2020-10-17") QDate("2020-10-17")
//    rowCount== 9
//    维护时间到== "2020-10-18" QDate("2020-10-18") QDate("2020-10-17")

    switch(m_mode){
    case E_MATEN:
    {
        QString next= GetVal(DK_MaintenanceTimeNext).toString();
        QDate nextDt = QDate::fromString(next, D_DATEFORMAT_DAY);

        if(nextDt<=QDate::currentDate()){
            qDebug()<<u8"保养时间到=="<<next << nextDt <<QDate::currentDate();
            this->show();
        }
    }
        break;
    case E_REP:
    {
        QString next = GetVal(DK_ReplacementTimeNext).toString();
        QDate nextDt = QDate::fromString(next, D_DATEFORMAT_DAY);

        if(nextDt<=QDate::currentDate()){
            qDebug()<<u8"维护时间到=="<<next << nextDt <<QDate::currentDate();
            this->show();
        }
    }
        break;
    }
}

void MaintenanceWidget::on_pushButton_add_clicked()
{
    switch(m_mode){
    case E_MATEN:
    {
        QString &name = ui->lineEdit_name->text();
        if(name.isEmpty()){
            QMessageBox::information(this,u8"消息",u8"请填写名字!" );
            return;
        }

#if 1
        int inv = GetVal(DK_MaintenanceInv).toUInt();
        QString next= GetVal(DK_MaintenanceTimeNext).toString();
        QDate nextDt = QDate::fromString(next, D_DATEFORMAT_DAY);

        if(nextDt>QDate::currentDate()){
            QMessageBox::information(this,u8"消息",u8"未到保养时间!" );
            return;
        }
        QString last=next;

        nextDt=QDate::currentDate();
        SetVal( DK_MaintenanceTimeLast, nextDt.toString(D_DATEFORMAT_DAY) ) ;
        nextDt=nextDt.addDays( inv );
        next= nextDt.toString( D_DATEFORMAT_DAY );
        qDebug()<<u8"添加保养=="<<nextDt << next;
        SetVal( DK_MaintenanceTimeNext, next ) ;
#else
        QString last= GetVal(DK_MaintenanceTimeNext).toString();
        if( SettingMaintenance::setNextMaintenance(false) ){
            QMessageBox::information(this,"消息","未到保养时间!" );
            return;
        }
#endif

        QString note = ui->lineEdit_note->text();
        insertMaintenance(model->rowCount()+1, last, name, note );

        showTable();

        QMessageBox::information(this,u8"消息",u8"添加成功!" );
    }
        break;
    case E_REP:
    {
        QString &name = ui->lineEdit_name->text();
        if(name.isEmpty()){
            QMessageBox::information(this,u8"消息",u8"请填写名字!" );
            return;
        }
#if 1
        int inv = GetVal(DK_ReplacementInv).toUInt();
        QString next = GetVal(DK_ReplacementTimeNext).toString();
        QDate nextDt = QDate::fromString(next, D_DATEFORMAT_DAY);

        if(nextDt>QDate::currentDate()){
            QMessageBox::information(this,u8"消息",u8"未到维护更换时间!" );
            return;
        }
        QString last=next;

        nextDt=QDate::currentDate();
        SetVal( DK_ReplacementTimeLast, nextDt.toString(D_DATEFORMAT_DAY) ) ;
        nextDt=nextDt.addDays( inv );
        next= nextDt.toString( D_DATEFORMAT_DAY );

        qDebug()<<u8"添加维护=="<<nextDt << next;
        SetVal( DK_ReplacementTimeNext, next ) ;
#else
        QString last= GetVal(DK_ReplacementTimeNext).toString();
        if( SettingMaintenance::setNextReplacement(false) ){
            QMessageBox::information(this,"消息","未到维护更换时间!" );
            return;
        }
#endif
        QString note = ui->lineEdit_note->text();
        insertReplacement(model->rowCount()+1, last, name, note );

        showTable();

        QMessageBox::information(this,u8"消息",u8"添加成功!" );
    }
        break;
    }

}
