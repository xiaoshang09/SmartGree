#include "showtextmessage.h"

ShowTextMessage::ShowTextMessage(QString Qs, QTextBrowser * Qb)
{
    this->Qs=Qs;
    this->Qb=Qb;
    n= Qb->document()->blockCount();
}

void ShowTextMessage:: show()
{
    n= Qb->document()->blockCount();
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss");
     Qb->append(current_date+": "+Qs);
    if(n>49)
    {
    SaveLog();
    Qb->document()->clear();
    }
}

void ShowTextMessage::SaveLog()
{
         QString filePath =QString(LOG_PATH);
         //文件名
         QDir tempDir;
          //如果filePath路径不存在，创建它
         if(!tempDir.exists(filePath))
            {
                //qDebug()<<"不存在该路径"<<endl;
              tempDir.mkpath(filePath);
            }
         //QMessageBox::critical(0,"错误","文件打开失败，信息没有保存！","确定");
         QFile file(QString(LOG_PATH)+"/"+ QDateTime::currentDateTime().toString("yyyy-MM-dd")+".log");
         if(! file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
         {
             QMessageBox::critical(0,"错误","文件打开失败，信息没有保存！","确定");
             return;
         }
         QTextStream out(&file);//写入
         out << Qb->document()->toPlainText()+"\n";
         file.close();

}
