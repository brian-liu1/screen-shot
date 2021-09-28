#ifndef MYLINE_H
#define MYLINE_H

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

class MyLine :public QGraphicsItem
{
public:
    MyLine();
    ~MyLine();
    QRectF  boundingRect() const;
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setStartPoint(const QPointF &pos);
    void setEndPoint(const QPointF &pos);
    void setStrokeWidth(float w);
    void setStrokeColor(const QColor &color);
    enum { Type = UserType + 1};
    int type() const{
        return  Type;
    }
    QPainterPath * getPath(void);
    void setPath(QPainterPath *);
private:
    QPointF m_start;
    QPointF m_next;
    QPainterPath path;
    QRectF m_rcBounding;
    QPen m_pen;
    QPointF m_topLeftInScene;
    float m_strokeWidth;//线段宽度
    QColor m_strokeColor;//线段颜色
};

#endif // MYLINE_H
