#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QSplashScreen>
#include <QPixmap>
#include <QLabel>

namespace Ui {
class SplashScreen;
}

class SplashScreen : public QSplashScreen
{
    Q_OBJECT

public:
    static SplashScreen* instance(){
        if(m_obj == NULL){
            m_obj = new SplashScreen();
        }
        return m_obj;
    }

    explicit SplashScreen(const QPixmap &pixmap = QPixmap(), Qt::WindowFlags f = Qt::WindowFlags());
    ~SplashScreen();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

public slots:
    void progressBarAdd(int val);//添加进度,当进度>100时隐藏此界面
    void progressBarClear( );
    void setText(const QString& str);

private:
    Ui::SplashScreen *ui;
    static SplashScreen* m_obj;
    int m_progress=0;
};

#endif // SPLASHSCREEN_H
