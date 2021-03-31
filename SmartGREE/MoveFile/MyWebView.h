#ifndef MYWEBVIEW_H
#define MYWEBVIEW_H

#include <QWebEngineView>

#include "SqlFunc.h"


#include "DataStatistics_global.h"
class DATASTATISTICSSHARED_EXPORT  MyWebView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit MyWebView(QWidget *parent = nullptr);

    enum ENUM_ShowType{
        E_Year,
        E_Month,
        E_Week,
        E_Day,
        E_Date,
    };
protected:
    virtual void resizeEvent(QResizeEvent *event);

signals:

public slots:
    void setData(int type, const QJsonObject& dat);

private slots:
    void loadFinished(bool ok);
    void onResizeEcharts(int w, int h);
    void onLoadData();

private:
    bool m_loadFinished=false;

};

#endif // MYWEBVIEW_H
