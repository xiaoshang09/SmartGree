#include "mythread.h"

MyThread::MyThread()
{
    stopped=false;
    Ms=new MySignal();
    //connect(this,SIGNAL(run()))
}
void MyThread::action()
{
    //emit NewSigal();

}
void MyThread:: run()
{
    //qreal i=0;

    while (!stopped) {
        //qDebug()<<"1";
        Ms->test();
         th->msleep(200);
        //刷新界面
        qApp->processEvents();
      //emit NewSigal();

    }
    stopped=false;

    //exec();
}

void  MyThread:: stop()
{

    stopped=true;

}
