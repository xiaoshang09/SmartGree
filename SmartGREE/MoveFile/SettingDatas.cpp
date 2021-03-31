#include "SettingDatas.h"
#include <QSettings>
#include <QByteArray>
#include <QDebug>
#include <QApplication>


SettingDatas::SettingDatas(QObject *parent)
    :QObject(parent)
{
    m_tmId = startTimer(2000);
}

void SettingDatas::setDefault(){
    qWarning()<<u8"warn:恢复默认设置";

    setSavePause(true);
    emit sigSetDefaultDir();//先初始化工程目录
    emit sigSetDefault();
    setSavePause(false);

    save();
}

bool SettingDatas::setValue(const char *key, const QVariant &val, bool save)
{
    if(value(key)==val){
        return false;//same
    }
    bool ret = setProperty(key,val);

    emit valueChanged(key);

    if(save && m_autoSave){
        saveValue(key, val);
    }
    return ret;
}

void SettingDatas::setAutoSave(bool sv)
{
    m_autoSave=sv;
}

void SettingDatas::setSavePause(bool sv)
{
    m_pause=sv;
}

void SettingDatas::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    if(m_save){
        m_save=false;
        qDebug()<<__FUNCTION__<<"test==";
        save();
    }
}

QString SettingDatas::creatWorkSpace()
{
    //读取内存,无数据的话,读取文件
    QString dir = value(DK_ProjectDir).toString();
    if(dir.simplified().isEmpty()){

        QSettings wsFile(D_FILE_WORKSPACE, QSettings::IniFormat);
        wsFile.setIniCodec(D_FILE_CODE);

        //文件也无数据的话,则初始化
        dir = wsFile.value(DK_ProjectDir).toString();
        if(dir.simplified().isEmpty()){

            setDefault();//是否需要初始化配置??

            dir = value(DK_ProjectDir).toString();

            wsFile.setValue(DK_ProjectDir, dir);
            qWarning()<<u8"warn:工程目录无效,已自动新建:"<< dir;
        }
    }

    return dir;
}

void SettingDatas::load()
{
    qDebug()<<__FUNCTION__ << "creatWorkSpace==";
    //配置文件
    QString dir=creatWorkSpace();

    QSettings iniFile( dir + D_FILE_SETTINGS, QSettings::IniFormat);
    iniFile.setIniCodec(D_FILE_CODE);

    //读取数据
    QStringList all = iniFile.allKeys();//从配置文件中读取所有的分组
    for(int i=0;i<all.size();i++){
        setProperty(all.at(i).toLatin1().constData(),iniFile.value(all.at(i)));

        qDebug()<<"load=="<<all.at(i) << iniFile.value(all.at(i));
    }

    emit loadFinished( );

    qDebug()<<__FUNCTION__ << ": settings load finished!";
}

void SettingDatas::save()
{
    //配置文件
    QString dir = creatWorkSpace();

    QSettings iniFile( dir + D_FILE_SETTINGS, QSettings::IniFormat);
    iniFile.setIniCodec(D_FILE_CODE);

    //    QList<QByteArray> nlist = dynamicPropertyNames();
    //
    QMap<const char*,QVariant> tmpMap;
    lock();
    std::swap(tmpMap, m_dataMap);
    unlock();

    //读取数据
    QMap<const char*,QVariant>::ConstIterator it = tmpMap.begin();
    while(it!=tmpMap.end())
    {
        QString tkey = it.key();
        iniFile.setValue(tkey.toLatin1().constData(), it.value());

        qDebug()<<"save=="<< dir + D_FILE_SETTINGS<<  tkey << it.value();

//        qDebug()<<"get=="<< iniFile.value( tkey);
        it++;
    }

}

void SettingDatas::saveValue(const char *key, const QVariant &val)
{
#if 0
    QString dir = value(DK_ProjectDir).toString();
    if(dir.isEmpty()){
        dir="./";
        setValue(DK_ProjectDir, "./");
    }

    //配置文件
    QSettings iniFile(D_FILE_SETTINGS, QSettings::IniFormat);//使用相对路径需要删除"项目==》shadow build选项"
    iniFile.setIniCodec(D_FILE_CODE);//在此添加设置，即可读写ini文件中的中文

    //写入数据
    iniFile.setValue(key, val);
#else

    lock();
    m_dataMap.insert( key, val );
    unlock();

    if(!m_pause)
        m_save = true;
#endif
}

