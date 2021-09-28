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
   if(event->reason() == Qt::MouseFocusReason && QApplication::mouseButtons()==Qt::RightButton)
   {
      //右键点击其他地方失去焦点，定义为取消操作，恢复原始文本
       setPlainText(m_store_str);
       setTextInteractionFlags(Qt::NoTextInteraction);//恢复不能编辑状态
       //emit EditStatus(false);//编辑完成(发送信号出去)
       clearFocus();

   }
   else if(event->reason() == Qt::PopupFocusReason)
   {
  //右键弹出菜单时不做处理
   }
   else
   {  //其他情况，包括下面点击回车的情况，编辑成功，发送信号给父对象
        setTextInteractionFlags(Qt::NoTextInteraction);
        //emit EditStatus(false);//编辑完成(发送信号出去)
   }
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

