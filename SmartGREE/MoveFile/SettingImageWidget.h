#ifndef SETTINGIMAGEWIDGET_H
#define SETTINGIMAGEWIDGET_H

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QWidget>
#endif


namespace Ui {
class SettingImageWidget;
}

class SettingImageWidget : public QWidget
{
    Q_OBJECT

public:
    enum SaveType
    {
        SaveNothing,
        SaveAll,
        SaveOk,
        SaveNg,
    };


    explicit SettingImageWidget(QWidget *parent = 0);
    ~SettingImageWidget();

public slots:
    void onValueChanged(const char *key);
    void onLoad();
    void setDefault();
private slots:

    void on_radioButton_saveNothing_clicked(bool checked);

    void on_radioButton_saveAll_clicked(bool checked);

    void on_radioButton_saveOk_clicked(bool checked);

    void on_radioButton_saveNg_clicked(bool checked);

    void on_checkBox_saveCloud_clicked(bool checked);

    void on_checkBox_saveDepthImage_clicked(bool checked);

    void on_checkBox_saveGrayImage_clicked(bool checked);

    void on_comboBox_imageType_currentIndexChanged(const QString &arg1);

    void on_comboBox_cloudType_currentIndexChanged(const QString &arg1);
private slots:
//    QEosSlotFuncDefine(mainPageChangedSlot)
    void on_checkBox_genByDate_clicked(bool checked);

private:
    Ui::SettingImageWidget *ui;
};

#endif // SETTINGIMAGEWIDGET_H
