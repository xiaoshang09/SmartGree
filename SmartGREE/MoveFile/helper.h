#ifndef HELPER_H
#define HELPER_H

#include <QPixmap>
#include <QOBject>


class Helper
{
public:
    Helper();
    static QPixmap GetPixmapSvg(const QString &icon,const QSize &size);
    static QString GetVersion(){ return "V1.0.0"; }
    static void MoveCenter(QWidget* w, bool isResize=false);
    static QWidget *GetCurrentPosWidget();//获取当前鼠标对应的控件
};

#endif // HELPER_H
