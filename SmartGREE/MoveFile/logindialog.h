#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QButtonGroup>
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    static LoginDialog* instance(){
        static LoginDialog* obj=new LoginDialog();
        return obj;
    }
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    //STUserData::UserType::
    int loginType(){return m_loginType;}
protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void sig_login_return(bool isok);//登录

public slots:
    void on_pushButton_login_clicked();//登录
    void on_pushButton_confirm_clicked();//修改密码
    void on_pushButton_returnLogin_clicked();//修改返回

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void setStyle();

private:
    Ui::LoginDialog *ui;
    QButtonGroup* btGroup;
    void changeSelectWidgetStyle();
    void loadLastLoginInfo();
    void setLoginError(bool isError,const QString &text);
    void setChangePwError(bool isError, const QString &text);
    int m_loginType;
};

#endif // LOGINDIALOG_H
