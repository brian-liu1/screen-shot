#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H


#include <QWidget>
#include <QMenu>
#include <QPoint>
#include <QSize>
#include <QLabel>
#include "graphicview/graphicview.h"
#include "graphicview/graphicscene.h"
#include <QBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QUndoStack>

class Screen
{
public:
    enum STATUS {SELECT, MOV, SET_W_H, EDIT};
    Screen() {}
    Screen(QSize size);

    void setStart(QPoint pos);
    void setEnd(QPoint pos);
    QPoint getStart();
    QPoint getEnd();

    QPoint getLeftUp();
    QPoint getRightDown();

    STATUS getStatus();
    void setStatus(STATUS status);

    int width();
    int height();
    bool isInArea(QPoint pos);          // 检测pos是否在截图区域内
    void move(QPoint p);                // 按 p 移动截图区域

private:
    QPoint leftUpPos, rightDownPos;     //记录 截图区域 左上角、右下角
    QPoint startPos, endPos;            //记录 鼠标开始位置、结束位置
    int maxWidth, maxHeight;            //记录屏幕大小
    STATUS status;                      //三个状态: 选择区域、移动区域、设置width height

    void cmpPoint(QPoint &s, QPoint &e);//比较两位置，判断左上角、右下角
};

#ifdef quc
class Q_DECL_EXPORT ScreenWidget : public QWidget
#else
class ScreenWidget : public QWidget
#endif

{
    Q_OBJECT
public:
    static ScreenWidget *Instance();
    explicit ScreenWidget(QWidget *parent = 0);
    ~ScreenWidget();

private:
    static QScopedPointer<ScreenWidget> self;
    QMenu *menu;            //右键菜单对象
    Screen *screen;         //截屏对象
    QPixmap *fullScreen;    //保存全屏图像
    QPixmap *bgScreen;      //模糊背景图
    QPoint movPos;          //坐标
    GraphicView * m_view;
    GraphicScene * m_scene;
    QVBoxLayout * layout;
    QColor m_color;
    int pen_w;
    QUndoStack* m_undoStack;

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);

private:
    void initView(void);
    void setLayoutVisible(QVBoxLayout * layout, int row, bool bEnable);

public slots:
    void saveScreen();
    void saveFullScreen();
    void saveScreenOther();
    void saveFullOther();
    void addNewLine();
    void addNewRect();
    void addNewText();
    void setColor();
    void redoAct();
    void undoAct();
    void itemMoved(QGraphicsItem * item, QPointF pos);
    void itemAdd(QGraphicsItem * item);
};

#endif // SCREENWIDGET_H

