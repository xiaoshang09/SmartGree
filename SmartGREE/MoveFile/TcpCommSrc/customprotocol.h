#ifndef CUSTOMPROTOCOL_H
#define CUSTOMPROTOCOL_H

#include <QPointer>
#include <QByteArray>
#include "tcpthread.h"
#include "tcpserverpool.h"

#include "tcpcomm_global.h"
class TCPCOMMSHARED_EXPORT CustomProtocol : public QObject
{
    Q_OBJECT
public:
    CustomProtocol(QObject *parent = nullptr):QObject(parent){ startTimer(5); }
    virtual ~CustomProtocol(){ }
    //    virtual CustomProtocol* creat()=0;//return this;

public slots:
    /**
     * @brief start  启动服务
     * @param port
     */
    virtual void start( int port )=0;
    /* 例子
    virtual void start( int port ){
        TcpServerPool::instance()->startSvr( port );
        connect( TcpServerPool::instance(), &TcpServerPool::connected, [port](int _port){
            if(port == _port){
                setSocket( TcpServerPool::instance()->client( _port ) );
                connect( m_qsocket, SIGNAL(readData(const QByteArray&)), this, SLOT(readPkg(const QByteArray&)) );
            }
        }
        );

        connect( TcpServerPool::instance(), &TcpServerPool::disconnected, [port](int _port){
            if(port == _port){
                //TODO
            }
        }
        );
    }*/
    /**
     * @brief readPkg  解包
     * @param buf
     * @return //0=ok,-1=error
     */
    virtual int readPkg( const QByteArray& buf )=0;

    /**
     * @brief writePkg 发包
     * @param pkg
     * @return  //0=ok,-1=error
     */
    virtual int writePkg( const QByteArray&pkg ){
        if( m_qsocket ){
            m_qsocket->writeData(  pkg );
            return 0;
        }
        return -1;
    }

    /**
     * @brief timer_event  循环检测之类的事件
     */
//    virtual void timerEvent( )=0;
    virtual void timerEvent(QTimerEvent *event)=0;

protected:

public:
    void setSocket( TcpThread* qsocket ){
        m_qsocket = qsocket;
        //                connect( m_qsocket, SIGNAL(readData(const QByteArray&)), this, SLOT(readPkg(const QByteArray&)) );
    }

    QPointer<TcpThread> socket() const{
        return m_qsocket;
    }

protected:
    QPointer<TcpThread> m_qsocket;

};


#define TcpCommCustomProtocol_iid "org.qter.TcpComm.Plugin.CustomProtocol"
Q_DECLARE_INTERFACE( CustomProtocol, TcpCommCustomProtocol_iid )


class CustomProtocolFactory
{
public:
    //创建协议对象
    static CustomProtocol* create( const char* class_name );
    //    static CustomProtocol* create( TcpThread* socket, const char* class_name );
};


#define D_ByteCast(x) ((quint8) (x) & 0x00FF)
//字节转短整形数
static inline unsigned short bytes_to_uint16( quint8 *b ) {
    unsigned short ret;
    ret = (unsigned short)(D_ByteCast(b[0]));
    ret |= (unsigned short)(D_ByteCast(b[1])) << 8;
    return ret;
}
//字节转整形数
static inline quint32 bytes_to_uint32( quint8 *b ) {
    quint32 ret;
    ret = (quint32)(D_ByteCast(b[0]));
    ret |= (quint32)(D_ByteCast(b[1])) << 8;
    ret |= (quint32)(D_ByteCast(b[2])) << 16;
    ret |= (quint32)(D_ByteCast(b[3])) << 24;
    return ret;
}
//字节转长整形数
static inline quint64  bytes_to_uint64( quint8 *b ) {
    quint64 ret;
    ret = (quint64)(D_ByteCast(b[0]));
    ret |= (quint64)(D_ByteCast(b[1])) << 8;
    ret |= (quint64)(D_ByteCast(b[2])) << 16;
    ret |= (quint64)(D_ByteCast(b[3])) << 24;
    ret |= (quint64)(D_ByteCast(b[4])) << 32;
    ret |= (quint64)(D_ByteCast(b[5])) << 40;
    ret |= (quint64)(D_ByteCast(b[6])) << 48;
    ret |= (quint64)(D_ByteCast(b[7])) << 52;
    return ret;
}
//把64位转换成byte型数组
static inline void uint64_to_bytes( long long i, quint8 *b){
    b[0] = D_ByteCast(i);
    b[1] = D_ByteCast(i>>8);
    b[2] = D_ByteCast(i>>16);
    b[3] = D_ByteCast(i>>24);
    b[4] = D_ByteCast(i>>32);
    b[5] = D_ByteCast(i>>40);
    b[6] = D_ByteCast(i>>48);
    b[7] = D_ByteCast(i>>56);
}
//把32位转换成byte型数组
static inline void uint32_to_bytes( int i, quint8 *b) {
    b[0] = D_ByteCast(i);
    b[1] = D_ByteCast(i>>8);
    b[2] = D_ByteCast(i>>16);
    b[3] = D_ByteCast(i>>24);
}
//把16位转换成byte型数组
static inline void uint16_to_bytes( short i, quint8 *b) {
    b[0] = D_ByteCast(i);
    b[1] = D_ByteCast(i>>8);
}

static inline int number_to_buf( quint64 number, int bytelen , quint8*buf  )
{
    //        ret_buf=new char[8];
    //        unsigned char tbuf[8]={0};
    //   int dwRes=0;
    switch(bytelen){
    case 1:  buf[0] = number;
        break;
    case 2:  uint16_to_bytes( number, buf );
        break;
    case 4:  uint32_to_bytes( number, buf );
        break;
    case 8:  uint64_to_bytes( number, buf );
        break;
    default: printf("不支持的数据长度:bytelen=%d \n",bytelen);
        return 0;
    }
    return 1;
}
static inline int buf_to_number( quint8*buf ,  int bytelen ,  quint64 *number )
{
    switch(bytelen){
    case 1: *number = buf[0] ;
        break;
    case 2:  *number=bytes_to_uint16(  buf );
        break;
    case 4:  *number=bytes_to_uint32(  buf );
        break;
    case 8:  *number=bytes_to_uint64(  buf );
        break;
    default: printf("不支持的数据长度:bytelen=%d \n",bytelen);
        return 0;
    }
    return 1;
}

#ifndef INT_READ_BIT
#define INT_READ_BIT(DATA, POS) ((*((int *) (DATA)) >> (POS)) & 0x01)
#define INT_WRITE_BIT(DATA, POS, VAL) \
    do { \
    if (VAL) *((int *) (DATA)) |=  (1 << (POS)); \
    else     *((int *) (DATA)) &= ~(1 << (POS)); \
    } while (0)


#define INT_SET_BITFLAG(DATA, BITFLAG) \
{\
    int i=0;\
    while( i<32 ){\
    if( (BITFLAG>>i)&0x01 ){\
    INT_WRITE_BIT( DATA, i, 1);\
    break; \
    }\
    i++;\
    }\
}

#define INT_CLEAR_BITFLAG(DATA, BITFLAG) \
{\
    int i=0;\
    while( i<32 ){\
    if( (BITFLAG>>i)&0x01 ){\
    INT_WRITE_BIT( DATA, i, 0);\
    break; \
    }\
    i++;\
    }\
}

#endif

#endif // CUSTOMPROTOCOL_H
