#include "tcpsocket.h"
#include <QDebug>

TcpSocket::TcpSocket(int socketDescriptor, QObject *parent) :
    QTcpSocket(parent),
    m_socketId(socketDescriptor)
{
    //    this->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 1024*1024);
    connect( this, SIGNAL(readyRead()) , this, SLOT( privateRead() ) );
}

TcpSocket::~TcpSocket()
{

}

void TcpSocket::tcpWrite(const QByteArray &buf)
{
//    qDebug()<<__FUNCTION__<< buf;
    if( !isOpen() )
        return;

    write(buf);
}

void TcpSocket::privateRead()
{
    if( !isOpen() )
        return;

    waitForReadyRead(1);

    QByteArray buf = readAll();
//    qDebug()<<__FUNCTION__<< buf;

    emit tcpRead( buf );
}

int TcpSocket::connectToServer( QString ip,  quint16 port )
{
    if( state()!= QTcpSocket::ConnectedState ){
        connectToHost( ip, port );

        if( !waitForConnected(500) )
        {
            qDebug()<<"connect to server failed !"<< ip << port ;
            return -1;
        }
        qDebug()<<"connect to server ok !"<< ip << port ;
    }
    if(!isOpen()){
        this->open(QTcpSocket::ReadWrite);
    }
    return 0;
}

void TcpSocket::sltSetSocketClose( )
{
    if(this->isOpen())
        this->close();
}
