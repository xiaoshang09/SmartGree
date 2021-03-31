#include "LogWidget.h"
#include "ui_LogWidget.h"

#include "log4qt.h"
#include "LogWidgetAppender.h"
#include "ttcclayout.h"
#include "basicconfigurator.h"
#include "logmanager.h"
#include "databaseappender.h"
#include "dailyrollingfileappender.h"
#include "SettingDatas.h"


#include <QDateTime>
#include <QVBoxLayout>
#include <QTextCodec>
#include <QMessageBox>
#include <QFile>
#include <QDataStream>
#include <QPlainTextEdit>
#include <QTextEdit>



LogWidget::LogWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWidget)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());
}

LogWidget::~LogWidget()
{
    delete ui;
}

void LogWidget::showEvent(QShowEvent *event){
    QWidget::showEvent(event);

//    on_pushButton_now_clicked();
}

void LogWidget::onAppendLog(const QString &log)
{
    //TODO
        ui->plainTextEdit->insertPlainText(log);
}

void LogWidget::showFileTxt(const QString &fileTxt)
{

    qDebug()<<"show log:"<< fileTxt;

    QFile file;
    file.setFileName(fileTxt);
    bool ok=file.open(QIODevice::ReadOnly);
    if(ok)
    {
        QTextStream in(&file);
        ui->plainTextEdit->setPlainText(in.readAll());
        file.close();

        QTextCursor cursor=ui->plainTextEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->plainTextEdit->setTextCursor(cursor);
    }
    else
    {
        QMessageBox::information(this,"Error Message","Open File:"+file.errorString());
        return;
    }
}

void LogWidget::on_pushButton_now_clicked()
{
#if 0
    ui->plainTextEdit->clear();
#else
    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString dir = "./log/" + QDateTime::currentDateTime().toString("yyyy") + "/";

    showFileTxt( dir + date + ".log" );
#endif
}

void LogWidget::on_pushButton_check_clicked()
{
    QString date = ui->dateEdit->text();
    QString dir = "./log/" + QString::number(ui->dateEdit->date().year()) + "/";

    showFileTxt( dir + date + ".log" );
}

void LogWidget::on_pushButton_fiter_clicked()
{
    QString search_text = ui->lineEdit->text();
    if (search_text.trimmed().isEmpty()) {
        QMessageBox::information(this, tr("Empty search field"), tr("The search field is empty."));
    } else {
        QTextDocument *document = ui->plainTextEdit->document();
        bool found = false;
        QTextCursor highlight_cursor(document);
        QTextCursor cursor(document);



        //        cursor.beginEditBlock();
        //        QTextCharFormat default_format(highlight_cursor.charFormat());
        //        default_format.setForeground(QColor(240,240,240));
        //        cursor.mergeCharFormat(default_format);
        //        default_format.sel


        //开始
        cursor.beginEditBlock();
        QTextCharFormat color_format(highlight_cursor.charFormat());
        color_format.setForeground(Qt::red);
        while (!highlight_cursor.isNull() && !highlight_cursor.atEnd()) {
            //查找指定的文本，匹配整个单词
            highlight_cursor = document->find(search_text, highlight_cursor);
            if (!highlight_cursor.isNull()) {
                if(!found)
                    found = true;

                highlight_cursor.mergeCharFormat(color_format);
            }
        }
        cursor.endEditBlock();
        //结束

        if (found == false) {
            //            QMessageBox::information(this, tr("Word not found"), tr("Sorry,the word cannot be found."));

            qDebug()<<"log search: word not found!";

            //重置
            cursor.select(QTextCursor::Document);
            cursor.setCharFormat(QTextCharFormat());
            cursor.clearSelection();
            ui->plainTextEdit->setTextCursor(cursor);
        }
    }
}

void LogWidget::on_lineEdit_returnPressed()
{
    on_pushButton_fiter_clicked();
}
