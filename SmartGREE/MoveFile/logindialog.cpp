#include "logindialog.h"
#include "ui_logindialog.h"
#include "UserManager.h"
#include <QtWidgets>
#include <QRadioButton>
#include <QKeyEvent>

//
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    btGroup = new QButtonGroup( this );
    btGroup->addButton(ui->radioButton_operator, STUserData::Operator);
    btGroup->addButton(ui->radioButton_maintenance, STUserData::Maintenance);
    btGroup->addButton(ui->radioButton_management, STUserData::Management);
    ui->radioButton_operator->setChecked(true);

    UserManager::instance();

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle(tr(u8"登录系统"));

    ui->widget->hide();
//    ui->widget->setParentWidget(this);//不应该关闭
//    ui->widget_select->setAutoFillBackground(true);   // 这个属性一定要设置
//    changeSelectWidgetStyle();

    ui->label_version->setText(tr(u8"软件版本信息：V%1").arg(qApp->applicationVersion()));
    ui->stackedWidget_center->setCurrentIndex(0);

    auto layout = this->layout();
    layout->setMargin(0);
    layout->setAlignment(ui->horizontalLayout,Qt::AlignTop);

    //登录布局
    ui->verticalLayout->setSizeConstraint(QLayout::SetFixedSize);

    //[标题文字间隔]
    auto font = ui->label_title->font();
    font.setLetterSpacing(QFont::AbsoluteSpacing,10);
    ui->label_title->setFont(font);

    auto font1 = ui->label_userSetting->font();
    font1.setLetterSpacing(QFont::AbsoluteSpacing, 10);
    ui->label_userSetting->setFont(font1);

    ui->horizontalLayout_help->setAlignment(ui->checkBox_rememberPW,Qt::AlignLeft);
    ui->horizontalLayout_help->setAlignment(ui->label_change, Qt::AlignRight);

    ui->label_change->installEventFilter(this);
    ui->lineEdit_passwd->installEventFilter(this);

    setLoginError(false, "");
    loadLastLoginInfo();

    setStyle();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        break;
    default:
        QDialog::keyPressEvent(event);
    }
}

//void LoginDialog::on_pushButton_developer_clicked()
//{
//    changeSelectWidgetStyle();
//}

//void LoginDialog::on_pushButton_operator_clicked()
//{
//    changeSelectWidgetStyle();
//}

bool LoginDialog::eventFilter(QObject *watched, QEvent *event)
{
//    if(watched == this)
//    {
//        if(event->type() == QEvent::Resize ||
//                event->type() == QEvent::Show)
//        {
//            changeSelectWidgetStyle();
//        }
//        if(event->type() == QEvent::KeyPress)
//        {
//            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
//            if(keyEvent->key() == Qt::Key_Return ||
//                    keyEvent->key() == Qt::Key_Enter)
//            {
//                qDebug()<<"evrnt====1";
//                on_pushButton_login_clicked();
//            }
//        }
//    }

//    if(watched == this)
//    {
//        if(event->type() == QEvent::MouseButtonPress)
//        {
//            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
//            if(mouseEvent->buttons() == Qt::LeftButton)
//            {
//                this->setProperty("PressPos",mouseEvent->globalPos());
//                this->setProperty("IsLeftPress",true);
////                return true;
//            }
//        }else if(event->type() == QEvent::MouseMove)
//        {
//            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
//            if(mouseEvent->buttons() == Qt::LeftButton)
//            {
//                bool isLeftPress = this->property("IsLeftPress").toBool();
//                if(isLeftPress)
//                {
//                    if(this->isMaximized())
//                    {
//                        return true;
//                    }
//                    auto pressPos = this->property("PressPos").toPoint();
//                    auto offsetPos = mouseEvent->globalPos() - pressPos;
//                    this->move(this->geometry().topLeft()+offsetPos);
//                    this->setProperty("PressPos",mouseEvent->globalPos());
////                    return true;
//                }
//            }
//        }else if(event->type() == QEvent::MouseButtonRelease)
//        {
//            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
//            if(mouseEvent->buttons() == Qt::LeftButton)
//            {
//                this->setProperty("IsLeftPress",false);
//                this->setProperty("PressPos",QPoint(0,0));
////                return true;
//            }
//        }
//    }

    if(event->type() == QEvent::MouseButtonPress)
    {
        if(watched == ui->label_change)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->buttons() == Qt::LeftButton)
            {
                setChangePwError(false, "");
                ui->lineEdit_userName_2->setText(ui->lineEdit_userName->text());
                ui->stackedWidget_center->setCurrentIndex(1);
            }
        }
    }else if(event->type() == QEvent::KeyPress)
    {
        if( watched == this  ){
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if(keyEvent->key() == Qt::Key_Return ||
                    keyEvent->key() == Qt::Key_Enter)
            {
                on_pushButton_login_clicked();
            }
        }
    }

    return QWidget::eventFilter(watched,event);
}

