#ifndef MYRECT_H
#define MYRECT_H

#include <QWidget>
#include <QtGui>
#include <QPixmap>
#include <QPainter>
#include <QRectF>
#include <QMouseEvent>
#include <QPointF>
#include <QDragEnterEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItem>

enum STATE_FLAG{
     DEFAULT_FLAG=0,
     MOV_LEFT_LINE,//标记当前为用户按下矩形的左边界区域
     MOV_TOP_LINE,//标记当前为用户按下矩形的上边界区域
     MOV_RIGHT_LINE,//标记当前为用户按下矩形的右边界区域
     MOV_BOTTOM_LINE,//标记当前为用户按下矩形的下边界区域
     MOV_RECT,//标记当前为鼠标拖动图片移动状态
 };

class MyRect: public QGraphicsItem
{
public:
    MyRect();
    ~MyRect();
    void setStartPoint(const QPointF &pos);
    void setEndPoint(const QPointF &pos);
    void setStrokeWidth(int w);
    void setStrokeColor(const QColor &color);
    enum { Type = UserType + 2};
    int type() const{
        return  Type;
    }
    void setRectSize(QRectF mrect );
    QPolygonF setPolyRect(QPointF & pos, int radius);
protected:
    QRectF  boundingRect() const;
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void    hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void    hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void    mousePressEvent(QGraphicsSceneMouseEvent *event);
    void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void    mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    QPointF m_start;
    QPointF m_next;
    QRectF m_rcBounding;
    QPen m_pen;
    int m_strokeWidth;//线段宽度
    QColor m_strokeColor;//线段颜色
    QPainterPath path;
    QPointF m_topLeftInScene;
    bool hasHover;
    QRectF m_oldRect;
    QPolygonF m_leftpoly;
    QPolygonF m_rightpoly;
    QPolygonF m_toppoly;
    QPolygonF m_bottompoly;
    STATE_FLAG  m_StateFlag;
    qreal m_width;
    qreal m_height;

};

#endif // MYRECT_H
