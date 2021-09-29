#ifndef TEXTEDIT_H
#define TEXTEDIT_H

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

class TextEdit :public QGraphicsTextItem
{
    Q_OBJECT
public:
    TextEdit();
    ~TextEdit();
    enum { Type = UserType + 3};
    int type() const{
        return  Type;
    }
signals:
    void editEnd(void);
protected:
    void focusInEvent(QFocusEvent * event);
    void focusOutEvent(QFocusEvent * event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //void keyPressEvent(QKeyEvent *event);
    //QRectF  boundingRect() const;
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    qreal       m_scaleValue;
    qreal       m_scaleDafault;
    QString     m_str;
    int         m_zoomState;
    bool        m_isMove;
    QPointF     m_startPos;
    QRectF      m_textRect;
    QRectF      m_rect;
    QString     m_store_str;//用于保存
};

#endif // TEXTEDIT_H
