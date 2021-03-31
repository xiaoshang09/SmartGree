#include "tcpthread.h"
#include "tcpsocket.h"

#include <QEventLoop>
#include <QCoreApplication>
#include <QDebug>


TcpThread::TcpThread( int socketDescriptor,  QObject *parent ) :
    QThread(parent),
    m_socketId(socketDescriptor),
    m_loop(new QEventLoop)
{
    m_socket=NULL;
    m_isRun=false;
}

TcpThread::~TcpThread()
{
    stopThread();
}

bool TcpThread::isRunning()
{
    return m_isRun;
}

bool TcpThread::isServer()
{
    return m_socketId?true:false;
}

int TcpThread::socketId()
{
    return m_socketId;
}

bool TcpThread::state()
{
    return m_state;
}

void TcpThread::startThread()
{
    if(!isRunning())
        QThread::start();

    if( !m_isRun )
    {
        m_loop->exec();
    }
}

void TcpThread::stopThread()
{
    if(m_socket)
        setDisconnect();

    if(isRunning()){
        requestInterruption();
        quit();
        wait();
    }
}

void TcpThread::run()
{
    if( NULL==m_socket )
    {
        m_socket = new TcpSocket( m_socketId );

        connect( m_socket, SIGNAL( disconnected() ), this, SLOT( sltDisconnected() ) );
        connect( m_socket, SIGNAL( connected() ), this, SLOT(sltConnected()) );

        connect( this, SIGNAL( sigSetClientClose() ), m_socket, SLOT( sltSetSocketClose()) );
        connect( this, SIGNAL( tcpWrite(const QByteArray& )), m_socket, SLOT(tcpWrite(const QByteArray&) ), Qt::QueuedConnection );
#ifdef D_USE_QT_SIGNALS
        connect( m_socket, SIGNAL( tcpRead(const QByteArray& )), this, SIGNAL(readData(const QByteArray&)), Qt::QueuedConnection );
#else
        connect( m_socket, SIGNAL( tcpRead(const QByteArray& )), this, SLOT(tcpRead(const QByteArray&)) );
#endif
        if( m_socketId!=0 )
        {
            qDebug()<< "[Server]"<< m_socketId<<  QThread::currentThread();
            m_socket->setSocketDescriptor( m_socketId );
        } else {
            qDebug()<< "[Cient]"<< m_socketId<< QThread::currentThread();  //
            connect( this, SIGNAL(sigConnectToServer( QString,quint16 )), m_socket, SLOT(connectToServer( QString,quint16 )) , Qt::QueuedConnection );
        }
    }

    m_isRun=true;
    m_loop->quit();
    exec();
}

void TcpThread::timerEvent(QTimerEvent *event)
{
    QThread::timerEvent(event);

    reConnect();
}

void TcpThread::setDisconnect()
{
    emit sigSetClientClose();
#ifdef D_USE_QT_SIGNALS
#else
    m_leftBuf.clear();
#endif
}

void TcpThread::reConnect()
{
    if(state()){
        return;
    }

    emit sigConnectToServer(m_ip, m_port);
}

void TcpThread::connectToServer(QString ip, quint16 port, bool autoReconnect)
{
#ifdef D_USE_QT_SIGNALS
#else
    m_leftBuf.clear();
#endif
    startThread();

    if( !isServer() ){
        if(m_tmId>=0){
            killTimer(m_tmId);
            m_tmId=-1;
        }

        m_ip=ip;
        m_port=port;
        m_auto_reconnect=autoReconnect;
        emit sigConnectToServer(ip, port);
    }
}

#ifdef D_USE_QT_SIGNALS
#else
qint64 TcpThread::readData(char *data, qint64 maxlen)
{
    int len = m_leftBuf.size();
    if(len<1)
        return 0;

    qlock.lock();
    len = maxlen>len?len:maxlen;
    memcpy( data, m_leftBuf.data(), len );
    m_leftBuf.remove(0,len);
    qlock.unlock();

    return len;
}

QByteArray TcpThread::readData(){
    qlock.lock();
    QByteArray buf = m_leftBuf;
    m_leftBuf.clear();
    qlock.unlock();
    return buf;
}
#endif

qint64 TcpThread::writeData(const char *data, qint64 len)
{
    emit tcpWrite(QByteArray::fromRawData( data, len ) );
    return len;
}


void TcpThread::writeData(const QByteArray& buf){
    emit tcpWrite(buf);
}

void TcpThread::sltDisconnected()
{
    m_state=false;
#ifdef D_USE_QT_SIGNALS
#else
    m_leftBuf.clear();
#endif
    emit disconnected();

    if(m_auto_reconnect){
        m_tmId=startTimer(3500);//3.5S
        qDebug()<<socketId()<<",tcp try reconnect...";
    }
}

void TcpThread::sltConnected()
{
    m_state=true;
#ifdef D_USE_QT_SIGNALS
#else
    m_leftBuf.clear();
#endif
    emit connected();
}

#ifdef D_USE_QT_SIGNALS
#else
void TcpThread::tcpRead(const QByteArray &buf)
{
    qlock.lock();
    m_leftBuf += buf;
    qlock.unlock();
}
#endif
