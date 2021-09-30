#include "myrect.h"
#include <QGraphicsScene>
#include <QDebug>

MyRect::MyRect():
    m_start(QPointF(0, 0)),
    m_next(QPointF(0, 0)),
    m_rcBounding(0, 0, 0, 0),
    hasHover(false),
    m_StateFlag(DEFAULT_FLAG)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    //this->acceptDrops();
    this->setAcceptHoverEvents(true);
}

MyRect::~MyRect()
{

}

QRectF  MyRect::boundingRect() const
{
    if(!hasHover)
        return QRectF(m_rcBounding.x() - 20, m_rcBounding.y() - 20, m_rcBounding.width() + 40, m_rcBounding.height() + 40);
    else {
        return QRectF(m_rcBounding.x() - 24, m_rcBounding.y() - 24, m_rcBounding.width() + 48, m_rcBounding.height() + 48);
    }
}

void MyRect::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->save();
    painter->setPen(m_pen);
    painter->drawRect(m_rcBounding);
    if(hasHover)
    {
        QPen pen;
        QBrush brush(Qt::SolidPattern);
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
    m_topLeftInScene = m_rcBounding.topLeft();
    setPos(m_topLeftInScene);
    m_rcBounding.moveTo(0, 0);//本地坐标
    setRectSize(m_rcBounding);
    prepareGeometryChange();

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
    hasHover = true;
    prepareGeometryChange();
}

void MyRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    hasHover = false;
    setCursor(Qt::ArrowCursor);
    prepareGeometryChange();
}

void MyRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_StateFlag == MOV_LEFT_LINE)
    {
        QPainterPath path_;
        qreal delt_x = event->pos().x() - m_start.x();
        QPointF tl = QPointF(delt_x, 0);
        QPointF current = this->pos();
        path_.moveTo(tl);
        path_.lineTo(m_rcBounding.bottomRight());
        if(delt_x < m_rcBounding.bottomRight().x() - 1)
        {
            m_rcBounding = path_.boundingRect();
            m_rcBounding.moveTo(0, 0);
            prepareGeometryChange();
            setPos(current.x() + delt_x, current.y());
            setRectSize(m_rcBounding);
        }
    }
    else if(m_StateFlag == MOV_RIGHT_LINE)
    {
        QPainterPath path_;
        qreal delt_x = event->pos().x() - m_start.x();
        QPointF tr = QPointF(delt_x + m_rcBounding.topRight().x(), 0);
        if(tr.x() > 1)
        {
            path_.moveTo(tr);
            path_.lineTo(m_rcBounding.bottomLeft());
            m_rcBounding = path_.boundingRect();
            prepareGeometryChange();
            setRectSize(m_rcBounding);
            m_start = event->pos();
        }
    }
    else if(m_StateFlag == MOV_TOP_LINE)
    {
        QPainterPath path_;
        qreal delt_y = event->pos().y() - m_start.y();
        QPointF tl = QPointF(0, delt_y);
        QPointF current = this->pos();
        path_.moveTo(tl);
        path_.lineTo(m_rcBounding.bottomRight());
        if(delt_y < m_rcBounding.bottomRight().y() - 1)
        {
            m_rcBounding = path_.boundingRect();
            m_rcBounding.moveTo(0, 0);
            prepareGeometryChange();
            setPos(current.x(), current.y() + delt_y);
            setRectSize(m_rcBounding);
        }
    }
    else if(m_StateFlag == MOV_BOTTOM_LINE)
    {
        QPainterPath path_;
        qreal delt_y = event->pos().y() - m_start.y();
        QPointF br = QPointF(m_rcBounding.bottomRight().x(), delt_y + m_rcBounding.bottomRight().y());
        if(br.y() > 1)
        {
            path_.moveTo(br);
            path_.lineTo(m_rcBounding.topLeft());
            m_rcBounding = path_.boundingRect();
            prepareGeometryChange();
            setRectSize(m_rcBounding);
            m_start = event->pos();
        }
    }
    else if(m_StateFlag == MOV_RECT)
    {
        QPointF pos = event->pos() - m_start;
        moveBy(pos.x(), pos.y());
    }
    //QGraphicsItem::mouseMoveEvent(event);
}

void MyRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;
    QPointF pos = event->pos();
    if(m_leftpoly.containsPoint(pos, Qt::WindingFill))
    {
        m_StateFlag = MOV_LEFT_LINE;
        m_start = pos;
        m_height = m_rcBounding.height();
        setCursor(Qt::SizeHorCursor);
    }
    else if(m_rightpoly.containsPoint(pos, Qt::WindingFill))
    {
        m_StateFlag = MOV_RIGHT_LINE;
        m_start = pos;
        setCursor(Qt::SizeHorCursor);
    }
    else if(m_toppoly.containsPoint(pos, Qt::WindingFill))
    {
        m_StateFlag = MOV_TOP_LINE;
        m_start = pos;
        setCursor(Qt::SizeVerCursor);
    }
    else if(m_bottompoly.containsPoint(pos, Qt::WindingFill))
    {
        m_StateFlag = MOV_BOTTOM_LINE;
        m_start = m_start = pos;
        setCursor(Qt::SizeVerCursor);
    }
    else if(m_rcBounding.contains(pos))
    {
        m_StateFlag = MOV_RECT;
        m_start = pos;
        setCursor(Qt::OpenHandCursor);
    }
    //QGraphicsItem::mousePressEvent(event);
}

void MyRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    QGraphicsItem::mouseReleaseEvent(event);
}
 void MyRect::setRectSize(QRectF mrect)
 {
    m_oldRect = mrect;
    QPointF pf = QPointF(mrect.x() + mrect.width() / 2, mrect.y());
    m_toppoly = setPolyRect(pf, 4);
    QPointF rf = QPointF(mrect.x() + mrect.width(), mrect.y() + mrect.height() / 2);
    m_rightpoly = setPolyRect(rf, 4);
    QPointF bf = QPointF(mrect.x() + mrect.width() / 2, mrect.y() + mrect.height());
    m_bottompoly = setPolyRect(bf, 4);
    QPointF lf = QPointF(mrect.x(), mrect.y() + mrect.height() / 2);
    m_leftpoly = setPolyRect(lf, 4);
 }

 QPolygonF MyRect::setPolyRect(QPointF & pos, int radius)
 {
     QVector<QPointF> vpt;
     QPointF pf = QPointF(pos.x(), pos.y() - radius); //top
     vpt.append(pf);
     QPointF rf = QPointF(pos.x() + radius, pos.y()); //right
     vpt.append(rf);
     QPointF bf = QPointF(pos.x(), pos.y() + radius); //bottom
     vpt.append(bf);
     QPointF lf = QPointF(pos.x() - radius, pos.y()); //bottom
     vpt.append(lf);
     return QPolygonF(vpt);
 }
