#pragma once
#include <QLineEdit>

class ClickLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	ClickLineEdit(QWidget *parent = Q_NULLPTR);
	~ClickLineEdit();


protected:
	virtual void mousePressEvent(QMouseEvent *event) override;

signals:
	void Clicked();
};
