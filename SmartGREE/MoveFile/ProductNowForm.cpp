#include "ProductNowForm.h"
#include "ui_ProductNowForm.h"

#include "SqlFunc.h"
#include "CameraWidget.h"
#include "DsGroup.h"
#include "ProtocolHandller.h"

#include <QTableView>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMessageBox>

ProductNowForm::ProductNowForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductNowForm)
{
    ui->setupUi(this);
    m_web=NULL;
    qDebug()<<"test====0===";
    initWeb();

    initTableIdChanged();
    initTableErrorHapend();

    updateTableIdChanged();
    updateTableError();

    ui->gridLayout_2->addWidget(CameraWidget::instance(), 0, 0, 1, 1);
    ui->widget_23->hide();

    connect(CameraWidget::instance(),SIGNAL(changeID(const QString&)), this, SLOT(doIdChanged(const QString&)) );
    connect(ProtocolHandller::instance(),SIGNAL(triggerCamera(int)), this, SLOT(doTriggerCamera(int)) );
    connect(ProtocolHandller::instance(),SIGNAL(changeProgram(int)), this, SLOT(doChangeProgram(int)) );

//    void changeProgram(int);//切换PLC程序
    startTimer(3300);

    //自动更新
    //    QFileSystemWatcher *watcher = new QFileSystemWatcher(this);
    //        watcher->addPath("path");
    //        connect(watcher,SIGNAL(fileChanged(QString)),SLOT(updateData()));

}

ProductNowForm::~ProductNowForm()
{
    delete ui;
}

void ProductNowForm::doTriggerCamera(int type)
{
    if(m_id.isEmpty()){
//        qDebug()<<__FUNCTION__ << "m_id is empty !";
        return;
    }

    QString pic;
    int ret = 0;//0=false
    float x, y, z,Rx,Ry,Rz;

    //true = 正,false=反
    switch(type){
    case EM_TRIGGER_CAMERA_TITO://底托来料方向
    {
        ret=CameraWidget::instance()->check2D1Dir(m_id , pic);//
        switch(ret){
        case 1:   //zheng
            ProtocolHandller::instance()->sendToPlc_check2D1Dir(1,true);
            ret=CameraWidget::instance()->check2D1pos(m_id , pic);//图像处理
            break;
        case 0:   //fan
            ProtocolHandller::instance()->sendToPlc_check2D1Dir(1,false);
            break;
        case -1:   //NG
        default:
            ProtocolHandller::instance()->sendToPlc_check2D1Dir(0,false);
            break;
            //
        }

    }
        break;

    case EM_TRIGGER_CAMERA_METAL://地板拍照工位
    {
        ret=CameraWidget::instance()->check2D2pos(m_id , pic, x, y, z,Rx,Ry,Rz);
        switch(ret){
        case 1:   //OK
            ProtocolHandller::instance()->sendToPlc_check2D2pos(1,true, x, y, z,Rx,Ry,Rz);
            break;
        default: //NG
            ProtocolHandller::instance()->sendToPlc_check2D2pos(0,false, x, y, z,Rx,Ry,Rz);
            break;
        }
        //要装工件了
        QByteArray buf;
        buf.append(pic);
        doAddOutput( m_id, ret>0?1:0, buf);
    }
        break;

    default:
        break;
    }

#ifdef D_TEST
    ret = rand()%10>2?1:0;//for test

    QByteArray buf;
    buf.append(pic);
    doAddOutput( m_id, ret, buf);
#endif
}

void ProductNowForm::doChangeProgram(int mode)
{
    //TODO
    if(mode==1){
        //
    }
}

void ProductNowForm::timerEvent(QTimerEvent *event)
{

//    if(this->isVisible()){
//        updateTable();
//        updateDs();
//    }

#ifdef D_TEST

#endif
}

void ProductNowForm::initWeb()
{
    m_web = new MyWebView();

    ui->gridLayout_4->addWidget(m_web, 0, 0, 1, 1);

    connect( m_web , SIGNAL(loadFinished(bool)), this , SLOT(loadFinished()));

#if 1
    QString file = "dsNow.html";
    m_web->setUrl( QUrl::fromLocalFile( qApp->applicationDirPath()+"/"+ file ) );
#else
    m_web->setUrl(QUrl("qrc:/web/dsNow.html"));
#endif

}

