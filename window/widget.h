#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAction>
#include <QBoxLayout>
#include <QPushButton>
#include <windows.h>
#include <QSystemTrayIcon>
#include <QTimer>
#include "screenwidget.h"

enum HIDEPOSATION//隐藏位置
{
    HP_None = 0,
    HP_Top = 1,
    HP_Left = 2,
    HP_Right = 3
};

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void initBoard(void);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void hideWindow();
    void showWindow();
    bool isWindowInScreen();
    QPoint checkPos(QPoint &);
    void creatMenu();

public slots:
    void shotScreen();
    void timerout();
    void activatedSysTray(QSystemTrayIcon::ActivationReason);
    void exit();
signals:
    void click(void);

private:
    ATOM HTK_RUN;
    HIDEPOSATION m_hp;
    int m_screenWidth;
    int m_screenHeight;
    QSystemTrayIcon * mSysTrayIcon;
    bool m_drag;
    QPoint m_startpos;
    QPoint topleftpos;
    QTimer *m_timer;
    QMenu * menu;
    bool flag_close;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    void enterEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // WIDGET_H
