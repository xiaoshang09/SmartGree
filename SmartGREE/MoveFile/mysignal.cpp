#include "mysignal.h"

MySignal::MySignal(QObject *parent) : QObject(parent)
{

}
void MySignal::test()
{
    emit Signal();
    //qDebug()<<"2";
}
