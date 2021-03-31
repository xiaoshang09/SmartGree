#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QPointer>

class TcpThread;

#include "tcpcomm_global.h"
class TCPCOMMSHARED_EXPORT  TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();

    QPointer<TcpThread> client() const {return m_socket;}

protected:
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))  
    void incomingConnection(int socketDescriptor);
#else
    void incomingConnection(qintptr handle);
#endif

signals:
    void newClient(int);
    void clientClosed(int);

public slots:
    void removeClient();

private:
    QPointer<TcpThread> m_socket;

};

#endif // TCPSERVER_H
