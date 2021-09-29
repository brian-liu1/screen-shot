#include "mybutton.h"

MyButton::MyButton(QWidget *parent) : QPushButton(parent)
{

}

MyButton::~MyButton()
{

}

void MyButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
    QPushButton::enterEvent(event);
}
