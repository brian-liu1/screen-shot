#include "myrect.h"
#include <QGraphicsScene>
#include <QDebug>

MyRect::MyRect():
    m_start(QPointF(0, 0)),
    m_next(QPointF(0, 0)),
    m_rcBounding(0, 0, 0, 0),
    hasHover(false)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    this->acceptDrops();
    this->setAcceptHoverEvents(true);
}

MyRect::~MyRect()
{

}

QRectF  MyRect::boundingRect() const
{
    if(!hasHover)
    return m_rcBounding;
    else {
        return QRectF(m_rcBounding.x() - 4, m_rcBounding.y() - 4, m_rcBounding.width() + 8, m_rcBounding.height() + 8);
    }
}

void MyRect::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    painter->setPen(m_pen);
    //QPainterPath path_ = path.translated(-m_topLeftInScene);
    //painter->drawRect(path_.boundingRect());
    painter->drawRect(m_rcBounding);
    if(hasHover)
    {
        QPen pen;
        QBrush brush(Qt::FDiagPattern);
        brush.setColor(Qt::red);
        pen.setColor(Qt::red);
        pen.setWidth(m_pen.width() + 2);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawEllipse(QPointF(m_rcBounding.x() ,                        m_rcBounding.y() + m_rcBounding.height() / 2), 4, 4);
        painter->drawEllipse(QPointF(m_rcBounding.x() + m_rcBounding.width() , m_rcBounding.y() + m_rcBounding.height() / 2), 4, 4);
        painter->drawEllipse(QPointF(m_rcBounding.x() + m_rcBounding.width() / 2 , m_rcBounding.y() + m_rcBounding.height()), 4, 4);
        painter->drawEllipse(QPointF(m_rcBounding.x() + m_rcBounding.width() / 2 , m_rcBounding.y()), 4, 4);
    }
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


void MyRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    qDebug()<<"hover enter";
    hasHover = true;
    prepareGeometryChange();
}

void MyRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    qDebug()<<"hover leave";
    hasHover = false;
    prepareGeometryChange();
}

void MyRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"mouser move";
    QGraphicsItem::mouseMoveEvent(event);
}

void MyRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"mouser press";
    QGraphicsItem::mousePressEvent(event);
}

void MyRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"mouse release";
    QGraphicsItem::mouseReleaseEvent(event);
}
