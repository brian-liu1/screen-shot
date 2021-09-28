#include "widget.h"
#include "screenwidget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    initBoard();
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

Widget::~Widget()
{

}

void Widget::initBoard()
{
    QHBoxLayout *m_layout = new QHBoxLayout();
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    QPushButton *shot_screen = new QPushButton();
    connect(shot_screen, &QPushButton::clicked, this, &Widget::shotScreen);
    shot_screen->setText("shot");
    QPushButton * line = new QPushButton();
    line->setText("line");
    QPushButton * rect = new QPushButton();
    rect->setText("rect");
    m_layout->addWidget(shot_screen);
    this->setLayout(m_layout);
    this->setGeometry(100, 300, 40, 40);
}

void Widget::shotScreen()
{
    ScreenWidget * screen = new ScreenWidget();
    screen->showFullScreen();
}
