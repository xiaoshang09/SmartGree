#include "tcpserverpool.h"
#include "tcpsocket.h"
#include "tcpserver.h"
#include "tcpthread.h"

#include <QEventLoop>
#include <QCoreApplication>
#include <QDebug>


ServerPoolPrivate::ServerPoolPrivate(TcpServerPool* ptr, QObject *parent )
    :QObject(parent)
    ,m_ptr(ptr)
{  }

ServerPoolPrivate::~ServerPoolPrivate()
{
    release();
}

void ServerPoolPrivate::release(){
    foreach(auto svr, m_svrs){
        if(svr->isListening()){
            svr->close();
            svr->deleteLater();
        }
    }
    m_svrs.clear();
}

void ServerPoolPrivate::startSvr( int port ){
    if(m_svrs.contains( port ) ){
        qDebug()<< "WARN: server had stared !!! " << port ;
        return;
    }

    TcpServer* svr = new TcpServer();
    svr->listen( QHostAddress::Any , port );

    if( svr->isListening() ){
        qDebug()<< "INFO: start server ok :" << svr->serverAddress() << svr->serverPort() ;
        m_svrs.insert( port, svr );
        connect( svr, SIGNAL(newClient(int)), m_ptr, SIGNAL(connected(int) ) );
        connect( svr, SIGNAL(clientClosed(int)), m_ptr, SIGNAL(disconnected(int) ) );

        emit svrOn(port);
    }else{
        qDebug()<< "ERROR: start server failed :  " << svr->serverAddress() << svr->serverPort() ;
        emit svrDown(port);
    }
}

void ServerPoolPrivate::stopSvr( int port ){
    TcpServer* svr = m_svrs.take( port );
    if(!svr)
        return;
    if(svr->isListening()){
        svr->close();
        svr->deleteLater();
    }
    qDebug()<< "INFO: stop server :" << port ;
    emit svrDown(port);
}


TcpServerPool::TcpServerPool( QObject *parent )
    :QThread(parent)
    ,m_isRun(false)
    ,m_loop(new QEventLoop)
    ,m_p(NULL)
{
}

TcpServerPool::~TcpServerPool()
{
    stopThread();
}

TcpThread *TcpServerPool::client(int port)
{
    if(m_p){
        TcpServer* svr= m_p->m_svrs.value(port);
        if(svr){
            return svr->client();
        }
    }
    return NULL;
}

TcpServer *TcpServerPool::server(int port)
{
    if(m_p){
        TcpServer* svr= m_p->m_svrs.value(port);
        return svr;
    }
    return NULL;
}

QList<int> TcpServerPool::svrs()
{
    QList<int> ll;
    if(m_p){
        ll=m_p->m_svrs.keys();
    }
    return ll;
}

bool TcpServerPool::isRunning()
{
    return m_isRun;
}

void TcpServerPool::stopThread()
{
    if(m_p){
        m_p->release();
    }

    if(isRunning()){
        requestInterruption();
        quit();
        wait();
    }
}

void TcpServerPool::startThread()
{
    //lock;
    if(!isRunning())
        QThread::start();

    if( !m_isRun )
    {
        m_loop->exec();
    }
    //unlock;
}

void TcpServerPool::run()
{
    if( NULL==m_p )
    {
        m_p = new ServerPoolPrivate(this);

        connect( this, SIGNAL( startSvr(int) ), m_p, SLOT( startSvr(int) ) );
        connect( this, SIGNAL( stopSvr(int) ), m_p, SLOT( stopSvr(int)) );

        connect( m_p, SIGNAL( svrOn(int) ), this, SIGNAL( svrOn(int) ) );
        connect( m_p, SIGNAL( svrDown(int) ), this, SIGNAL( svrDown(int)) );

        //#ifdef D_USE_QT_SIGNALS
        //        connect( m_p, SIGNAL( tcpRead(const QByteArray& )), this, SIGNAL(readData(const QByteArray&)), Qt::QueuedConnection );
        //#else
        //        connect( m_p, SIGNAL( tcpRead(const QByteArray& )), this, SLOT(tcpRead(const QByteArray&)) );
        //#endif
        //        connect( this, SIGNAL( tcpWrite(const QByteArray& )), m_p, SLOT(tcpWrite(const QByteArray&) ), Qt::QueuedConnection  );
    }

    m_isRun=true;
    m_loop->quit();
    exec();
}
