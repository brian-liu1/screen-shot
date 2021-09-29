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
        pen.setColor(Qt::yellow);
        painter->setPen(pen);
        painter->drawPolygon(m_leftpoly);
        painter->drawPolygon(m_rightpoly);
        painter->drawPolygon(m_toppoly);
        painter->drawPolygon(m_bottompoly);
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
    m_rcBounding.moveTo(0, 0);//本地坐标
    setRectSize(m_rcBounding);
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
    prepareGeometryChange();
}

void MyRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_StateFlag == MOV_LEFT_LINE)
    {
        QPainterPath path_;
        path_.moveTo(m_start);
        QPointF pos = QPointF(mapToScene(event->pos()).x(), m_height);
        path_.lineTo(pos);
        m_rcBounding = path_.boundingRect();
        setRectSize(m_rcBounding);
        prepareGeometryChange();
        //qDebug()<<m_rcBounding;
    }
    else if(m_StateFlag == MOV_RECT)
    {
        QPointF pos = event->pos() - m_start;
        moveBy(pos.x(), pos.y());
        //qDebug()<<event->pos();
        //prepareGeometryChange();
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
        m_start = QPointF(this->pos().x() + m_rcBounding.width(), this->pos().y() + m_rcBounding.height());
        m_height = m_rcBounding.height();
        setCursor(Qt::SizeHorCursor);
    }
    else if(m_rightpoly.containsPoint(pos, Qt::WindingFill))
    {
        m_StateFlag = MOV_RIGHT_LINE;
        m_start = QPointF(this->pos().x() + m_rcBounding.width(), this->pos().y() + m_rcBounding.height());
        setCursor(Qt::SizeHorCursor);
    }
    else if(m_toppoly.containsPoint(pos, Qt::WindingFill))
    {
        m_StateFlag = MOV_TOP_LINE;
        m_start = m_rcBounding.bottomLeft();
        setCursor(Qt::SizeVerCursor);
    }
    else if(m_bottompoly.containsPoint(pos, Qt::WindingFill))
    {
        m_StateFlag = MOV_BOTTOM_LINE;
        m_start = m_rcBounding.topLeft();
        setCursor(Qt::SizeVerCursor);
    }
    else if(m_rcBounding.contains(pos))
    {
        m_StateFlag = MOV_RECT;
        m_start = pos;
        setCursor(Qt::OpenHandCursor);
        qDebug()<<m_start<<pos<<this->pos()<<mapToScene(pos);
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
