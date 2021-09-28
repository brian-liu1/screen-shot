#include "mycommand.h"
#include "myline.h"
#include "myrect.h"
#include "textedit.h"


addCommand::addCommand(GraphicScene * graphicsScene, QGraphicsItem* item, QUndoCommand* parent)
{
    Q_UNUSED(parent)
    m_scene = graphicsScene;
    m_item = item;
    //setText("add item");//undoView 中就会显示（父类的方法）
}

void addCommand::redo()//stack push 时 会自动调用
{
    m_scene->addItem(m_item);
    m_scene->clearSelection();
    m_scene->update();
}

void addCommand::undo()
{
    m_scene->removeItem(m_item);
    m_scene->update();
}

moveCommand::moveCommand(QGraphicsItem *item, const QPointF oldPos, QUndoCommand *parent)
{
    Q_UNUSED(parent);
    m_item = item;
    m_newPos = m_item->pos();
    m_oldPos = oldPos;
}

void moveCommand::redo()
{
    m_item->setPos(m_newPos);
    //setText(QString("Move Item:(%1,%2)").arg(m_item->pos().rx()).arg(m_item->pos().ry()));
}

void moveCommand::undo()
{
    m_item->setPos(m_oldPos);
    m_item->scene()->update();
    //setText(QString("Move Item:(%1,%2)").arg(m_item->pos().rx()).arg(m_item->pos().ry()));
}

