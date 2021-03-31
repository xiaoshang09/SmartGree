#ifndef MYFORM_H
#define MYFORM_H

#include <QWidget>
#include <QIODevice>
#include <QFile>
#include <showtextmessage.h>
#include "HalconCpp.h"

using namespace HalconCpp;
class CameraForm;

namespace Ui {
class MyTemplate;
}

class MyTemplate : public QWidget
{
    Q_OBJECT

public:
    explicit MyTemplate(QWidget *parent = 0);
    ~MyTemplate();

    void form_Initialization_Interface();
    void form_checkProduct(int i);

    void setWt(QWidget*wt);

    void appendMessage2(const QString &msg);

    bool DelDir(const QString &afile);

    void new_file(QString str);

    void dev_display_ncc_matching_results (HTuple hv_ModelID, HTuple hv_Color, HTuple hv_Row,
                                              HTuple hv_Column, HTuple hv_Angle, HTuple hv_Model);
    void dev_display_shape_matching_results (HTuple hv_ModelID, HTuple hv_Color, HTuple hv_Row,
        HTuple hv_Column, HTuple hv_Angle, HTuple hv_ScaleR, HTuple hv_ScaleC, HTuple hv_Model);

private slots:
    void on_form_pushButton_2D1_clicked();

    void on_form_pushButton_2D1_2_clicked();

    int on_form_Store_material_clicked();

    void on_form_Capture_clicked();

    void on_form_ROI_2D1_clicked();

    void on_form_show_2D1_clicked();

    void on_form_reset_2D1_clicked();

    void on_form_save_2D1_clicked();

    void on_from_ROI_2D2_clicked();

    void on_form_reset_2D2_clicked();

    void on_form_save_2D2_clicked();

    void on_form_new_save_clicked();

    void on_form_reset_clicked();

    void on_form_show_2D2_clicked();

    void on_form_ROI_2D1_2_clicked();

    void on_form_show_2D1_2_clicked();

    void on_form_save_2D1_2_clicked();


private:
    Ui::MyTemplate *ui;
    CameraForm *wt;

};

#endif // MYFORM_H
