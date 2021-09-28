#include "myrect.h"
#include <QGraphicsScene>
#include <QDebug>

MyRect::MyRect():
    m_start(QPointF(0, 0)),
    m_next(QPointF(0, 0)),
    m_rcBounding(0, 0, 0, 0)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    this->acceptDrops();
}

MyRect::~MyRect()
{

}

QRectF  MyRect::boundingRect() const
{
    return m_rcBounding;
}

void MyRect::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    painter->setPen(m_pen);
    //QPainterPath path_ = path.translated(-m_topLeftInScene);
    //painter->drawRect(path_.boundingRect());
    painter->drawRect(boundingRect());
    painter->restore();
}

void MyRect::setStartPoint(const QPointF &pos)
{
    setPos(pos);
    m_start = pos;
    path.moveTo(pos);
}

void MyRect::setEndPoint(const QPointF &pos)
{
    QPainterPath path_;
    path_.moveTo(m_start);
    path_.lineTo(pos);
    m_rcBounding = path_.boundingRect();
    //m_topLeftInScene = m_rcBounding.topLeft();
    //setPos(m_topLeftInScene);
    m_rcBounding.moveTo(0, 0);//本地坐标
}

void MyRect::setStrokeWidth(int w){
    m_strokeWidth = w;
    m_pen.setWidth(w);
}

void MyRect::setStrokeColor(const QColor &color){
    m_strokeColor = color;
    m_pen.setColor(color);
}
