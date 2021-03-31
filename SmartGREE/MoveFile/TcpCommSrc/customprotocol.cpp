#include "customprotocol.h"
#include "crc32.h"
#include "TcpThread.h"

#include <QDebug>
#include <QByteArray>
#include <QDataStream>


CustomProtocol* CustomProtocolFactory::create( const char* class_name )
{
    int id = QMetaType::type( class_name );
    if (id != -1) {
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        CustomProtocol *parser = static_cast< CustomProtocol* > ( QMetaType::construct( id ) );
#else
        CustomProtocol *parser = static_cast< CustomProtocol* > ( QMetaType::create( id ) );
#endif
        return parser;
    }
    return NULL;
}

//CustomProtocol* CustomProtocolFactory::create( TcpThread*socket, const char* class_name )
//{
//    int id = QMetaType::type( class_name );
//    if (id != -1) {
//#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
//        CustomProtocol *parser = static_cast< CustomProtocol* > ( QMetaType::construct( id ) );
//#else
//        CustomProtocol *parser = static_cast< CustomProtocol* > ( QMetaType::create( id ) );
//#endif
//        parser->setSocket( socket );
//        return parser;
//    }
//    return NULL;
//}


//CustomProtocol::CustomProtocol()
//{
//}

//CustomProtocol::~CustomProtocol(){
//}

//int CustomProtocol::readCmd( const QByteArray &buf )
//{
//    qDebug()<<"virtual decompressionData========";
//    return 1;
//}

//int  CustomProtocol::writePkg(const QByteArray &pkg )
//{
//    if( m_qsocket ){
//        m_qsocket->writeData(  pkg );
//        return 0;
//    }
//    return -1;
//}

//void CustomProtocol::timer_event(){

//}





