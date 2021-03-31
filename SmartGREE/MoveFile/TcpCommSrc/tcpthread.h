#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QPointer>
#include <QThread>
#include <QMutex>

#define D_USE_QT_SIGNALS

class QEventLoop;
class TcpSocket;

///
///线程安全的socket操作接口
///
#include "tcpcomm_global.h"
class TCPCOMMSHARED_EXPORT  TcpThread : public QThread
{
    Q_OBJECT
public:
    /**
    * @brief TcpThread
    * @param socketDescriptor , 0=客户端
    * @param parent
    */
    explicit TcpThread(int socketDescriptor=0, QObject *parent = 0 );
    ~TcpThread();
    bool isRunning();
    bool isServer();
    int  socketId();
    bool state();//true=connected

signals:
    void connected();
    void disconnected();

#ifdef D_USE_QT_SIGNALS
signals:
    void readData(const QByteArray& buf);
#else
public slots:
    qint64 readData(char *data, qint64 maxlen);
    QByteArray readData();
private slots:
    void tcpRead(const QByteArray &buf);
private:
    QMutex qlock;
    QByteArray m_leftBuf;
#endif

public slots:
    qint64 writeData(const char *data, qint64 len);
    void writeData(const QByteArray& buf);

    //==============only for client===================
public slots:
    void connectToServer(QString ip, quint16 port, bool autoReconnect=false);//连接
    void setDisconnect();//断开连接
    void reConnect();

public slots:
    void startThread();
    void stopThread();//关闭程序时释放资源

    //=================private========================
protected:
    void run();
    virtual void timerEvent(QTimerEvent *event);
signals:
    void sigSetClientClose();
    void sigConnectToServer(QString ip, quint16 port);
    void tcpWrite(const QByteArray& buf);
private slots:
    void sltDisconnected();
    void sltConnected();

private:
    int m_socketId;
    bool m_isRun;
    QPointer<TcpSocket> m_socket;
    QEventLoop* m_loop;
    QString m_ip;
    int m_port;
    int m_tmId=-1;
    bool m_state=false;
    bool m_auto_reconnect=false;

private:
//    friend class TcpServer;

};

#endif // TCPTHREAD_H