void ProductNowForm::initTableIdChanged()
{
#ifdef D_USE_MYSQL
    QSqlDatabase _db = QSqlDatabase::addDatabase("QMYSQL","ProductNowForm2"+QTime::currentTime().toString("HH:mm:ss.zzz"));
    _db.setHostName("localhost");
    _db.setUserName("root");
    _db.setPassword("");
#else
    QSqlDatabase _db=QSqlDatabase::addDatabase("QSQLITE","ProductNowForm2"+QTime::currentTime().toString("HH:mm:ss.zzz"));
#endif
    _db.setDatabaseName(D_DB_FILE_A);
    if( !_db.open() ){
        qDebug()<<__FUNCTION__<<"db open error !!!"<<_db.lastError();
        isOK=false;
    }
    QSqlTableModel* model = new QSqlTableModel(this, _db);

    ui->tableView_idChanged->setModel(model);
    ui->tableView_idChanged->setColumnHidden(0, true);
    ui->tableView_idChanged->resizeColumnsToContents();
    ui->tableView_idChanged->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // grid原本就是有多少格显示多少格，
    //    ui->tableView_idChanged->setShowGrid(true); // 可隐藏grid
    // 只能单选
    ui->tableView_idChanged->setSelectionMode(QAbstractItemView::SingleSelection);
    // 以行作为选择标准
    ui->tableView_idChanged->setSelectionBehavior(QAbstractItemView::QAbstractItemView::SelectRows);
    // 行头隐藏
    //    ui->tableView_idChanged->verticalHeader()->hide();
    // 让列头可被点击，触发点击事件
    ui->tableView_idChanged->horizontalHeader()->setSectionsClickable(true);
    // 去掉选中表格时，列头的文字高亮
    ui->tableView_idChanged->horizontalHeader()->setHighlightSections(false);
    ui->tableView_idChanged->horizontalHeader()->setBackgroundRole(QPalette::Background);
    // 列头灰色
    ui->tableView_idChanged->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(225,225,225)};");
    //
    ui->tableView_idChanged->horizontalHeader()->setStretchLastSection(true);
    //自动列宽//效率很低
//    ui->tableView_idChanged->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //手动
    ui->tableView_idChanged->setColumnWidth(0, 85);
    ui->tableView_idChanged->setColumnWidth(1, 135);
    ui->tableView_idChanged->setColumnWidth(2, 135);
    ui->tableView_idChanged->setColumnWidth(3, 85);

}

void ProductNowForm::initTableErrorHapend()
{
#ifdef D_USE_MYSQL
    QSqlDatabase _db = QSqlDatabase::addDatabase("QMYSQL","ProductNowForm1"+QTime::currentTime().toString("HH:mm:ss.zzz"));
    _db.setHostName("localhost");
    _db.setUserName("root");
    _db.setPassword("");
#else
    QSqlDatabase _db=QSqlDatabase::addDatabase("QSQLITE","ProductNowForm1"+QTime::currentTime().toString("HH:mm:ss.zzz"));
#endif
    _db.setDatabaseName(D_DB_FILE_A);
    if( !_db.open() ){
        qDebug()<<__FUNCTION__<<"db open error !!!"<<_db.lastError();
        isOK=false;
    }
    QSqlTableModel* model = new QSqlTableModel(this, _db);

    ui->tableView_errorHapend->setModel(model);
    ui->tableView_errorHapend->setColumnHidden(0, true);
    ui->tableView_errorHapend->resizeColumnsToContents();
    ui->tableView_errorHapend->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // grid原本就是有多少格显示多少格，
    //    ui->tableView_errorHapend->setShowGrid(true); // 可隐藏grid
    // 只能单选
    ui->tableView_errorHapend->setSelectionMode(QAbstractItemView::SingleSelection);
    // 以行作为选择标准
    ui->tableView_errorHapend->setSelectionBehavior(QAbstractItemView::QAbstractItemView::SelectRows);
    // 行头隐藏
    //    ui->tableView_errorHapend->verticalHeader()->hide();
    // 让列头可被点击，触发点击事件
    ui->tableView_errorHapend->horizontalHeader()->setSectionsClickable(true);
    // 去掉选中表格时，列头的文字高亮
    ui->tableView_errorHapend->horizontalHeader()->setHighlightSections(false);
    ui->tableView_errorHapend->horizontalHeader()->setBackgroundRole(QPalette::Background);
    // 列头灰色
    ui->tableView_errorHapend->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(225,225,225)};");
    //
    ui->tableView_errorHapend->horizontalHeader()->setStretchLastSection(true);
    //自动列宽//效率很低
//    ui->tableView_errorHapend->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //手动
    ui->tableView_errorHapend->setColumnWidth(0, 85);
    ui->tableView_errorHapend->setColumnWidth(1, 135);
    ui->tableView_errorHapend->setColumnWidth(2, 135);
    ui->tableView_errorHapend->setColumnWidth(3, 85);

}

void ProductNowForm::loadFinished()
{
    updateDs();
}

