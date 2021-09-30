#include "widget.h"
#include "screenwidget.h"
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QGuiApplication>
#include <QScreen>
#include <QtAlgorithms>
#include <QtGlobal>
#include <QApplication>
#include "mybutton.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      m_drag(false),
      flag_close(false)
{
    QPixmap icon(":icon/logo.png");
    setWindowIcon(icon);
    initBoard();
    QRect rect = QGuiApplication::primaryScreen()->geometry();
    m_screenWidth = rect.width();
    m_screenHeight = rect.height();
    setWindowFlag(Qt::FramelessWindowHint);
    QString str = QString::fromLocal8Bit("HOTKEY");
    std::wstring wlp = str.toStdWString();
    LPCWSTR lpcstr = wlp.c_str();
    HTK_RUN = GlobalAddAtom(lpcstr);
    RegisterHotKey(HWND(this->winId()), HTK_RUN, MOD_CONTROL|MOD_ALT, 'P');
    connect(this, &Widget::click, this, &Widget::shotScreen);
    m_hp = HP_Top;
    hideWindow();
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerout()));
    m_timer->start(500);
}

Widget::~Widget()
{
    UnregisterHotKey(HWND(this->winId()), HTK_RUN);
    GlobalDeleteAtom(HTK_RUN);
    qDebug()<<"app end";
}

void Widget::initBoard()
{
    MyButton *shot_screen = new MyButton(this);

    connect(shot_screen, &QPushButton::clicked, this, &Widget::shotScreen);
    QPixmap icon(":icon/cut.png");
    shot_screen->setFlat(true);
    shot_screen->setIcon(icon);
    shot_screen->setIconSize(QSize(40, 40));
    shot_screen->setGeometry(10, 10, 40, 40);
    this->setGeometry(920, 0, 60, 60);
}

void Widget::shotScreen()
{
    ScreenWidget * screen = new ScreenWidget();
    screen->setGeometry(50, 50, 720, 640);
    screen->show();
}

bool Widget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if(eventType == "windows_generic_MSG")
    {
        MSG* pMsg = reinterpret_cast<MSG*>(message);
        if(pMsg->message == WM_HOTKEY)
        {
            if(pMsg->wParam == HTK_RUN)
                emit click();
        }
    }
    return QWidget::nativeEvent(eventType, message, result);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    m_drag = true;
    m_startpos = event->globalPos() - this->pos();
    QWidget::mousePressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint m_pos;
    if(m_drag && isWindowInScreen())
    {
        m_pos = event->globalPos() - m_startpos;
        m_pos = checkPos(m_pos);
        move(m_pos);
        m_startpos = event->globalPos() - this->pos();
    }
    QWidget::mouseMoveEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        m_drag = false;
    QWidget::mouseReleaseEvent(event);
}

void Widget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    if(m_hp != HP_None)
        hideWindow();
}

void Widget::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::SizeAllCursor);
    if(m_hp != HP_None)
        showWindow();
}

void Widget::hideWindow()
{
    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    animation->setStartValue(QRect(x(),y(),width(),height()));
    if(m_hp == HP_Top)
        animation->setEndValue(QRect(x(),2 - height(),width(),height()));
    else if(m_hp == HP_Left)
        animation->setEndValue(QRect(2 - width(),y(),width(),height()));
    else if(m_hp == HP_Right)
        animation->setEndValue(QRect(m_screenWidth - 2,y(),width(),height()));
    animation->setDuration(250);
    animation->start();
}

void Widget::showWindow()
{
    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    animation->setStartValue(QRect(x(),y(),width(),height()));
    if(m_hp == HP_Top)
        animation->setEndValue(QRect(x(),0,width(),height()));
    else if(m_hp == HP_Left)
        animation->setEndValue(QRect(0,y(),width(),height()));
    else if(m_hp == HP_Right)
        animation->setEndValue(QRect(m_screenWidth - width(),y(),width(),height()));
    animation->setDuration(250);
    animation->start();
}

bool Widget::isWindowInScreen()
{
    bool index = false;
    QRect rect = QGuiApplication::primaryScreen()->geometry();
    QRect m_rect = QRect(this->pos().x(), this->pos().y(), width(), height());
    if(rect.contains(m_rect))
        index = true;
    else
        index = false;

    if(this->pos().x() <= 0)
        m_hp = HP_Left;
    else if(this->pos().y() <= 0)
        m_hp = HP_Top;
    else if(m_rect.right() >= rect.right())
        m_hp = HP_Right;
    else {
        m_hp = HP_None;
    }
    return true;
}

QPoint Widget::checkPos(QPoint & pos)
{
    int x;
    int y;
    QRect rect = QGuiApplication::primaryScreen()->geometry();
    QRect m_rect = QRect(rect.x(), rect.y(), rect.width() - width(), rect.height() - height());
    if(m_rect.contains(pos))
        return pos;
    else {
        x = qMax(pos.x(), 0);
        x = qMin(x, m_rect.width());
        y = qMax(pos.y(), 0);
        y = qMin(y, m_rect.height());
    }
    return QPoint(x, y);
}

void Widget::timerout()
{
    mSysTrayIcon = new QSystemTrayIcon(this);
    //新建托盘要显示的icon
    QIcon icon = QIcon(":/icon/logo.png");
    //将icon设到QSystemTrayIcon对象中
    mSysTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    mSysTrayIcon->setToolTip(QObject::trUtf8("白板V0.1"));
    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(activatedSysTray(QSystemTrayIcon::ActivationReason)));
    //在系统托盘显示此对象
    mSysTrayIcon->show();
    creatMenu();
    mSysTrayIcon->setContextMenu(menu);
    m_timer->stop();
    this->hide();
}

void Widget::activatedSysTray(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
        case QSystemTrayIcon::Trigger:
            //单击托盘图标
            break;
        case QSystemTrayIcon::DoubleClick:
            //双击托盘图标
            //双击后显示主程序窗口
            this->show();
            break;
        default:
            break;
        }
}

void Widget::closeEvent(QCloseEvent *event)
{
    if(!flag_close)
        event->ignore();
    else {
        QApplication::setQuitOnLastWindowClosed(true);
        event->accept();
    }
    this->hide();
}

void Widget::creatMenu()
{
    QAction * mShowMainAction = new QAction(QObject::trUtf8("显示主界面"),this);
    connect(mShowMainAction,SIGNAL(triggered()),this,SLOT(show()));

    QAction * mExitAppAction = new QAction(QObject::trUtf8("退出"),this);
    connect(mExitAppAction,SIGNAL(triggered()),this,SLOT(exit()));

    menu = new QMenu(this);
    menu->addAction(mShowMainAction);
    menu->addSeparator();
    menu->addAction(mExitAppAction);
}

void Widget::exit()
{
    flag_close = true;
    this->close();
}
