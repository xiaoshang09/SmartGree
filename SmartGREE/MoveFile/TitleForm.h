#ifndef TITLEFORM_H
#define TITLEFORM_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class TitleForm;
}

class TitleForm : public QWidget
{
    Q_OBJECT

public:
    explicit TitleForm(QWidget *parent = 0);
    ~TitleForm();

    enum ENUM_BtnType{
        //
        E_LOGO,
        E_TITLE,
        E_MSG,
        E_LOG,
        //
        E_STATE,
        E_SETTINGS,
        E_USR,
        E_HELP,
        //
        E_MIN,
        E_MAX,
        E_CLOSE,
    };

    enum ENUM_STATE{
        E_OK,
        E_NG,
        E_WARN,
    };

    void setLogo(const QPixmap &pix);
    void setTitle(const QString &str);
    void setMessage(const QString &str);
    void setWin(QWidget* win);
    void setState(int state, const QString&txt);
    void setUsr(const QString &str);

signals:
    void clicked(int type);//ENUM_BtnType

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);
    virtual void paintEvent(QPaintEvent *event);

private slots:
    void on_toolButton_min_clicked();
    void on_toolButton_max_clicked();
    void on_toolButton_close_clicked();

private:
    Ui::TitleForm *ui;
    QWidget* m_win;
    QButtonGroup btnBox;

};

#endif // TITLEFORM_H
