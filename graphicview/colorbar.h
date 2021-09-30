#ifndef COLORBAR_H
#define COLORBAR_H

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

#define RADIUS 50
#define DELAT_X (RADIUS + 20)

class ColorBar :public QGraphicsObject
{
    Q_OBJECT
public:
    ColorBar();
    ~ColorBar();
    QRectF  boundingRect() const;
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    enum { Type = UserType + 10};
    int type() const{
        return  Type;
    }

signals:
    void colorIndex(int);

private:
    void setColorRect();
    QPolygonF setPolyRect(QPointF & pos, int radius);
    void setPointVect();

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
        QVector<QPointF> pos;
        QVector<QPolygonF> polyVect;
        int m_location;
};

#endif // COLORBAR_H

