#include "tcpserver.h"
#include "tcpthread.h"
#include <QDebug>


TcpServer::TcpServer(QObject *parent)
    :QTcpServer(parent)
    ,m_socket(NULL)
{
}

TcpServer::~TcpServer()
{
    removeClient();
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
void TcpServer::incomingConnection(int socketDescriptor)
#else
void TcpServer::incomingConnection(qintptr socketDescriptor)
#endif
{
    qDebug()<< "tcp new connect =="<< socketDescriptor;

    removeClient();
    m_socket = new TcpThread( socketDescriptor, this );
    m_socket->startThread();
    connect( m_socket, SIGNAL(disconnected()), this, SLOT(removeClient()) );

    emit newClient( this->serverPort() );
}

void TcpServer::removeClient()
{
    if(m_socket){
        qDebug()<< "tcp remve connect ==";

        m_socket->setDisconnect();
        m_socket->deleteLater();//防止内存泄漏
        m_socket=NULL;

        emit clientClosed( this->serverPort() );
    }
}



