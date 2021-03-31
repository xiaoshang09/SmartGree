#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QWidget>
#endif
#include "QTextEdit.h"

class QListWidgetItem;
namespace Ui {
class HelpWidget;
}

class HelpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HelpWidget(QWidget *parent = nullptr);
    ~HelpWidget() override;

private:
    Ui::HelpWidget *ui;
};

#endif // HELPWIDGET_H
