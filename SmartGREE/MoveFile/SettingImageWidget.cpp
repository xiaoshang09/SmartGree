#include "settingimagewidget.h"
#include "ui_settingimagewidget.h"
#include "SettingDatas.h"


Q_DECLARE_METATYPE(SettingImageWidget::SaveType)

SettingImageWidget::SettingImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingImageWidget)
{
    ui->setupUi(this);

    connect(SettingDatas::instance(), SIGNAL(valueChanged(const char *) ), this, SLOT(onValueChanged(const char *) ) );
    connect(SettingDatas::instance(), SIGNAL(loadFinished() ), this, SLOT(onLoad()));
    connect(SettingDatas::instance(), SIGNAL(sigSetDefault() ), this, SLOT(setDefault()));
}

SettingImageWidget::~SettingImageWidget()
{
    delete ui;
}

void SettingImageWidget::onValueChanged(const char *key)
{

}

void SettingImageWidget::onLoad()
{

}

void SettingImageWidget::setDefault()
{

}

void SettingImageWidget::on_radioButton_saveNothing_clicked(bool checked)
{
    if(checked)
    {
        //System->setSysParam("SaveImagesCondition",SaveNothing);
    }
}

void SettingImageWidget::on_radioButton_saveAll_clicked(bool checked)
{
    if(checked)
    {
        //System->setSysParam("SaveImagesCondition",SaveAll);
    }
}

void SettingImageWidget::on_radioButton_saveOk_clicked(bool checked)
{
    if(checked)
    {
        //System->setSysParam("SaveImagesCondition",SaveOk);
    }
}

void SettingImageWidget::on_radioButton_saveNg_clicked(bool checked)
{
    if(checked)
    {
        //System->setSysParam("SaveImagesCondition",SaveNg);
    }
}

void SettingImageWidget::on_checkBox_saveCloud_clicked(bool checked)
{
     //System->setSysParam("isSaveCloud",checked);
     if(checked)
     {
//         Util::InformationBox(NULL,tr("提示"),tr("由于点云数据较大，\n"
//                                               "选择保存点云会影响运行的效率,\n"
//                                               "请根据需求作选择。"));
     }
}

void SettingImageWidget::on_checkBox_saveDepthImage_clicked(bool checked)
{
//    //System->setSysParam("isSaveDepthImage",checked);
}

void SettingImageWidget::on_checkBox_saveGrayImage_clicked(bool checked)
{
//     //System->setSysParam("isSaveGrayImage",checked);
}

void SettingImageWidget::on_comboBox_imageType_currentIndexChanged(const QString &arg1)
{
//    //System->setSysParam("SaveImagesSuffix",arg1);
}

void SettingImageWidget::on_comboBox_cloudType_currentIndexChanged(const QString &arg1)
{
//    //System->setSysParam("SaveCloudSuffix",arg1);
}


void SettingImageWidget::on_checkBox_genByDate_clicked(bool checked)
{
//    //System->setSysParam("isGenImagesFileByDate",checked);
}
