#include "TitleForm.h"
#include "ui_TitleForm.h"
#include <QApplication>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>
#include <QPainter>
#include <QMessageBox>

TitleForm::TitleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleForm)
  ,m_win(NULL)
{
    ui->setupUi(this);
    installEventFilter( this );

    //    btnBox.addButton(ui->label_logo,E_LOGO);
    //    btnBox.addButton(ui->label_msg, E_MSG);
    //    btnBox.addButton(ui->label_title, E_TITLE);

    btnBox.addButton(ui->toolButton_state, E_STATE);
    btnBox.addButton(ui->toolButton_set, E_SETTINGS);
    btnBox.addButton(ui->toolButton_usr, E_USR);
    btnBox.addButton(ui->toolButton_log, E_LOG);
    btnBox.addButton(ui->toolButton_help, E_HELP);

    btnBox.addButton(ui->toolButton_min, E_MIN);
    btnBox.addButton(ui->toolButton_max, E_MAX);
    btnBox.addButton(ui->toolButton_close, E_CLOSE);

    connect( &btnBox, SIGNAL(buttonClicked(int)), this, SIGNAL(clicked(int)));
}

TitleForm::~TitleForm()
{
    delete ui;
}

void TitleForm::setLogo(const QPixmap &pix)
{
    //    ui->label_logo->setText("");
    ui->label_logo->setPixmap( pix );
    ui->label_logo->setAlignment(Qt::AlignCenter);
}

void TitleForm::setTitle(const QString &str)
{
    ui->label_title->setText(str);;
}

void TitleForm::setMessage(const QString &str)
{
    ui->label_msg->setText(str);
}

void TitleForm::setWin(QWidget *win)
{
    m_win = win;
}

void TitleForm::setState(int state, const QString &txt)
{
    switch(state){
    case E_OK:
        ui->toolButton_state->setIcon( QIcon(":/image/ok.png") );
        ui->toolButton_state->setText(txt);
        break;
    case E_WARN:
        ui->toolButton_state->setIcon( QIcon(":/image/warn.png") );
        ui->toolButton_state->setText(txt);
        break;
    case E_NG:
    default:
        ui->toolButton_state->setIcon( QIcon(":/image/error.png") );
        ui->toolButton_state->setText(txt);
        break;
    }
}

void TitleForm::setUsr(const QString &str)
{
    ui->toolButton_usr->setText(str);
}

bool TitleForm::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->buttons() == Qt::LeftButton)
        {
            QRect rect(10,0,width()-10,height());
            if (rect.contains(mouseEvent->pos()))
            {
                //                        qDebug()<<__FUNCTION__<< "test==="<<mouseEvent->globalPos() <<m_win->geometry().topLeft();

                if(m_win){
                    //geometry().topLeft 这个数据有问题,移动过程获取异常// 在这里计算可防止出错
                    QPoint winPos = m_win->geometry().topLeft();
                    if(m_win->windowFlags() & Qt::FramelessWindowHint){
                        ;
                    }else{
                        winPos = QPoint(winPos.x(),winPos.y()-30);
                    }
                    setCursor(Qt::SizeAllCursor);
                    setProperty("curPos", mouseEvent->globalPos());
                    setProperty("OffsetPos", winPos-mouseEvent->globalPos());
                    setProperty("IsLeftPress", true);
                    setProperty("IsMove", false);

                    qDebug()<<"IsLeftPress true!";
                }
            }
            else
            {
                setProperty("IsLeftPress", false);

//                qDebug()<<"IsLeftPress false!";
            }
        }
    }
    else if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        //            qDebug()<<"mouseEvent->globalPos();=="<< mouseEvent->globalPos()<< mouseEvent->pos();
        if (mouseEvent->buttons() == Qt::LeftButton)
        {
            bool isLeftPress = property("IsLeftPress").toBool();
            if (isLeftPress)
            {
                if(m_win){
                    if (m_win->isMaximized())
                    {
                        return true;
                    }
                    else
                    {
                        QPoint lastPos = property("curPos").toPoint()-mouseEvent->globalPos();

                        if(  lastPos.manhattanLength()> 8 ){
                            setProperty("IsMove", true);

                            QPoint offPos = property("OffsetPos").toPoint();
                            QPoint mousePos =  mouseEvent->globalPos();//这个几乎没有变过
                            QPoint movePos =  offPos + mousePos;
                            m_win->move( movePos );
                        }
                    }
                    return true;
                }
            }
        }
        else
        {
            setProperty("IsLeftPress", false);
            return true;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        setCursor(Qt::ArrowCursor);
//        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        QWidget *action = QApplication::widgetAt(QCursor::pos());//获取鼠标点击处的控件
//        qDebug()<<__FUNCTION__ <<"release==2===" <<watched << ui->label_logo <<this << action;
//        qDebug()<<__FUNCTION__ <<"release==33==="
//               << property("IsLeftPress").toBool()
//               << property("IsMove").toBool();

        if(property("IsLeftPress").toBool() && (!property("IsMove").toBool()) ){

            if(!action && watched==this){//qt BUG
                //TODO.. 有个BUG ,偶尔出现0x0,导致无法返回
                emit clicked(E_TITLE);
            }else{
                if(action == ui->label_logo){
                    emit clicked(E_LOGO);
                }else if(action == ui->label_msg){
                    emit clicked(E_MSG);
                }else if(action == ui->label_title){
                    emit clicked(E_TITLE);
                }else{
                    qDebug()<<"===no widget clied!"<< watched  ;
                }
            }
        }

        setProperty("IsLeftPress", false);
//        qDebug()<<"IsLeftPress false!";

        return true;
    }
    else if (event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->buttons() == Qt::LeftButton)
        {
            qDebug()<<"isMaximized:"<<isMaximized();
            if( m_win ){
                if (m_win->isMaximized())
                {
                    m_win->showNormal();
                }
                else
                {
                    m_win->showMaximized();
                }
            }
        }
    }


    return QWidget::eventFilter(watched, event);
}

void TitleForm::paintEvent(QPaintEvent *event)
{
    static QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);

    //TODO

    QWidget::paintEvent( event );
}

void TitleForm::on_toolButton_min_clicked()
{
    if( m_win ){
        m_win->showMinimized();
    }
}

void TitleForm::on_toolButton_max_clicked()
{
    if( m_win ){
        if (m_win->isMaximized())
        {
            m_win->showNormal();
        }
        else
        {
            m_win->showMaximized();
        }
    }
}

void TitleForm::on_toolButton_close_clicked()
{
    if( m_win ){
        if(QMessageBox::Yes == QMessageBox::question(this, u8"提示", u8"确定关闭软件吗?"))
        {
            m_win->close();
        }
    }
}

