#include "graphicscene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <graphicview/myline.h>
#include <graphicview/myrect.h>
#include <graphicview/textedit.h>

GraphicScene::GraphicScene(QObject *parent)
    : QGraphicsScene(parent),
      pen_w(2)
{
    m_color = Qt::black;
}

GraphicScene::~GraphicScene(){

}

void GraphicScene::setToolType(int type){
    m_toolType = type;
    if(type == 0)
        m_sharp = nullptr;
    if(type > 0)
    {
        QCursor cursor;
        //加载图片
        QPixmap pixmap(":/icon/line.png");
        //定义大小
        QSize picSize(16, 16);
        //缩放图片，按比例
        QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::KeepAspectRatio);
        //设置样式
        cursor = QCursor(scaledPixmap, -1, -1);
        this->views().at(0)->setCursor(cursor);
    }
    else {
        this->views().at(0)->setCursor(Qt::ArrowCursor);
    }
}

void GraphicScene::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton) return;
    if(m_toolType > 0)
    {
        switch (m_toolType) {
        case 1:
        {
            MyLine * line = new MyLine();
            line->setStrokeColor(m_color);
            line->setStrokeWidth(pen_w);
            if(line == nullptr) return;//异常，不作处理
            line->setStartPoint(ev->scenePos());
            m_sharp = qgraphicsitem_cast<QGraphicsItem *>(line);
            addItem(m_sharp);
        }break;
        case 2:
        {
            MyRect * rect = new MyRect();
            rect->setStrokeColor(m_color);
            rect->setStrokeWidth(pen_w);
            if(rect == nullptr) return;//异常，不作处理
            rect->setStartPoint(ev->scenePos());
            m_sharp = qgraphicsitem_cast<QGraphicsItem *>(rect);
            addItem(m_sharp);
        }break;
        default:
            break;
        }
        ev->accept();
    }
    else {
        QPointF mousePos (ev->buttonDownScenePos(Qt::LeftButton).x(),
                          ev->buttonDownScenePos(Qt::LeftButton).y());
        const QList<QGraphicsItem* >itemList = items(mousePos);
        m_Item = itemList.isEmpty() ? nullptr :itemList.first();
        if(m_Item != nullptr)
            m_oldPos = m_Item->pos();
        QGraphicsScene::mousePressEvent(ev);
    }
}

void GraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    if(m_toolType > 0 && m_sharp)
    {
        switch (m_toolType) {
        case 1:
        {
            MyLine * line;
            line = qgraphicsitem_cast<MyLine *>(m_sharp);
            line->setEndPoint(ev->scenePos());

        }break;
        case 2:
        {
            MyRect * rect;
            rect = qgraphicsitem_cast<MyRect *>(m_sharp);
            rect->setEndPoint(ev->scenePos());
        }break;
        default:
            break;
        }
        update();
        ev->accept();

    }
    else
    {
        QGraphicsScene::mouseMoveEvent(ev);
    }
}

void GraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton) return;//只处理鼠标左键
    if(m_toolType > 0)
    {
        if(m_toolType == 3)
        {
            TextEdit * text = new TextEdit();
            if(text == nullptr) return;//异常，不作处理
            m_sharp = qgraphicsitem_cast<TextEdit *>(text);
            addItem(m_sharp);
            text->setPos(ev->scenePos());
            text->setFocus();

        }
        if(m_sharp != nullptr)
            emit itemAddSignal(qgraphicsitem_cast<QGraphicsItem *>(m_sharp));
        setToolType(0);
        ev->accept();
    }
    else {
        if(m_Item != nullptr){
            if(m_oldPos != m_Item->pos())
                emit itemMoveSignal(qgraphicsitem_cast<QGraphicsItem *>(m_Item), m_oldPos);
        }
       QGraphicsScene::mouseReleaseEvent(ev);
    }
}

void GraphicScene::setPenW(int w)
{
    pen_w = w;
}

void GraphicScene::setColor(QColor &c)
{
    m_color = c;
}
