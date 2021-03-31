#ifndef XDATABASE_H
#define XDATABASE_H

// C++ lib import
#include <functional>

// Qt lib import
//#include <QtCore>
#include <QtSql>
#include <QObject>
#include <QMap>
//#include <QThread>


#define PropertyDeclare(Type, Name, setName, ...)                   \
    private:                                                        \
    Type m_ ## Name __VA_ARGS__;                                    \
    public:                                                         \
    inline const Type &Name(void) const { return m_ ## Name; }      \
    inline void setName(const Type &Name) { m_ ## Name = Name; }    \
    private:

namespace XDatabase
{

enum DatabaseModeEnum{ DatabaseNameMode, DatabaseHostMode };

enum QueryMode { QueryAutoMode, QueryMultiMode, QuerySingleMode };

#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  DatabaseSettings
{
private:
    PropertyDeclare(DatabaseModeEnum, databaseMode, setDatabaseMode)

    PropertyDeclare(QString, databaseType, setDatabaseType)
    PropertyDeclare(QString, connectionName, setConnectionName)

    // File mode
    PropertyDeclare(QString, nameModeName, setNameModeName)

    // Host mode
    PropertyDeclare(QString, hostModeHostName, setHostModeHostName)
    PropertyDeclare(QString, hostModeDatabaseName, setHostModeDatabaseName)
    PropertyDeclare(QString, hostModeUserName, setHostModeUserName)
    PropertyDeclare(QString, hostModePassword, setHostModePassword)

private:
    DatabaseSettings(const DatabaseModeEnum &databastMode, const QString &databaseType, const QString &connectionName);

public:
    DatabaseSettings(const QString &databaseType, const QString &connectionName, const QString &nameModeName);

    DatabaseSettings(const QString &databaseType, const QString &connectionName, const QString &hostModeHostName, const QString &hostModeDatabaseName, const QString &hostModeUserName, const QString &hostModePassword);
};

#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  ConnectSettings
{
private:
    PropertyDeclare(int, maxOpenTime, setMaxOpenTime)
    PropertyDeclare(QueryMode, queryMode, setQueryMode)
    PropertyDeclare(int, minWaitTime, setMinWaitTime)

public:
    ConnectSettings(const int &maxOpenTime = 60 * 1000, const QueryMode &queryMode = QueryAutoMode, const int &minWaitTime = -1);
};

#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  Query
{
private:
    QSqlQuery *m_query;
    QMutex *m_mutex = NULL;

public:
    Query(QSqlDatabase &dataBase, QMutex *mutex = NULL);

    Query(Query &&other);

    ~Query(void);

    inline QSqlQuery *operator->(void) { return m_query; }

    inline QSqlQuery *operator*(void) { return m_query; }

    QSqlQuery *getQuery(void);

    QSqlQuery *takeQuery(void);

    QMutex *takeMutex(void);
};

#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  ConnectNode: public QObject
{
     Q_OBJECT

private:
    QSqlDatabase *m_database = NULL;
    QString m_connectionName;

    DatabaseSettings m_dataBaseSettings;
    ConnectSettings m_connectSettings;

    QTimer *m_autoClose = NULL;
    QMutex *m_mutex = NULL;

public:
    ConnectNode(const DatabaseSettings &dataBaseSettings, const ConnectSettings &connectSettings);

    ~ConnectNode(void);

public:
    Query query(void);

    QSqlDatabase *dataBase(void){return m_database;}

public slots:
    bool addDataBase(void);

    void removeDataBase(void);

    bool open(void);

    void close(void);

signals:
    void controlStartAutoClose(void);

    void controlStopAutoClose(void);
};

#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  Control
{
private:
    DatabaseSettings m_databaseSettings;
    ConnectSettings m_connectSettings;

    QMap<qint64, ConnectNode *> m_node;

    QMutex m_mutex;
    QTime *m_wait = NULL;

public:
    Control(const DatabaseSettings &databaseSettings, const ConnectSettings &connectSettings = ConnectSettings());

    Control(const Control &) = delete;

    ~Control(void);

public :
    void removeAll(void);

    Query query(void);//sqlite下,多线程会导致崩溃//只能在ui线程用了

    QSqlDatabase *db(void);

private:
    void insert(const qint64 &key);
};


}


extern int maintest();

#endif // XDATABASE_H
