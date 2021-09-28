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

protected:
    QRectF  boundingRect() const;
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPointF m_start;
    QPointF m_next;
    QRectF m_rcBounding;
    QPen m_pen;
    int m_strokeWidth;//线段宽度
    QColor m_strokeColor;//线段颜色
    QPainterPath path;
    QPointF m_topLeftInScene;

};

#endif // MYRECT_H
