#include "colorbar.h"
#include <QGraphicsScene>
#include <complex>
#include <math.h>

ColorBar::ColorBar()
{
    setPointVect();
    setColorRect();
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    setSelected(true);
    setFocus();
}

ColorBar::~ColorBar()
{

}

QRectF  ColorBar::boundingRect() const
{
    return QRectF(0, 0, DELAT_X * 2, DELAT_X * 2);
}

void ColorBar::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    QVector<QColor> mycolor;
    mycolor<<Qt::gray<<Qt::red<<Qt::green<<Qt::yellow<<Qt::cyan<<Qt::black<<Qt::magenta<<Qt::blue;
    painter->save();
    QPen pen;
    QBrush brush(Qt::SolidPattern);
    pen.setColor(Qt::black);
    foreach(QPointF p, pos)
    {
        int i = pos.indexOf(p);
        brush.setColor(mycolor.at(i));
        painter->setBrush(brush);
        if(i == m_location)
            pen.setWidth(6);
        else
            pen.setWidth(2);
        painter->setPen(pen);
        painter->drawEllipse(p, 16, 16);
    }
    painter->restore();
}

void ColorBar::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->pos();
    foreach(QPolygonF p, polyVect)
    {
        if(p.containsPoint(pos, Qt::WindingFill))
        {
            emit colorIndex(polyVect.indexOf(p));
            return;
        }
    }
    emit colorIndex(-1);
}

void ColorBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->pos();
    foreach(QPolygonF p, polyVect)
    {
        if(p.containsPoint(pos, Qt::WindingFill))
        {
            m_location = polyVect.indexOf(p);
            prepareGeometryChange();
            return;
        }
    }
    if(m_location != -1)
    {
        m_location = -1;
        prepareGeometryChange();
    }

}

void ColorBar::setColorRect()
{
    foreach(QPointF p, pos)
    {
        QPolygonF poly = setPolyRect(p, 16);
        polyVect<<poly;
    }
}

void ColorBar::setPointVect()
{
    qreal x, y;
    x = 0;
    y = -RADIUS;
    pos<<QPointF(x + DELAT_X, y + DELAT_X);//p0
    x = RADIUS * cos(45 * M_PI / 180);
    y = -RADIUS * cos(45 * M_PI / 180);
    pos<<QPointF(x + DELAT_X, y + DELAT_X);//p1
    x = RADIUS;
    y = 0;
    pos<<QPointF(x + DELAT_X, y + DELAT_X);//p2
    x = RADIUS * cos(45 * M_PI / 180);
    y = RADIUS * cos(45 * M_PI / 180);
    pos<<QPointF(x + DELAT_X, y + DELAT_X);//p3
    x = 0;
    y = RADIUS;
    pos<<QPointF(x + DELAT_X, y + DELAT_X);//p4
    x = -RADIUS * cos(45 * M_PI / 180);
    y = RADIUS * cos(45 * M_PI / 180);
    pos<<QPointF(x + DELAT_X, y + DELAT_X);//p5
    x = -RADIUS;
    y = 0;
    pos<<QPointF(x + DELAT_X, y + DELAT_X);//p6
    x = -RADIUS * cos(45 * M_PI / 180);
    y = -RADIUS * cos(45 * M_PI / 180);
    pos<<QPointF(x + DELAT_X, y + DELAT_X);//p7
}

QPolygonF ColorBar::setPolyRect(QPointF & pos, int radius)
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
