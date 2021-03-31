#ifndef SHOWTEXTMESSAGE_H
#define SHOWTEXTMESSAGE_H
#include <QMainWindow>
#include <QTextBrowser>
#include<QTime>
#include <QFile>
#include <QSettings>   //头文件加上它
#include <QFile>
#include "stdio.h"
#include<iostream>
#include<QMessageBox>

#include <QtCore/QCoreApplication>
#include<QFileInfo>
#include<QTextCodec>   //头文件加上它
#include<QStringList>
#include<QDateTime>
#include<QDebug>
#include<QObject>
#include<QTemporaryFile>
#include<QDir>


#define LOG_PATH            "./data/log"

class ShowTextMessage
{
public:
    ShowTextMessage(QString Qs, QTextBrowser * Qb);
    void show();
    void SaveLog();
private:
    QString Qs;
    QTextBrowser * Qb;
    int n=0;
};

#endif // SHOWTEXTMESSAGE_H