void LoginDialog::setStyle()
{
    QString styleStr;
    QFile ff;

    ff.setFileName(":logindialog.qss");
    if (ff.exists())
    {
        ff.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&ff);
        styleStr += (ts.readAll());
        ff.close();
    }else{
        qDebug()<<"qss not found !" << ff.fileName();
    }
    this->setStyleSheet( styleStr );
}

void LoginDialog::changeSelectWidgetStyle()
{
//    QImage image;
//    if(ui->pushButton_developer->isChecked())
//    {
//        image.load(":login/user_o_checked.png");
//    }else
//    {
//        image.load(":login/user_t_checked.png");
//    }

//    QPalette palette(ui->widget_select->palette());
//    palette.setBrush(QPalette::Window,
//                 QBrush(image.scaled(ui->widget_select->size(),
//                                     Qt::IgnoreAspectRatio,
//                                     Qt::SmoothTransformation)));
//    ui->widget_select->setPalette(palette);
}

void LoginDialog::loadLastLoginInfo()
{
    auto user = UserManagerPtr->curLoginUser();
    qDebug()<<"ddddddddddddddd"<<user;
    if (user.isRemember)
    {
        ui->checkBox_rememberPW->setChecked(true);

        ui->lineEdit_userName->setText(user.name);
        ui->lineEdit_passwd->setText(user.passwd);
        btGroup->button( user.type )->setChecked(true);
        ui->pushButton_login->setFocus();
    }
    else
    {
        ui->lineEdit_userName->setText(user.name);
        ui->lineEdit_passwd->setText("");
        ui->checkBox_rememberPW->setChecked(false);
        ui->lineEdit_passwd->setFocus();
    }

}

void LoginDialog::setLoginError(bool isError,const QString &text)
{
    ui->label_tipIcon->setHidden(!isError);
    ui->label_tipText->setHidden(!isError);
    ui->label_tipText->setText(text);
}

void LoginDialog::setChangePwError(bool isError, const QString & text)
{
    ui->label_tipIcon_2->setHidden(!isError);
    ui->label_tipText_2->setHidden(!isError);
    ui->label_tipText_2->setText(text);
}

void LoginDialog::on_pushButton_login_clicked()
{
    auto userName = ui->lineEdit_userName->text();
    auto passwd = ui->lineEdit_passwd->text();

    m_loginType = STUserData::UserError ;
    STUserData::UserType type = STUserData::UserType(btGroup->checkedId());

    auto isRemember = ui->checkBox_rememberPW->isChecked();


    STUserData user(type,userName,passwd);
    user.isRemember = isRemember;

    auto theUserExist = UserManagerPtr->isHasTheUser(user);
    if (!theUserExist)
    {
        setLoginError(true, tr("用户不存在。"));

        emit sig_login_return(false);
        //        this->reject();//隐藏
        return;
    }

    if(UserManagerPtr->login(user))
    {
        setLoginError(false, "");
        qDebug()<<"login ok!"<<user.name;

        this->accept();//隐藏
    }else
    {
        setLoginError(true, tr("用户名密码错误,请确认再试。"));

        emit sig_login_return(false);
        //        this->reject();//隐藏
        return;
    }

    m_loginType = type;
    emit sig_login_return(true);
}

void LoginDialog::on_pushButton_confirm_clicked()
{
    STUserData::UserType type = STUserData::UserType(btGroup->checkedId());
    auto userName = ui->lineEdit_userName_2->text();
    if (userName.isEmpty())
    {
        setChangePwError(true, tr("用户名为空。"));
        return;
    }
    else
    {
        STUserData user;
        user.type = type;
        user.name = userName;

        auto theUserExist = UserManagerPtr->isHasTheUser(user);
        if (!theUserExist)
        {
            setChangePwError(true, tr("用户不存在。"));
            return;
        }
    }

    auto passwd1 = ui->lineEdit_passwd_2->text();
    auto passwd2 = ui->lineEdit_passwd_3->text();
    if (passwd1 == passwd2 && passwd1.size() == 6)
    {
        auto oldUser = STUserData(type, userName, "1");
        auto newUser = STUserData(type, userName, passwd1);
        auto isok = UserManagerPtr->modifyUser(oldUser, newUser);
        if (!isok)
        {
            setChangePwError(true, tr(u8"修改失败，无法写入数据库。"));
            return;
        }
        else
        {
            setChangePwError(false, "");
            on_pushButton_returnLogin_clicked();
            return;
        }
    }
    else
    {
        if (passwd1.size() != 6 || passwd2.size() != 6)
        {
            setChangePwError(true, tr(u8"密码长度不足6位数。"));
            return;
        }
        if (passwd1 != passwd2)
        {
            setChangePwError(true, tr(u8"两次密码不一致。"));
            return;
        }
    }

}

void LoginDialog::on_pushButton_returnLogin_clicked()
{
    ui->stackedWidget_center->setCurrentIndex(0);
}
