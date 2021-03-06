#include "XDatabase.h"
// Qt lib import
#include <QCoreApplication>
#include <QSqlError>
#include <QThread>

//#include <QtConcurrent>



using namespace XDatabase;


// DatabaseSettings
DatabaseSettings::DatabaseSettings(const DatabaseModeEnum &databastMode, const QString &databaseType, const QString &connectionName)
{
    m_databaseMode = databastMode;
    m_databaseType = databaseType;
    m_connectionName = connectionName;
}

DatabaseSettings::DatabaseSettings(const QString &databaseType, const QString &connectionName, const QString &nameModeName):
    DatabaseSettings(DatabaseNameMode, databaseType, connectionName)
{
    m_nameModeName = nameModeName;
}

DatabaseSettings::DatabaseSettings(const QString &databaseType, const QString &connectionName, const QString &hostModeHostName, const QString &hostModeDatabaseName, const QString &hostModeUserName, const QString &hostModePassword):
    DatabaseSettings(DatabaseHostMode, databaseType, connectionName)
{
    m_hostModeHostName = hostModeHostName;
    m_hostModeDatabaseName = hostModeDatabaseName;
    m_hostModeUserName = hostModeUserName;
    m_hostModePassword = hostModePassword;
}

// ConnectSettings
ConnectSettings::ConnectSettings(const int &maxOpenTime, const QueryMode &queryMode, const int &minWaitTime)
{
    m_maxOpenTime = maxOpenTime;
    m_queryMode = queryMode;
    m_minWaitTime = minWaitTime;
}

// Query
Query::Query(QSqlDatabase &dataBase, QMutex *mutex):
    m_query(new QSqlQuery(dataBase))
{
    m_mutex = mutex;
}

Query::Query(Query &&other)
{
    m_query = other.takeQuery();
    m_mutex = other.takeMutex();
}

Query::~Query(void)
{
    if(m_query)
    {
        delete m_query;
    }
    if(m_mutex)
    {
        m_mutex->unlock();
    }
}

QSqlQuery *Query::getQuery()
{
    return m_query;
}

QSqlQuery *Query::takeQuery(void)
{
    auto buf = m_query;
    m_query = NULL;
    return buf;
}

QMutex *Query::takeMutex(void)
{
    auto buf = m_mutex;
    m_mutex = NULL;
    return buf;
}

// ConnectNode
ConnectNode::ConnectNode(const DatabaseSettings &dataBaseSettings, const ConnectSettings &connectSettings):
    m_dataBaseSettings(dataBaseSettings),
    m_connectSettings(connectSettings)
{
    m_connectionName = QString("%1(%2)").arg(m_dataBaseSettings.connectionName()).arg(QString::number(qint64(QThread::currentThread()), 16));

    m_mutex = new QMutex(QMutex::Recursive);

    if(m_connectSettings.maxOpenTime())
    {
        m_autoClose = new QTimer;
        m_autoClose->setSingleShot(true);
        m_autoClose->setInterval(m_connectSettings.maxOpenTime());
        m_autoClose->moveToThread(qApp->thread());
        m_autoClose->setParent(qApp);

        connect(m_autoClose, SIGNAL(timeout()), this, SLOT(close()), Qt::DirectConnection);
        connect(this, SIGNAL(controlStartAutoClose()), m_autoClose, SLOT(start()));
        connect(this, SIGNAL(controlStopAutoClose()),  m_autoClose, SLOT(stop()));
    }

    this->addDataBase();
}

ConnectNode::~ConnectNode(void)
{
    if(m_mutex){ m_mutex->lock(); }

    if(m_autoClose)
    {
        m_autoClose->deleteLater();
    }

    this->removeDataBase();

    if(m_mutex){ m_mutex->unlock(); }
    if(m_mutex){ delete m_mutex; }
}

Query ConnectNode::query(void)
{
    if(!m_database)
    {
        this->addDataBase();
    }

    if(!m_database->isOpen())
    {
        m_database->open();
    }

    if(m_mutex){ m_mutex->lock(); }

    Query buf(*m_database, m_mutex);
    emit controlStartAutoClose();

    //no unlock ??? why
    return buf;
}

bool ConnectNode::addDataBase(void)
{
    if(m_mutex){ m_mutex->lock(); }

    if(m_database)
    {
        this->removeDataBase();
    }

    m_database = new QSqlDatabase(QSqlDatabase::addDatabase(m_dataBaseSettings.databaseType(), m_connectionName));

    switch(m_dataBaseSettings.databaseMode())
    {
    case DatabaseNameMode:
    {
        m_database->setDatabaseName(m_dataBaseSettings.nameModeName());
        break;
    }
    case DatabaseHostMode:
    {
        m_database->setHostName(m_dataBaseSettings.hostModeHostName());
        m_database->setDatabaseName(m_dataBaseSettings.hostModeDatabaseName());
        m_database->setUserName(m_dataBaseSettings.hostModeUserName());
        m_database->setPassword(m_dataBaseSettings.hostModePassword());
//        m_database->setPort(3306);
        break;
    }
    default:
    {
        if(m_mutex){ m_mutex->unlock(); }
        return false;
    }
    }

    const auto &&flag = this->open();

    if(m_dataBaseSettings.databaseType() == "1QSQLITE")
    {
        //    Sqlite ?????????????????????????????????????????????110ms??????????????????????????????
        //??????????????????
        qDebug()<<"test===========111"<<m_dataBaseSettings.databaseType();
        m_database->exec("PRAGMA synchronous = NORMAL");//NORMAL????????????????????????????????????????????????,32ms???FULL ???????????????????????????????????????,10ms???//OFF ??????????????????
        m_database->exec("PRAGMA journal_mode = MEMORY");//SQL?????????????????????????????????
    }

    if(m_mutex){ m_mutex->unlock(); }

    return flag;
}

