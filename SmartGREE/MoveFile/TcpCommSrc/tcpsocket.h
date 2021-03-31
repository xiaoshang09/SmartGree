#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QByteArray>


class  TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    /**
     * @brief TcpSocket
     * @param socketDescriptor , 0=客户端
     * @param parent
     */
    explicit TcpSocket( int socketDescriptor=0, QObject *parent = 0 );
    ~TcpSocket();
    int socketId(){return m_socketId;}

signals:
    void tcpRead(const QByteArray &buf);
public slots:
    void tcpWrite(const QByteArray &buf);
    int connectToServer(QString ip, quint16 port);

private slots:
    void privateRead();
//    void privateWrite(const QByteArray &buf);
    void sltSetSocketClose();

private:
    int m_socketId;

    friend class TcpThread;
//    friend class TcpServer;
//    friend class TcpClient;

};


#endif // TCPSOCKET_H
