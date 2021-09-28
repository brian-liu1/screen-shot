#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H

#include <QGraphicsView>

class GraphicView: public QGraphicsView
{
    Q_OBJECT
public:
    GraphicView(QWidget *parent);
    ~GraphicView();
};

#endif // GRAPHICVIEW_H
