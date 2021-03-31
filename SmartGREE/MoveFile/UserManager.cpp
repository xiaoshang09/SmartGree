#include "UserManager.h"
//
#include "QApplication"
#include <qdebug.h>
#include <QMutex>
#include <QMutexLocker>
#include <QtCore>

UserManager* UserManager::mPtr = nullptr;
QMutex *UserManager::mMutex = new QMutex();

UserManager::UserManager(QObject *parent)
    : QObject(parent)
{
    this->load();

    addUser(STUserData(STUserData::Operator, "gree", "123456"));
    addUser(STUserData(STUserData::Maintenance, "greem", "123456"));
    addUser(STUserData(STUserData::Management, "admin", "000000"));

}

void UserManager::setCurLoginUser(STUserData data)
{
    mCurLoginUser = data;

//    //System->setSysParam("CurLoginType", int(data.type));
//    //System->setSysParam("CurLoginName", data.name,false);
//    //System->setSysParam("CurLoginPasswd", data.passwd,false);
//    //System->setSysParam("CurLoginRemember", data.isRemember);

    qApp->setProperty("CurLoginType", int(data.type));
    qApp->setProperty("CurLoginName", data.name);
    qApp->setProperty("CurLoginPasswd", data.passwd);
    qApp->setProperty("CurLoginRemember", data.isRemember);
}

STUserData UserManager::curLoginUser() const
{
    return mCurLoginUser;
}

void UserManager::save()
{
    if(mUserMap.isEmpty()){
        qDebug()<<__FUNCTION__<<"user is empty!";
        return;
    }

    QDir dir(qApp->applicationDirPath() + "/config/");
    dir.mkpath(qApp->applicationDirPath() + "/config/");

    auto path = qApp->applicationDirPath() + "/config/user.dat";
    QFile file(path);
    if(file.open(QFile::WriteOnly))
    {
        QDataStream stream(&file);
        stream << mCurLoginUser;

        stream << userCount();
        auto values = mUserMap.values();
        for (int i = 0; i < values.size(); ++i)
        {
            auto list = values.at(i);
            for (int j = 0; j < list.size(); ++j)
            {
                auto user = list.at(j);
                stream << user;
            }
        }

        file.close();
    }
}

void UserManager::load()
{
    auto path = qApp->applicationDirPath() + "/config/user.dat";
    if(!QFile::exists(path)){
        qDebug()<<__FUNCTION__<<"file not exists!";
        return;
    }

    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        QDataStream stream(&file);
        stream >> mCurLoginUser;

        qDebug()<<"mCurLoginUser=="<<mCurLoginUser;

        this->setCurLoginUser(mCurLoginUser);

        int userCount = 0;
        stream >> userCount;
        for (int i = 0; i < userCount; ++i)
        {
            STUserData user;
            stream >> user;
            this->addUser(user);
        }

        file.close();
    }
}

int UserManager::userCount()
{
    int count = 0;
    auto values = mUserMap.values();
    for (int i = 0; i < values.size(); ++i)
    {
        auto list = values.at(i);
        count += list.size();
    }

    return count;
}

UserManager::~UserManager()
{
    if(mMutex)
        delete mMutex;
}

bool UserManager::login(STUserData data)
{
    if(data.name.isEmpty() || data.passwd.isEmpty()) return false;

    STUserData temp = queryTheUser(data.type,data.name);
    if (temp == data)
    {
        qDebug() << tr("%1 登录系统").arg(data.name);
        setCurLoginUser(data);
        this->save();
        return true;
    }
    return false;
}


UserManager *UserManager::instance()
{
    QMutexLocker locker(mMutex);
    if(mPtr == nullptr)
        mPtr = new UserManager();
    return mPtr;
}

bool UserManager::addUser(STUserData data)
{
    auto isok = isHasTheUser(data);
    if(!isok)
    {
        if(mUserMap.contains(data.type))
        {
            auto list = mUserMap.value(data.type);
            list << data;
            mUserMap.insert(data.type,list);
            this->save();
            return true;
        }else
        {
            STUserDataList list;
            list << data;
            mUserMap.insert(data.type,list);
            this->save();
            return true;
        }
    }

    return false;
}
bool UserManager::removeUser(STUserData data)
{
    if(mUserMap.contains(data.type))
    {
        auto list = mUserMap.value(data.type);
        foreach (auto user, list) {
            if(user.name == data.name)
            {
                list.removeOne(user);
            }
        }
        mUserMap.insert(data.type,list);
        this->save();
        return true;
    }

    return false;
}

bool UserManager::modifyUser(STUserData oldUser, STUserData newUser)
{
    if(mUserMap.contains(oldUser.type))
    {
        auto list = mUserMap.value(oldUser.type);
        for (int i = 0; i < list.size(); ++i)
        {
            auto user = list.at(i);
            if(user.name == oldUser.name)
            {
                list.replace(i,newUser);
            }
        }
        mUserMap.insert(oldUser.type,list);
        this->save();
        return true;
    }

    return false;
}

STUserData UserManager::queryTheUser(STUserData::UserType type, const QString &name)
{
    if(mUserMap.contains(type))
    {
        auto list = mUserMap.value(type);
        foreach (auto user, list) {
            if(user.name == name)
            {
                return user;
            }
        }
    }

    return STUserData();
}

bool UserManager::isHasTheUser(STUserData data)
{
    if(mUserMap.contains(data.type))
    {
        auto list = mUserMap.value(data.type);
        foreach (auto user, list) {
            if(user.name == data.name){
                return true;
            }
        }
    }

    return false;
}

