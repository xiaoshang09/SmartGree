#include "SplashScreen.h"
#include "ui_splashscreen.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QScreen>
#include "helper.h"

SplashScreen* SplashScreen::m_obj=NULL;
SplashScreen::SplashScreen(const QPixmap &pixmap, Qt::WindowFlags f) :
    QSplashScreen(pixmap,f),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);

    ui->label_logo->setPixmap( Helper::GetPixmapSvg(":image/gree.svg", QSize(400,180) ) );
    ui->label_title->setText( u8"让世界爱上中国制造");
    ui->label_version->setText( u8"软件版本信息: " + Helper::GetVersion()  );
}

SplashScreen::~SplashScreen()
{
    delete ui;
}

void SplashScreen::paintEvent(QPaintEvent *event)
{
//    QScreen *screen = QGuiApplication::primaryScreen ();
//    QRect screenRect =  screen->availableVirtualGeometry();
//    //    resize(screenRect.width(),screenRect.height());

//    QPainter painter(this);
////    QRect rect = pixmap().rect();
////    rect.adjusted( );
////    painter.drawPixmap( rect.x() ,pixmap());

//    int type = 1;

//    if (pixmap().isNull())
//    {
//        ui->label->setStyleSheet("background-color:gray");
//    }
//    else
//    {
//        switch (type)
//        {
//            case 0://居中:
//                ui->label->setStyleSheet("background-color:black;background-image:url(1.jpg);background-position:center;background-repeat:no-repeat;");
//                break;

//            case 1://缩放:
//                ui->label->setStyleSheet("background-color:black");
//                ui->label->setAlignment(Qt::AlignCenter);
//                ui->label->setPixmap(pixmap().scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
//                break;

//            case 2://拉伸:
//                ui->label->setStyleSheet("background-color:black");
//                ui->label->setPixmap(pixmap().scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
//                break;

//            case 3://平铺:
//                ui->label->setStyleSheet("background-color:black;background-image:url(1.jpg);background-position:top left;background-repeat:repeat-xy;");
//                break;
//        }
//    }

}

void SplashScreen::mousePressEvent(QMouseEvent *event)
{
    //重写//防止点击时消失
}

void SplashScreen::progressBarAdd(int val)
{
    m_progress += val ;
    if(m_progress>100){
        m_progress=100;
        this->hide();
    }
    ui->progressBar->setValue(m_progress);
}

void SplashScreen::progressBarClear()
{
    m_progress=0;
    ui->progressBar->setValue(0);
}

void SplashScreen::setText(const QString &str)
{
    ui->label_progress_msg->setText( str );
}
