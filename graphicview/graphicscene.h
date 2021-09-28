#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>

class GraphicScene: public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicScene(QObject *parent = nullptr);
    ~GraphicScene();
    void setToolType(int);
    void setColor(QColor &c);
    void setPenW(int w);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *ev);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);

signals:
    void itemMoveSignal(QGraphicsItem * p, QPointF pos);
    void itemAddSignal(QGraphicsItem * p);

private:
    QPixmap * m_pixmap;
    int m_toolType;
    QGraphicsItem * m_sharp;
    QGraphicsItem * m_Item;
    QColor  m_color;
    int pen_w;
    QPointF m_oldPos;

};

#endif // GRAPHICSCENE_H
