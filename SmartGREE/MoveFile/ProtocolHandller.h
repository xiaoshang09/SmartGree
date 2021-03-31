#ifndef PROTOCOLHANDLLER_H
#define PROTOCOLHANDLLER_H

#include <QObject>
#include "InstructionCode.h"
#include "tcpthread.h"
#include "SettingDatas.h"

using namespace InstructionCode;

class ProtocolHandller : public QObject
{
    Q_OBJECT
public:
    static ProtocolHandller* instance(){static ProtocolHandller*pobj=new ProtocolHandller(); return pobj;}
    explicit ProtocolHandller(QObject *parent = nullptr);

    void start(); //连接PLC
    void getPLCStatus();      //读取PLC状态
    void getPLCDatas();       //读取PLC生产数据
    void getRobotPositon();   //获取PLC机器人位置

protected:
    virtual void timerEvent(QTimerEvent *event);

signals:
    void triggerCamera(int);
    void changeProgram(int);//切换PLC程序
    void robotStatus(const QString& robot,float X, float Y, float Z, float Rx, float Ry, float Rz);

public slots:
    int sendToPlc_check2D1Dir(bool ok, int dir);
    int sendToPlc_check2D2pos(bool ok,int dir,float x,float y,float z,float Rx,float Ry,float Rz);
    int sendToPlc_check2D1pos(bool ok,int dir);

private slots:
    void onValueChanged(const char *key);
    void onLoad();

private:
    inline void onReceiveData(const QByteArray& data);
    inline void parsePLCStatus(const QStringList &datas);
    inline void parsePLCDatas(const QStringList &datas);
    inline void parseRobotPosition(const QStringList &datas);
    inline void parseChangeProgram(const QStringList &datas);

private:
    TcpThread* _socket;
    int m_tmId=-1;
    RobotStatus m_robotData;//缓存
    U_PLCStatus m_status;

};

#endif // PROTOCOLHANDLLER_H
