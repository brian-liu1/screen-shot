#include "graphicview.h"

GraphicView::GraphicView(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

GraphicView::~GraphicView()
{

}
