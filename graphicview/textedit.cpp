#include "textedit.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QGraphicsSceneDragDropEvent>
#include <QDrag>
#include <math.h>
#include <QGraphicsScene>
#include <QApplication>
#include <QStyleOptionGraphicsItem>

TextEdit::TextEdit()
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    setTextInteractionFlags(Qt::TextEditorInteraction);
}

TextEdit::~TextEdit()
{

}

void TextEdit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget * widget)
{ 
    QStyleOptionGraphicsItem opt(*option);
    opt.state = QStyle::State_None;
    QGraphicsTextItem::paint(painter, &opt ,widget);

}

void TextEdit::focusInEvent(QFocusEvent *event)
{
    if(event->reason() != Qt::PopupFocusReason){//注意右键菜单再次进入焦点时不保存原始文本
        m_store_str = toPlainText();//保存原始文本
    }
   QGraphicsTextItem::focusInEvent(event);
}

void TextEdit::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit editEnd();//编辑完成(发送信号出去)
    QGraphicsTextItem::focusOutEvent(event);
}

void TextEdit::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){//左键双击进入可编辑状态并打开焦点
        setTextInteractionFlags(Qt::TextEditorInteraction);
        setFocus();
        //QGraphicsTextItem::mouseDoubleClickEvent(event);
    }
}

