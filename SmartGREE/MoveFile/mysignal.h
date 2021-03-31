#ifndef MYSIGNAL_H
#define MYSIGNAL_H

#include<QObject>
#include<qdebug.h>

class MySignal : public QObject
{
    Q_OBJECT
public:
    explicit MySignal(QObject *parent = nullptr);
    void test();
signals:
   void  Signal();
public slots:

};

#endif // MYSIGNAL_H