void ConnectNode::removeDataBase(void)
{
    if(m_mutex){ m_mutex->lock(); }

    delete m_database;
    m_database = NULL;
    QSqlDatabase::removeDatabase(m_connectionName);

    if(m_mutex){ m_mutex->unlock(); }
}

bool ConnectNode::open(void)
{
    if(!m_database)
    {
        this->addDataBase();
    }

    if(m_mutex){ m_mutex->lock(); }

    emit controlStartAutoClose();
    const auto &&Flag = m_database->open();

    if(m_mutex){ m_mutex->unlock(); }

    return Flag;
}

void ConnectNode::close(void)
{
    if(m_mutex)
    {
        if(m_mutex->tryLock())
        {
            m_mutex->unlock();
            emit controlStopAutoClose();
            m_database->close();
        }
        else
        {
            emit controlStartAutoClose();
        }
    }
    else
    {
        emit controlStopAutoClose();
        m_database->close();
    }
}

// Control
Control::Control(const DatabaseSettings &databaseSettings, const ConnectSettings &connectSettings):
    m_databaseSettings(databaseSettings),
    m_connectSettings(connectSettings)
{
    if(m_connectSettings.queryMode() == QueryAutoMode)
    {
        if(databaseSettings.databaseType() == "QMYSQL")
        {
            m_connectSettings.setQueryMode(QueryMultiMode);
        }
        else if(databaseSettings.databaseType() == "QODBC")
        {
            m_connectSettings.setQueryMode(QueryMultiMode);
        }
        else
        {
            m_connectSettings.setQueryMode(QuerySingleMode);
        }
    }
    if(m_connectSettings.queryMode() == QuerySingleMode)
    {
        this->insert(qint64(QThread::currentThread()));
    }

    if(m_connectSettings.minWaitTime() == -1)
    {
        if(databaseSettings.databaseType() == "QMYSQL")
        {
            m_connectSettings.setMinWaitTime(0);
        }
        else if(databaseSettings.databaseType() == "QODBC")
        {
            m_connectSettings.setMinWaitTime(0);
        }
        else
        {
            m_connectSettings.setMinWaitTime(5);
            m_wait = new QTime;
            m_wait->start();
        }
    }
    else
    {
        m_wait = new QTime;
        m_wait->start();
    }
}

Control::~Control(void)
{
    foreach(auto &now, m_node)
    {
        now->deleteLater();
    }
    if(m_wait)
    {
        delete m_wait;
        m_wait=NULL;
    }
}

void Control::removeAll(void)
{
    m_mutex.lock();

    foreach(auto &Now, m_node)
    {
        Now->removeDataBase();
    }

    m_mutex.unlock();
}

//#include <assert.h>
Query Control::query(void)
{
    if(m_connectSettings.queryMode() != QueryMultiMode)
    {
        bool ok = (qint64(QThread::currentThread()) == m_node.keys().at(0));
        Q_ASSERT_X(  ok , __FUNCTION__, "mutilThread call db error!!!");
    }

//    qDebug()<<"gggggggggggggggggggggggggggggg queryMode()=="<< m_connectSettings.queryMode()  <<(qint64(QThread::currentThread()) == m_node.keys().at(0) );

    if(m_wait)
    {
        const auto &&flag = m_connectSettings.minWaitTime() - m_wait->elapsed();
        m_wait->restart();
        if(flag > 0)
        {
            QThread::msleep(flag);
        }
    }

//    qDebug()<< "m_node=2=="<<m_node.size() <<QThread::currentThread();

    if(m_connectSettings.queryMode() == QueryMultiMode)
    {
        m_mutex.lock();

        const auto &currentThread = qint64(QThread::currentThread());
        const auto &&now = m_node.find(currentThread);
        if(now != m_node.end())
        {
            auto buf((*now)->query());

            m_mutex.unlock();
            return buf;
        }
        else
        {
            this->insert(qint64(QThread::currentThread()));
            m_mutex.unlock();
            return this->query();
        }
    }
    else
    {

        Q_ASSERT_X( qint64(QThread::currentThread()) == m_node.keys().at(0), __FUNCTION__, "mutilThread call db error!!!");

//            if(m_node.size()<1){
//                m_mutex.lock();
//                this->insert(qint64(QThread::currentThread()));
//                m_mutex.unlock();
//            }

        return (*m_node.begin())->query();
    }
}

