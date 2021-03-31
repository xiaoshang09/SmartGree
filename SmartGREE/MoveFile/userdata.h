#pragma once
#include <QString>
#include <QList>
#include <qdebug.h>
#include <QDataStream>

struct STUserData
{
private:
    bool mNull;
public:
    enum UserType {UserError=-1, Operator, Maintenance ,Management};
    UserType type;
    QString name;
    QString passwd;
    bool isRemember;
    STUserData():type(Operator),
        name(""),
        passwd(""),
        isRemember(false),
        mNull(true)
    {

    }
    STUserData(UserType type,
        const QString &name,
        const QString &passwd)
        :type(type),
        name(name),
        passwd(passwd),
        isRemember(false),
        mNull(false)
    {

    }
    STUserData(const STUserData &data)
    {
        if (&data)
        {
            this->type = data.type;
            this->name = data.name;
            this->passwd = data.passwd;
            this->isRemember = data.isRemember;
            this->mNull = data.mNull;
        }
    }
    STUserData operator = (const STUserData &data)
    {
        if (&data)
        {
            this->type = data.type;
            this->name = data.name;
            this->passwd = data.passwd;
            this->isRemember = data.isRemember;
            this->mNull = data.mNull;
            return *this;
        }
        return STUserData();
    }
    ~STUserData()
    {
        name.clear();
        passwd.clear();
    }

    bool operator == (const STUserData &data)
    {
        if (&data)
        {
            if (this->type == data.type &&
                this->name == data.name &&
                this->passwd == data.passwd)
            {
                return true;
            }
        }
        return false;
    }

    bool isNull(){return this->mNull;}
    static QString getTypeText(UserType type)
    {
        switch(type){
        case Operator:
            return "Operator";
        case Maintenance:
            return "Maintenance";
        case Management:
            return "Management";
        default:
            return "Operator";
        }
    }
};


inline QDataStream &operator>>(QDataStream &s,STUserData &data)
{
    int type;
    s >> type >> data.name >> data.passwd >> data.isRemember;
    data.type = STUserData::UserType(type);
    return s;
}

inline QDataStream &operator<<(QDataStream &s, const STUserData &data)
{
    s << int(data.type) << data.name << data.passwd << data.isRemember;
    return s;
}
inline QDebug operator<<(QDebug debug, const STUserData &data)
{
    debug << "STUserData("
          << data.type
          << data.name
          << data.passwd
          << data.isRemember
          << ")";

    return debug;
}
