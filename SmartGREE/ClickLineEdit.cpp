#include "ClickLineEdit.h"

ClickLineEdit::ClickLineEdit(QWidget *parent)
	: QLineEdit(parent)
{
	setReadOnly(true);
}

ClickLineEdit::~ClickLineEdit()
{
}

void ClickLineEdit::mousePressEvent(QMouseEvent * event)
{
	emit Clicked();
}
