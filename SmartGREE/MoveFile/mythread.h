#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <qdebug.h>
#include <qstring.h>
#include <QObject>
#include "mysignal.h"
#include <QGuiApplication>

class MyThread : public QThread
{

public:
    MyThread();
    MySignal *Ms;
    void stop();
    void action();
    //bool stopped;

signals:
    //void NewSigal();
protected:
    void  run();
private:
    volatile bool stopped;

    QThread* th=new QThread();
};

#endif // MYTHREAD_H
