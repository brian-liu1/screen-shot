#ifndef MYCOMMAND_H
#define MYCOMMAND_H

#include <QUndoCommand>
#include "graphicscene.h"
//添加item
class addCommand :public QUndoCommand
{
public :
    addCommand(GraphicScene * graphicsScene, QGraphicsItem* item, QUndoCommand* parent = nullptr);

    void redo() override;//重写这两个函数
    void undo() override;

private:
    QGraphicsItem* m_item;

    GraphicScene* m_scene;

    QPointF m_initPos;
};
//移动item
class moveCommand:public QUndoCommand
{
public:
    moveCommand(QGraphicsItem* item, const QPointF oldPos, QUndoCommand* parent = 0);

    void redo() override;//重写这两个函数
    void undo() override;
private:
    QGraphicsItem * m_item;
    QPointF m_oldPos;
    QPointF m_newPos;

};

#endif // MYCOMMAND_H
