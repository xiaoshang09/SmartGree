#ifndef TCPCONFIG_H
#define TCPCONFIG_H

#include <QSettings>

/**
 * @brief The TcpConfig class 用来配置服务器
 */
class TcpConfig
{
public:
    TcpConfig();


     void readConfig(){
         QSettings *configIni = new QSettings("para.ini", QSettings::IniFormat);
         int CurrentTemplateIndex=2;
         int Finda=10;
         configIni->setValue( "x/m" ,Finda);
         int Findb=2;
         configIni->setValue( "x/n" ,Findb);
    }
};

#endif // TCPCONFIG_H
