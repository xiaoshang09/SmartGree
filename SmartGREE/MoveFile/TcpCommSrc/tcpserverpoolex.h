#ifndef TCPSERVERPOOLEX_H
#define TCPSERVERPOOLEX_H

#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QMap>

#include "tcpcomm_global.h"
class TCPCOMMSHARED_EXPORT TcpServerPoolEx : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief TcpServerPoolEx ,封装TcpServerPool, 将Tcp收到的数据缓存起来,等想用的时候用readPkg()接口取出来 (注意:一直不取会导致缓存很大)
     * @param parent
     */
    explicit TcpServerPoolEx(QObject *parent = nullptr);
    static TcpServerPoolEx* instance(){
        static TcpServerPoolEx* obj = new TcpServerPoolEx();
        return obj;
    }

public :
    virtual void startSvr( int port );
    virtual void stopSvr( int port );

public :
    virtual bool hasData(int port);
    virtual void readPkg(int port, QByteArray &buf );
    virtual void writePkg( int port, const QByteArray&pkg );

public:
    QMap<int, QQueue<QByteArray>* > tcpBuf;
    QMutex qlock;
};


#endif // TCPSERVERPOOLEX_H
