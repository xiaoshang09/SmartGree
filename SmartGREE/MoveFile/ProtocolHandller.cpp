#include "ProtocolHandller.h"
#include "SettingDatas.h"

#include <QDebug>
#include <QTimer>


ProtocolHandller::ProtocolHandller(QObject *parent) : QObject(parent)
{
    _socket = new TcpThread();
    connect(_socket, &TcpThread::connected, [this]()
    {
        qDebug()<<__FUNCTION__<<u8"Plc Connected";
        SetData( DK_PlcState,true );
    });

    connect(_socket, &TcpThread::disconnected, [this]()
    {
        qDebug()<<__FUNCTION__<<u8"Plc Disconnected";
        SetData( DK_PlcState,false );
    });

    connect(_socket, &TcpThread::readData, [this](const QByteArray& buf)
    {
        //            QByteArray data = _socket->readAll();
//        qDebug()<<__FUNCTION__<<u8"Plc原始数据=="<<buf;
        onReceiveData(buf);
    });

    connect(SettingDatas::instance(), SIGNAL(valueChanged(const char *) ), this, SLOT(onValueChanged(const char *) ) );
    connect(SettingDatas::instance(), SIGNAL(loadFinished() ), this, SLOT(onLoad()));

    startTimer(5000);
}

void ProtocolHandller::start()
{
    QString ip=GetVal( DK_PlcIp ).toString();
    int port=GetVal( DK_PlcPort ).toInt();
    bool autoct = GetVal( DK_PlcAutoConet ).toBool();

    if(ip.isEmpty() || ip.split(".").size()!=4){
        qDebug()<< "error: plc ip not set!";
        SetData( DK_PlcState,false );
        return;
    }
    if(port<1000){
        qDebug()<< "error: port<1000!";
        SetData( DK_PlcState,false );
        return;
    }

    qDebug()<<__FUNCTION__<< "tttttttttttttttttt"<<ip<< port;

    _socket->setDisconnect();
    _socket->connectToServer(ip,port,autoct);
}

/**
 * @brief ProtocolHandller::getPLCStatus
 * 获取PLC状态
 */
void ProtocolHandller::getPLCStatus()
{
    _socket->writeData(INST_GET_PLC_STATUS);
    _socket->wait(10);
}

/**
 * @brief ProtocolHandller::getPLCDatas
 * 获取PLC数据
 */
void ProtocolHandller::getPLCDatas()
{
    _socket->writeData(INST_GET_PLC_DATAS);
    _socket->wait(10);
}

/**
 * @brief ProtocolHandller::getRobotPositon
 * 获取机器人位置
 */
void ProtocolHandller::getRobotPositon()
{
    _socket->writeData(INST_GET_ROBOT_POS);
    _socket->wait(10);
}

void ProtocolHandller::timerEvent(QTimerEvent *event)
{
    if(GetVal( DK_PlcState ).toBool()){
          getPLCStatus();      //读取PLC状态
          getPLCDatas();       //读取PLC生产数据
//          getRobotPositon();   //获取PLC机器人位置
    }

#if 0 //D_TEST
    static int count = 0 ;
    if(count%3 == 0 ){
        emit triggerCamera(EM_TRIGGER_CAMERA_TITO);
    }

    if(count%3 == 2 ){
        emit triggerCamera(EM_TRIGGER_CAMERA_METAL);
    }
#endif
}

//工位方向//泡沫
int ProtocolHandller::sendToPlc_check2D1Dir(bool ok,int dir)
{
    qDebug()<<__FUNCTION__<<"test==="<< ok << dir;
    QString retStr ;
    if(dir){
        retStr="11";//
        //pos
    }else{
        retStr="10";
        //转向
    }
    if(!ok){
        retStr="12";
    }

    QString cmd = QString("0220,%1,\\r\\n").arg(retStr);

    QByteArray buf;
    buf.append( cmd );
    _socket->writeData( buf );
    _socket->wait(10);

    return 0;
}

