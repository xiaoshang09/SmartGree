#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QWidget>
#endif

namespace Ui {
class AboutWidget;
}

class AboutWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AboutWidget(QWidget *parent = 0);
    ~AboutWidget();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::AboutWidget *ui;
};

#endif // ABOUTWIDGET_H
