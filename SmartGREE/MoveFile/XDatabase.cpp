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
        //    Sqlite 插入速度慢，平均插入一条速度在110ms左右，所以需要优化：
        //提升读写速度
        qDebug()<<"test===========111"<<m_dataBaseSettings.databaseType();
        m_database->exec("PRAGMA synchronous = NORMAL");//NORMAL在关键的磁盘操作的每个序列后同步,32ms。FULL 在每个关键的磁盘操作后同步,10ms。//OFF 不进行同步。
        m_database->exec("PRAGMA journal_mode = MEMORY");//SQL日志记录保留在内存中。
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


//使用：

// JasonQt lib import
//#include "JasonQt/XDatabase.h"

int maintest()
{
//    int argc, char *argv[]
//    QCoreApplication a(argc, argv);

    /*
     * 注：关于附加参数
     * 这是可以不写的，如果要写的话，可以参考这个：
     *
     * 单次打开数据库最大时间：也就是最大open的时间，对于某些数据库，长时间open但不使用，不仅会造成资源浪费还会意外断开。在设置了60 * 1000后，且60秒内未进行查询，就自动断开。
     * 多线程支持：简单的说就是高级点的数据库，比如 MySql 写 XDatabase::QueryMultiMode ；低级的，比如 Sqlite ，写 XDatabase::QuerySingleMode ,就可以了。
     * 最小等待时间：对于某些数据库，比如Sqlite，密集查询时可能出错，此时可以适当的提升两次查询之间的最小等待时间，比如10ms
     */

    // Sqlite的连接方式                    类型        连接名        Sqlite文件路径      单次打开数据库最大时间                多线程支持           最小等待时间
    XDatabase::Control control(
    { "QSQLITE", "TestDB", "./test.db" },
    { 60 * 1000, XDatabase::QuerySingleMode, 10}
                );

    // MySql的连接方式                    类型      连接名        IP        数据库    用户名        密码
    //  XDatabase::Control control({ "QMYSQL", "TestDB", "localhost", "JasonDB", "root", "YourPassword" });


    // SqlServer的连接方式                  类型      连接名                                    数据库              数据库名   用户名         密码
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
        auto query(control.query()); // 这里的query在解引用（ -> 或者 * ）后返回的是 QSqlQuery ，直接用就可以了，不需要单独打开数据库或者其他的初始化

        query->prepare("insert into Test1 values(?)"); // 模拟插入操作

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
    for(int now = 0; now < 1; now++) // 开启3个线程测试
    {
        QtConcurrent::run([&]()
        {
            while(true)
            {
                count++;
                if(!(count % 1000))
                {
                    const auto &&now = time.elapsed();
                    qDebug() << now - last; // 打印每完成1000语句的时间
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
