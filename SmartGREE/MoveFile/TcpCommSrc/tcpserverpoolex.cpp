#include "tcpserverpoolex.h"
#include "tcpthread.h"
#include "tcpserverpool.h"


TcpServerPoolEx::TcpServerPoolEx(QObject *parent) : QObject(parent)
{

}

bool TcpServerPoolEx::hasData(int port)
{
    return (tcpBuf.value(port)->size()>0?true:false);
}

void TcpServerPoolEx::startSvr(int port)
{
    if(TcpServerPool::instance()->server(port)){
        return;
    }
    TcpServerPool::instance()->startThread( );
    TcpServerPool::instance()->startSvr( port );

    qlock.lock();
    QQueue<QByteArray>* bufList = tcpBuf.value(port);
    if(bufList){
        bufList->clear();
    }else{
        bufList = new QQueue<QByteArray>;
        tcpBuf.insert(port, bufList);
    }
    qlock.unlock();

    connect( TcpServerPool::instance(), &TcpServerPool::connected, this, [this, port](int _port){
        if(port == _port){//

//            TcpThread *client = TcpServerPool::instance()->client( port );
//            if( client ){
//                connect( client, SIGNAL(readData(const QByteArray&)), this, [this, port](const QByteArray& buf){
//                    if(buf.isEmpty())
//                        return;

//                    qlock.lock();
//                    tcpBuf.value( port )->append( buf );
//                    qlock.unlock();
//                } );
//            }
        }
    }
    );

    connect( TcpServerPool::instance(), &TcpServerPool::disconnected, this, [this,port](int _port){
        if(port == _port){
            TcpThread *client = TcpServerPool::instance()->client( port );
            if( client ){
                disconnect( client, SIGNAL(readData(const QByteArray&)), this,  0 );
            }

            qlock.lock();
            QQueue<QByteArray>* bufList = tcpBuf.value(port);
            if(bufList){
                bufList->clear();
            }
            qlock.unlock();
        }
    }
    );
}

void TcpServerPoolEx::stopSvr(int port)
{
    TcpServerPool::instance()->stopSvr( port );
}

void TcpServerPoolEx::readPkg(int port, QByteArray &buf)
{
    if( !hasData(port) )
        return;

    TcpThread *client = TcpServerPool::instance()->client( port );
    if( client ){
        qlock.lock();
        buf = tcpBuf.value(port)->dequeue();
        qlock.unlock();
    }
}

void TcpServerPoolEx::writePkg(int port, const QByteArray &pkg)
{
    if(pkg.isEmpty())
        return;

    TcpThread *client = TcpServerPool::instance()->client( port );
    if( client ){
        client->writeData( pkg );
    }
}




