#include "myline.h"
#include <QGraphicsScene>
#include <QDebug>

MyLine::MyLine():
    m_start(QPointF(0, 0)),
    m_next(QPointF(0, 0)),
    m_rcBounding(0, 0, 0, 0)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

MyLine::~MyLine()
{

}

QRectF  MyLine::boundingRect() const
{
    return m_rcBounding;
}

void MyLine::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    painter->setPen(m_pen);
    QPainterPath path_ = path.translated(-m_topLeftInScene);
    painter->drawPath(path_);
    painter->restore();
}

void MyLine::setStartPoint(const QPointF &pos)
{
    setPos(pos);
    m_start = pos;
    path.moveTo(pos);
}

void MyLine::setEndPoint(const QPointF &pos)
{
    path.lineTo(pos);
    m_rcBounding = path.boundingRect();
    m_topLeftInScene = m_rcBounding.topLeft();
    setPos(m_topLeftInScene);
    m_rcBounding.moveTo(0, 0);//本地坐标
}

void MyLine::setStrokeWidth(float w){
    m_strokeWidth = w;
    m_pen.setWidth(w);
}

void MyLine::setStrokeColor(const QColor &color){
    m_strokeColor = color;
    m_pen.setColor(color);
}

QPainterPath * MyLine::getPath()
{
    return &path;
}

void  MyLine::setPath(QPainterPath * p)
{
    path = *p;
}