void ProductNowForm::updateDs()
{
#if 1  //从数据库中统计

    XDatabase::Control control(
    { "QSQLITE", QTime::currentTime().toString("hh:mm:ss.zzz"), D_DB_FILE_A },
    { 3200, XDatabase::QueryAutoMode, D_DB_WAITTIME}
                );
    auto query(control.query());
#ifdef D_TEST
    QDate date = QDate::fromString("2019-11-29","yyyy-MM-dd");
#else
    QDate date = QDate::currentDate();
#endif
    TableDataSub dat;
    selectDay( query.getQuery(), date, dat);

    QString dateStr = date.toString( D_DATEFORMAT_DAY_SQL );
    QString title = QString(u8"当前产量:") + dateStr;

    QJsonObject jsObj;
    jsObj["tableName"]= title;

    QJsonObject js;
    TableDataSubToJson(dat, js, D_DATEFORMAT_HOUR_SHOW, true);
    jsObj["data"]=js;

    TableDataSub datSum;
    SumProductLineData(dat,datSum,dateStr);
    TableDataSubToJson(datSum, js, D_DATEFORMAT_DAY_SHOW);
    jsObj["dataSum"]=js;

//    qDebug()<<"jsObj==now=="<< date<< QString(QJsonDocument(jsObj).toJson(QJsonDocument::Compact)) ;

#else //从内存获取

    //取js数据
    //TODO
#endif

    m_web->setData(0, jsObj);
}

void ProductNowForm::updateTableError()
{
    int tindex = QDateTime(QDate::currentDate()).toSecsSinceEpoch();
    QSqlTableModel* modelx2 =qobject_cast<QSqlTableModel*>(ui->tableView_errorHapend->model());
    if(modelx2){
        modelx2->setTable(D_SqlTableErrorHappened);
        modelx2->setFilter(QString("tindex>%1").arg(tindex));
        modelx2->select();
        //当数据库超过256条数据时，model->rowCount 就会出现问题
        // 解决方法: 在查询语句后面，添加以下几行代码。
        while(modelx2->canFetchMore())
        {
            modelx2->fetchMore();
        }

        ui->tableView_errorHapend->setColumnHidden(0, true);
        ui->tableView_errorHapend->setColumnHidden(2, true);

//        modelx2->setHeaderData(0, Qt::Horizontal, u8"tindex");
        modelx2->setHeaderData(1, Qt::Horizontal, u8"发生时间");
//        modelx2->setHeaderData(2, Qt::Horizontal, u8"code");
        modelx2->setHeaderData(3, Qt::Horizontal, u8"消息");

        modelx2->query().clear();
    }

    ui->tableView_errorHapend->setColumnWidth(1, 135);
//        ui->tableView_errorHapend->setColumnWidth(3, 85);
}

void ProductNowForm::updateTableIdChanged()
{
    int tindex = QDateTime(QDate::currentDate()).toSecsSinceEpoch();
    QSqlTableModel* modelx1 =qobject_cast<QSqlTableModel*>(ui->tableView_idChanged->model());
    if(modelx1){
        modelx1->setTable(D_SqlTableProductIdChange);
        modelx1->setFilter(QString("tindex>%1").arg(tindex));
        modelx1->select();
        //当数据库超过256条数据时，model->rowCount 就会出现问题
        // 解决方法:  在查询语句后面，添加以下几行代码。
//        QTime T1 = QTime::currentTime();
        while(modelx1->canFetchMore())
        {
            modelx1->fetchMore();
        }

        ui->tableView_idChanged->setColumnHidden(0, true);

//        modelx1->setHeaderData(0, Qt::Horizontal, u8"tindex");
        modelx1->setHeaderData(1, Qt::Horizontal, u8"切换时间");
        modelx1->setHeaderData(2, Qt::Horizontal, u8"产品ID");

        modelx1->query().clear();
    }

    ui->tableView_idChanged->setColumnWidth(1, 135);
//        ui->tableView_idChanged->setColumnWidth(2, 85);

}

void ProductNowForm::doIdChanged(const QString &id)
{
    m_id=id;

    DsGroup::instance()->historyDs()->doIdClear();
    DsGroup::instance()->historyDs()->doIdAdd(id);

    int ret = CameraWidget::instance()->setProductId(id);
    if( ret ){
        qDebug()<<"camera setProductId"<<id;
    }

    updateTableIdChanged();
}

void ProductNowForm::doErrorHapend(const QString &msg)
{
    updateTableError();
}

void ProductNowForm::doAddOutput(const QString &id, bool ok, const QByteArray &pic)
{

    DsCycleBase* ds=DsGroup::instance()->historyDs();

    //修改数据
//    ds->lock();
//    S_ProductLineData*  pdat= ds->getData();
//    for(QString id : pdat->pdIDList){
//        S_PdOutputData* ptr = pdat->product.value(id);
//        ptr->output = 1;
//        ptr->output_ok = ok?1:0;
//        ptr->output_ng = ok?1:0;
//    }
//    ds->unlock();

//        qDebug()<<__FUNCTION__<<n << dt.toString("yyyy-MM-dd HH:mm:ss") << output  ;
    insertPdOutput(D_SqlTablePdOutput, QDateTime::currentDateTime(), id, 0, ok, pic);

    ds->doCurrentDataChanged();

//    ds->doRolling(dt,nextDt);

    updateDs();
}

void ProductNowForm::doProductStarted()
{
    DsGroup::instance()->historyDs()->doProductStarted();
}

void ProductNowForm::doProductWating()
{
    DsGroup::instance()->historyDs()->doProductWating();
}
