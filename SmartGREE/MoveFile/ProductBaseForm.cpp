#include "ProductBaseForm.h"
#include "ui_ProductBaseForm.h"


ProductBaseForm::ProductBaseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductBaseForm)
{
    ui->setupUi(this);
    m_nowFm=NULL;
    m_dataFm=NULL;
    m_sqlFm=NULL;

    initWin();
}

ProductBaseForm::~ProductBaseForm()
{
    delete ui;
}

void ProductBaseForm::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
//    showMain();
    qDebug()<<"test=====911======";
}

void ProductBaseForm::showMain()
{
    ui->stackedWidget->setCurrentWidget( m_nowFm );
}

void ProductBaseForm::initWin()
{
    qDebug()<<"test=====5===";
    m_nowFm=new ProductNowForm();
    qDebug()<<"test=====6===";
    m_dataFm=new ProductDataForm();
    m_sqlFm=new ProductSqlForm();

    qDebug()<<"test=====7===";
    ui->stackedWidget->addWidget( m_nowFm );
    ui->stackedWidget->addWidget( m_dataFm );
    ui->stackedWidget->addWidget( m_sqlFm );

    showMain();
    qDebug()<<"test=====8===";
}

void ProductBaseForm::on_pushButton_now_clicked()
{
    ui->stackedWidget->setCurrentWidget( m_nowFm );
}

void ProductBaseForm::on_pushButton_data_clicked()
{
    ui->stackedWidget->setCurrentWidget( m_dataFm );
}

void ProductBaseForm::on_pushButton_sql_clicked()
{
    ui->stackedWidget->setCurrentWidget( m_sqlFm );
}
