#ifndef UIHELPER_H
#define UIHELPER_H

#include <QPoint>
#include <QApplication>
#include <QDesktopWidget>
namespace UiLayer {

//窗体居中显示
static void moveFormToCenter(QWidget *widget,bool isResize = false)
{
    int deskWidth = qApp->desktop()->availableGeometry().width();
    int deskHeight = qApp->desktop()->availableGeometry().height();
    if(isResize)
    {
        widget->resize(deskWidth/3,deskHeight/3);
    }

    int frmX = widget->width();
    int frmY = widget->height();
    QPoint movePoint(deskWidth / 2 - frmX / 2,
                     deskHeight / 2 - frmY / 2);

    widget->move(movePoint);
}
}
#endif // UIHELPER_H
