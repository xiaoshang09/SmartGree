#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include <qmap.h>
#include "userdata.h"
#define UserManagerPtr UserManager::instance()
typedef QList<STUserData> STUserDataList;
class QMutex;
class UserManager : public QObject
{
    Q_OBJECT

public:
    ~UserManager();
    static UserManager *instance();
    bool login(STUserData data);
    bool addUser(STUserData data);
    bool removeUser(STUserData data);
    bool modifyUser(STUserData oldUser,STUserData newUser);
    STUserData queryTheUser(STUserData::UserType type,
                            const QString &name);
    bool isHasTheUser(STUserData data);
    void setCurLoginUser(STUserData data);
    STUserData curLoginUser() const;

    void save();
    void load();
    int userCount();
private:
    explicit UserManager(QObject *parent = nullptr);
    UserManager(const UserManager &p) = delete;
private:
    static UserManager *mPtr;
    static QMutex *mMutex;
    STUserData mCurLoginUser;
    QMap<STUserData::UserType,STUserDataList> mUserMap;
};

#endif // USERMANAGER_H
