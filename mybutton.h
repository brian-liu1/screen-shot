#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QEvent>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = nullptr);
    ~MyButton();

protected:
    void enterEvent(QEvent *event);

signals:

public slots:
};

#endif // MYBUTTON_H
