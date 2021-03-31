#include "helper.h"

#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>
#include <QScreen>
#include <QWidget>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QDateTime>

static QScreen *screen = QGuiApplication::primaryScreen();

Helper::Helper()
{

}

QPixmap Helper::GetPixmapSvg(const QString &icon, const QSize &size)
{
    QSvgRenderer svgRender;
    svgRender.load(icon);

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);//设置背景透明
    QPainter pter( &pixmap );
    svgRender.render(&pter);

    return std::move(pixmap);
}

void Helper::MoveCenter(QWidget *w, bool isResize)
{
    int deskWidth  = qApp->desktop()->availableGeometry().width();
    int deskHeight = qApp->desktop()->availableGeometry().height();
    if(isResize)
    {
        w->resize(deskWidth*0.6,deskHeight*0.6);
    }

//    qDebug()<<"test -=-222222-= "
//           << screen->availableSize()
//              << screen->physicalSize()
//                 ;
//    QRect screenRect =  screen->availableVirtualGeometry() ;
//    qDebug()<<"test -=-3333-= ";
////    w->move( (screenRect.width()-w->width())/2, (screenRect.height()-w->height())/2 );
//    qDebug()<<"test -=--= "
//           <<screenRect
//           << (screenRect.width()-w->width())/2
//           <<(screenRect.height()-w->height())/2
    //             ;
}

QWidget *Helper::GetCurrentPosWidget()
{
    return QApplication::widgetAt(QCursor::pos());//获取鼠标点击处的控件
}

