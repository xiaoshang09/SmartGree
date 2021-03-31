#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>

namespace Ui {
class LogWidget;
}

class LogWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget *parent = 0);
    ~LogWidget();

protected:
    virtual void showEvent(QShowEvent *event);

private slots:
    /**
       * @brief 接收日志信息的槽函数
       * @param log，格式化后的日志信息
       */
      void onAppendLog(const QString& log);
      void showFileTxt(const QString& fileTxt);

      void on_pushButton_now_clicked();
      void on_pushButton_check_clicked();
      void on_pushButton_fiter_clicked();
      void on_lineEdit_returnPressed();

private:
    Ui::LogWidget *ui;
};

#endif // LOGWIDGET_H
