#include "MyWebView.h"

#include <QDebug>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>



MyWebView::MyWebView(QWidget *parent) : QWebEngineView(parent)
{
    connect( this, SIGNAL(loadFinished(bool)), this , SLOT(loadFinished(bool)));
    page()->setBackgroundColor(QColor(45,45,45));
//    page()->setBackgroundColor(QColor(41,52,65));
}

void MyWebView::resizeEvent(QResizeEvent *event)
{
    QWebEngineView::resizeEvent(event);

    onResizeEcharts(this->width(), this->height());
}


void MyWebView::setData(int type,  const QJsonObject &jsObj)
{
    if(!m_loadFinished)
        return;

    switch(type){
    case E_Year:
        break;
    case E_Month:
        break;
    case E_Day:
        break;
    case E_Date:
        break;
    }

    QString optionStr =  QJsonDocument( jsObj ).toJson(QJsonDocument::Compact);
    QString jsText = QString("setData(%1)").arg(optionStr);

    page()->runJavaScript(jsText);
}

void MyWebView::loadFinished(bool ok)
{
    m_loadFinished = true;

    qDebug()<<__FUNCTION__<<ok;

    onResizeEcharts(this->width(), this->height());

//    onLoadData();

    //使对象在JavaScript框架下可用。对象将作为框架的窗口对象的一个子对象插入。
    //    this->page()->mainFrame()->addToJavaScriptWindowObject("PieEChartDlg",this);
}

void MyWebView::onResizeEcharts(int w, int h)
{
    QJsonObject sizeData;
    sizeData.insert("width", w-17);//有一个边框问题,必须砍掉17
    sizeData.insert("height", h-17);
    QString sizeStr = QJsonDocument(sizeData).toJson();
    QString js = QString("setSize(%1)").arg(sizeStr);
    page()->runJavaScript(js);
}

void MyWebView::onLoadData()
{
//    QJsonObject seriesData;
//    QJsonArray data1 = {1, 3, 9, 27, 81, 247, 741, 2223, 6669};
//    seriesData.insert("data1", data1);

//    QJsonArray data2 = {1, 82, 27, 8, 56, "32", 64, 128, 256};
//    seriesData.insert("data2", data2);

//    QString optionStr = QJsonDocument(seriesData).toJson();
//    QString js = QString("init2(%1)").arg(optionStr);
//    page()->runJavaScript(js);
}