QSqlDatabase* Control::db()
{
    return m_node.value( qint64(QThread::currentThread()) )->dataBase();
}

void Control::insert(const qint64 &key)
{
    m_node[key] = new ConnectNode(m_databaseSettings, m_connectSettings);
}


//?????????

// JasonQt lib import
//#include "JasonQt/XDatabase.h"

int maintest()
{
//    int argc, char *argv[]
//    QCoreApplication a(argc, argv);

    /*
     * ????????????????????????
     * ??????????????????????????????????????????????????????????????????
     *
     * ???????????????????????????????????????????????????open?????????????????????????????????????????????open???????????????????????????????????????????????????????????????????????????60 * 1000?????????60??????????????????????????????????????????
     * ?????????????????????????????????????????????????????????????????? MySql ??? XDatabase::QueryMultiMode ????????????????????? Sqlite ?????? XDatabase::QuerySingleMode ,???????????????
     * ???????????????????????????????????????????????????Sqlite????????????????????????????????????????????????????????????????????????????????????????????????????????????10ms
     */

    // Sqlite???????????????                    ??????        ?????????        Sqlite????????????      ?????????????????????????????????                ???????????????           ??????????????????
    XDatabase::Control control(
    { "QSQLITE", "TestDB", "./test.db" },
    { 60 * 1000, XDatabase::QuerySingleMode, 10}
                );

    // MySql???????????????                    ??????      ?????????        IP        ?????????    ?????????        ??????
    //  XDatabase::Control control({ "QMYSQL", "TestDB", "localhost", "JasonDB", "root", "YourPassword" });


    // SqlServer???????????????                  ??????      ?????????                                    ?????????              ????????????   ?????????         ??????
    //  XDatabase::Control control({ "QODBC", "TestDB", "Driver={SQL SERVER};server=iZ23kn6vmZ\\TEST;database=test;uid=sa;pwd=YourPassword;" });



    auto query(control.query());

    query->prepare("select * from Test1 where data = ?");

    query->addBindValue(rand() % 1280);

    if(!query->exec())
    {
        qDebug() << "Error" << __LINE__;
    }

    auto insert = [&]()
    {
        auto query(control.query()); // ?????????query??????????????? -> ?????? * ?????????????????? QSqlQuery ?????????????????????????????????????????????????????????????????????????????????

        query->prepare("insert into Test1 values(?)"); // ??????????????????

        query->addBindValue(rand() % 1280);

        if(!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }

        query->clear();

        query->prepare("insert into Test2 values(NULL, ?, ?)");

        query->addBindValue(rand() % 1280);
        QString buf;
        for(int now = 0; now < 50; now++)
        {
            buf.append('a' + (rand() % 26));
        }
        query->addBindValue(buf);

        if(!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }
    };
    auto delete_ = [&]()
    {
        auto query(control.query());

        query->prepare("delete from Test1 where data = ?");

        query->addBindValue(rand() % 1280);

        if(!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }

        query->clear();

        query->prepare("delete from Test2 where data1 = ?");

        query->addBindValue(rand() % 1280);

        if(!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }
    };
    auto update = [&]()
    {
        auto query(control.query());

        query->prepare("update Test1 set data = ? where data = ?");

        query->addBindValue(rand() % 1280);
        query->addBindValue(rand() % 1280);

        if(!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }

        query->clear();

        query->prepare("update Test2 set data1 = ?, data2 = ? where data1 = ?");

        query->addBindValue(rand() % 1280 + 1);
        QString buf;
        for(int now = 0; now < 50; now++)
        {
            buf.append('a' + (rand() % 26));
        }
        query->addBindValue(buf);
        query->addBindValue(rand() % 1280 + 1);

        if(!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }
    };
    auto select = [&]()
    {
        auto query(control.query());

        query->prepare("select * from Test1 where data = ?");

        query->addBindValue(rand() % 1280);

        if(!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }

        query->clear();

        query->prepare("select * from Test2 where data1 = ?");

        query->addBindValue(rand() % 1280);

        if(!query->exec())
        {
            qDebug() << "Error" << __LINE__;
        }
    };

    volatile int count = 0, last = 0;

    QTime time;
    time.start();

    QThreadPool::globalInstance()->setMaxThreadCount(10);

#if 0
    for(int now = 0; now < 1; now++) // ??????3???????????????
    {
        QtConcurrent::run([&]()
        {
            while(true)
            {
                count++;
                if(!(count % 1000))
                {
                    const auto &&now = time.elapsed();
                    qDebug() << now - last; // ???????????????1000???????????????
                    last = now;
                }

                switch(rand() % 10)
                {
                case 0:  { insert(); break; }
                case 1:  { delete_(); break; }
                case 2:  { update(); break; }
                default: { select(); break; }
                }
            }
            QThread::msleep(10);
        });
    }
#endif

//    return a.exec();
    return 0;
}
