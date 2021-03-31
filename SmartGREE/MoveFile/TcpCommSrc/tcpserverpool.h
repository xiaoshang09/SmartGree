#ifndef TCPSERVERPOOL_H
#define TCPSERVERPOOL_H


#include <QThread>
#include <QMap>

class QEventLoop;
class TcpThread;
class TcpServer;
class ServerPoolPrivate;


#include "tcpcomm_global.h"
class TCPCOMMSHARED_EXPORT  TcpServerPool : public QThread
{
    Q_OBJECT
public:
    static TcpServerPool* instance(){
        static TcpServerPool* obj = new TcpServerPool();
        return obj;
    }
    /**
    * @brief TcpServerPool
    * @param parent
    * 线程安全的tcp服务池,一个服务端口只有一个连接,有新连接的时会把旧的连接关闭
    */
    explicit TcpServerPool(QObject *parent = 0 );
    ~TcpServerPool();

    TcpThread* client(int port);
    TcpServer* server(int port);
    QList<int> svrs();//所有服务端口
    bool isRunning();

signals:
    void svrOn(int port);
    void svrDown(int port);

    void connected(int port);
    void disconnected(int port);

    //=========== api ============================
signals:
    void startSvr( int port );//call second
    void stopSvr( int port );
public slots:
    void startThread();//call first
    void stopThread();

    //===========private==========================
protected:
    void run();

private:
    bool m_isRun;
    QEventLoop* m_loop;
    ServerPoolPrivate* m_p;

};


class ServerPoolPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ServerPoolPrivate(TcpServerPool* ptr,QObject *parent = 0 );
    ~ServerPoolPrivate();

//signals:
//    void readData(const QByteArray& buf);
//public slots:
//    qint64 writeData(const char *data, qint64 len);
//    void writeData(const QByteArray& buf);

signals:
    void svrOn(int);
    void svrDown(int);

public slots:
    void release();
    void startSvr( int port );
    void stopSvr( int port );

private:
    QMap<int ,TcpServer* > m_svrs;
    TcpServerPool* m_ptr;

    friend class TcpServerPool;

};

#endif // TCPSERVERPOOL_H
