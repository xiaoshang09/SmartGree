#ifndef SETTINGDATAS_H
#define SETTINGDATAS_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QHash>
#include <QSettings>
#include <QMutex>

#include "DefinesGlobal.h"
#include "SettingKeys.h"

#define GetVal SettingDatas::instance()->value
#define SetVal SettingDatas::instance()->setValue
#define SetData(a,b) SettingDatas::instance()->setValue(a,b,false)

class SettingDatas : public QObject
{
    Q_OBJECT
public:
    SettingDatas(QObject* parent=nullptr);
    static SettingDatas* instance(){
        static SettingDatas* pobj=new SettingDatas();
        return pobj;
    }

    QVariant value(const char* key) const{
        return property(key);
    }
    //考虑到会有跨线程使用的情况,内部加锁了,尽量不要频繁(<5ms)写数据//save保存
    bool setValue(const char* key, const QVariant& val,bool save=true);

    QString creatWorkSpace();//创建工程目录件并返回其路径
    void setAutoSave(bool sv);//default:true
    void setSavePause(bool sv);//暂停保存,需要设置true之后才会保存
    void setDefault();//恢复默认配置
    void load();//初始化界面之后才能读取配置文件// 遇到要先有数据,后新建的怎么办?
    void save();

signals:
    void valueChanged(const char* key);//已修改
    void loadFinished();//初始化完毕
    void sigSetDefaultDir();//恢复工程文件夹
    void sigSetDefault();//恢复其他配置

protected:
    virtual void timerEvent(QTimerEvent *event);
    void saveValue(const char* key,const QVariant& val);
    inline void lock(){qlock.lock();}
    inline void unlock(){qlock.unlock();}

private:
    QMap<const char*,QVariant> m_dataMap;
    bool m_save=false;
    bool m_pause=false;
    int m_tmId=-1;
    bool m_autoSave=true;
    QMutex qlock;
};


#endif // SETTINGDATAS_H