//铁板
int ProtocolHandller::sendToPlc_check2D2pos(bool ok, int dir, float x, float y, float z, float Rx, float Ry, float Rz)
{
    qDebug()<<__FUNCTION__<<"test==="<< ok << dir;

    QString retStr ;
    if(dir){
        retStr="21";
    }else{
        retStr="20";
    }
    if(!ok){
        retStr="22";
    }

    QByteArray buf;
    buf.append( QString("0221,%0,%1,%2,%3,%4,%5,%6,\\r\\n").arg( retStr )
                .arg(x).arg(y).arg(z).arg(Rx).arg(Ry).arg(Rz) );
    _socket->writeData( buf );
    _socket->wait(10);

    return 0;
}

//最后定位
int ProtocolHandller::sendToPlc_check2D1pos(bool ok, int dir)
{
    qDebug()<<__FUNCTION__<<"test==="<< ok << dir;

    QString retStr ;
    if(dir){
        retStr="21";
    }else{
        retStr="20";
    }
    if(!ok){
        retStr="22";
    }

    QString cmd = QString("0221,%1,\\r\\n").arg(retStr);

    QByteArray buf;
    buf.append( cmd );
    _socket->writeData( buf );
    _socket->wait(10);

    return 0;
}

void ProtocolHandller::onValueChanged(const char *key)
{
    if(DK_COMP(key,DK_PlcIp) || DK_COMP(key,DK_PlcPort) ){

        start();
    }
}

void ProtocolHandller::onLoad()
{
    start();
}

void ProtocolHandller::onReceiveData(const QByteArray &data)
{
    QString dataStr(data.data());

    //粘包处理
    QStringList datas = dataStr.split(",",QString::SkipEmptyParts);


    QStringList cleanData;                 //去掉空格
    for(int i = 0; i<datas.size(); ++i)
    {
        QString item = datas.at(i);
        cleanData.push_back(item.simplified());
    }

    QString head      = cleanData.at(0);
//    auto endFlag      = datas.last();


    //PLC反馈 get (我读PLC)
    if(!head.compare(RES_PLC_STATUS))      //"0103"  PLC状态
    {
        parsePLCStatus(cleanData);
        return;
    }
    else if(!head.compare(RES_PLC_DATAS))  //"0104"  解析PLC数据
    {
        parsePLCDatas(cleanData);
        return;
    }
    else if(!head.compare(RES_ROBOT_POS))  //"0105"  机器人位置
    {
        parseRobotPosition(cleanData);
        return;
    }


    qDebug()<<__FUNCTION__<<u8"PLC请求:Plc原始数据=="<<data;
    //PLC请求 set (PLC主动发给我的)
    if(!dataStr.compare(REQ_TRIGGER_CAMERA_TITO))       //"0220,\r\n"     触发相机拍照，底托
    {
        qDebug()<<__FUNCTION__<<u8"触发相机拍照--底托";
        emit triggerCamera(EM_TRIGGER_CAMERA_TITO);

        //返回指令：0220, xx,\r\n
#ifdef D_TEST
//        QThread::msleep(100);
//        _socket->writeData( "0220, 10,\\r\\n" );
//        _socket->wait(10);
#endif
    }
    else if(!dataStr.compare(REQ_TRIGGER_CAMERA_METAL)) //"0221,\r\n"    触发相机拍照，钣金
    {
        qDebug()<<__FUNCTION__<<u8"触发相机拍照--钣金";
        emit triggerCamera(EM_TRIGGER_CAMERA_METAL);

        //返回指令：0221, xx,X,Y,Z,A,B,C ,\r\n
#ifdef D_TEST
//        QThread::msleep(100);
//        QByteArray buf;
//        buf.append( QString("0221,%1,%2,\\r\\n").arg("10").arg("100,101,102,01,02,03 ") );
//        _socket->writeData( buf );
#endif
    }
    else if(!head.compare(REQ_CHANGE_PROGRAM))       //"0222
    {
        qDebug()<<__FUNCTION__<<u8"设置程序";
        parseChangeProgram(cleanData);

        //返回指令：0222, xxxx, \r\n
#ifdef D_TEST
//        QThread::msleep(100);
//        QByteArray buf;
//        buf.append( QString("0222,%1,\\r\\n").arg("N123456") );
//        _socket->writeData( buf );
//        _socket->wait(10);
#endif
    }
    else{
        qDebug()<<u8"PLC协议解释失败:"<<dataStr<< cleanData;
    }
}

