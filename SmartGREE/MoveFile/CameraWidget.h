#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>

namespace Ui {
class CameraWidget;
}

//e.g. CameraWidget::instance()->addTemplate();
class CameraWidget : public QWidget
{
    Q_OBJECT

public:
    static CameraWidget* instance(){static CameraWidget*obj=new CameraWidget(); return obj;}
    explicit CameraWidget(QWidget *parent = 0);
    ~CameraWidget();

    enum ENUM_CHECK_TYPE{
        E_DITUO,
        E_BANJIN,
        E_XXX,
    };

    ///TODO===========
    void initCamera();//初始化相机
    int state();//相机当前状态 1=ok,0=ng
    void setAppPath(const QString&path);//设置工程路径
    int setProductId(const QString&id);//切换产品
//    int checkProduct(const QString&id,int type,QString&picture);//拍照并检测
//    int check2D1Dir(const QString &id,QString &picture);
//    int check2D1(const QString &id,QString &picture);
//    int check2D2pos(const QString &id,QString &picture,float &x,float &y,float &z,float &Rx,float &Ry,float &Rz);

    int check2D1Dir(const QString &id,QString &picture);
    int check2D2pos(const QString &id,QString &picture,float &x,float &y,float &z,float &Rx,float &Ry,float &Rz);
    int check2D1pos(const QString &id,QString &picture);


    int addTemplate(const QString &id, int type);//新建模板
    void showTemplates();//显示模板列表
    QWidget* settingsWt();//获取参数设置界面
    ///end TODO===========
signals:
    void changeID(const QString &id);

private slots:
    void on_toolButton_add_clicked();
    void on_pushButton_showTeps_clicked();
    void on_pushButton_addTep_clicked();

private:
    Ui::CameraWidget *ui;
};

#endif // CAMERAWIDGET_H
