#ifndef PRODUCTNOWFORM_H
#define PRODUCTNOWFORM_H

#include <QWidget>
#include "MyWebView.h"

namespace Ui {
class ProductNowForm;
}

class MyWebView;
class ProductNowForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProductNowForm(QWidget *parent = 0);
    ~ProductNowForm();

    MyWebView* web(){return m_web;}

protected:
    virtual void timerEvent(QTimerEvent *event);

//signals:
//    void sigIdChanged(const QString& id);
//    void sigErrorHapend(const QString& msg);
//    void sigAddOutput(const QString& id, bool ok);

public slots:
    void doTriggerCamera(int type);
    void doChangeProgram(int mode);

    //切换产品
    void doIdChanged(const QString& id);

    //错误发生
    void doErrorHapend(const QString& msg);

    //产量+1
    void doAddOutput(const QString& id, bool ok, const QByteArray &pic);

    //已启动生产//功耗+
    void doProductStarted();

    //待机中//功耗-
    void doProductWating();

public slots:
    void updateDs();//统计图
    void updateTableError();//表格1
    void updateTableIdChanged();//表格2

private slots:
    void initWeb();
    void initTableIdChanged();
    void initTableErrorHapend();
    void loadFinished();

private:
    Ui::ProductNowForm *ui;
    MyWebView* m_web;
    bool isOK=false;
    QString m_id;
};

#endif // PRODUCTNOWFORM_H