/**
 * @brief ProtocolHandller::parsePLCStatus
 * 返回指令：0103, XXXXXXXX , \r\n
 *          0103, FFFFA,     \r\n
 * @param datas
 */
void ProtocolHandller::parsePLCStatus(const QStringList &datas)
{
    if(datas.size() != 3)
        return;

    //解析PLC状态
    QString data = "0x"+datas.at(1);


    bool res;
    unsigned int value = data.toUInt(&res, 16);


    if(res)
    {
        if(value!=m_status.state){
            m_status.state=value;

            qDebug()<<__FUNCTION__<<u8"解析PLC状态 value=="<<data<<value<<res;
            SetData( DK_StateSTS, m_status.state );
        }
    }else{

        qDebug()<<__FUNCTION__<<u8"ERROR:解析PLC状态 错误!";
    }
}

/**
 * @brief ProtocolHandller::parsePLCDatas
 * 返回指令：0104, 空格 32位数据, 空格 32位数据 空格 ,\r\n
            0140, B2F1CFA1,     B2F1CFA2          ,\r\n
            DATA1：当前产能（STRING）
            DATA2: 生产节拍(STRING)
 * @param datas
 */
void ProtocolHandller::parsePLCDatas(const QStringList &datas)
{
    if(datas.size() != 4)
        return;

    //当前产能
    QString capacity = datas.at(1);
    //生产节拍
    QString taktTime = datas.at(2);

//    qDebug()<<__FUNCTION__<<u8"解析PLC数据"<<capacity<<"--"<<taktTime;
}

/**
 * @brief ProtocolHandller::parseRobotPosition
 * 返回指令：0105,  X,    Y,     Z,    A,      B,    C,   \r\n
 * 返回指令  0105,-145.22,60.21,70.55,-82.22,-55.12,22.31,\r\n
 * @param datas
 */
void ProtocolHandller::parseRobotPosition(const QStringList &datas)
{
    if(datas.size() != 8)
        return;

//    m_robotData.name = datas.at(1).toDouble();
    m_robotData.X = datas.at(1).toDouble();
    m_robotData.Y = datas.at(2).toDouble();
    m_robotData.Z = datas.at(3).toDouble();

    m_robotData.Rx = datas.at(4).toDouble();
    m_robotData.Ry = datas.at(5).toDouble();
    m_robotData.Rz = datas.at(6).toDouble();

    qDebug()<<__FUNCTION__<<u8"解析PLC机器人数据 TODO"<<datas ;

    emit robotStatus(m_robotData. name,  m_robotData. X, m_robotData.Y, m_robotData. Z, m_robotData. Rx, m_robotData. Ry, m_robotData. Rz);
}

/**
 * @brief ProtocolHandller::parseChangeProgram
 * 0222, xxxx, \r\n
 * 0222,空格 1234,\r\n 黄色是程序号（16进制）
 * @param datas
 */
void ProtocolHandller::parseChangeProgram(const QStringList &datas)
{
    if(datas.size() != 3)
        return;

    QString programId = datas.at(1);

    qDebug()<<__FUNCTION__<<u8"改变程序"<<programId;

    bool ok;
    int value_hex = programId.toInt(&ok, 16);
    if(ok){
        emit changeProgram(value_hex);
    }

}
