//#pragma execution_character_set("utf-8")

#include "screenwidget.h"
#include "qmutex.h"
#include "qapplication.h"
#include "qpainter.h"
#include "qfiledialog.h"
#include "qevent.h"
#include "qdatetime.h"
#include "qstringlist.h"
#include "qscreen.h"
#include <QColorDialog>
#include "mycommand.h"


#define deskGeometry qApp->primaryScreen()->geometry()
#define deskGeometry2 qApp->primaryScreen()->availableGeometry()

#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))

Screen::Screen(QSize size)
{
    maxWidth = size.width();
    maxHeight = size.height();

    startPos = QPoint(-1, -1);
    endPos = startPos;
    leftUpPos = startPos;
    rightDownPos = startPos;
    status = SELECT;
}

int Screen::width()
{
    return maxWidth;
}

int Screen::height()
{
    return maxHeight;
}

Screen::STATUS Screen::getStatus()
{
    return status;
}

void Screen::setStatus(STATUS status)
{
    this->status = status;
}

void Screen::setEnd(QPoint pos)
{
    endPos = pos;
    leftUpPos = startPos;
    rightDownPos = endPos;
    cmpPoint(leftUpPos, rightDownPos);
}

void Screen::setStart(QPoint pos)
{
    startPos = pos;
}

QPoint Screen::getEnd()
{
    return endPos;
}

QPoint Screen::getStart()
{
    return startPos;
}

QPoint Screen::getLeftUp()
{
    return leftUpPos;
}

QPoint Screen::getRightDown()
{
    return rightDownPos;
}

bool Screen::isInArea(QPoint pos)
{
    if (pos.x() > leftUpPos.x() && pos.x() < rightDownPos.x() && pos.y() > leftUpPos.y() && pos.y() < rightDownPos.y()) {
        return true;
    }

    return false;
}

void Screen::move(QPoint p)
{
    int lx = leftUpPos.x() + p.x();
    int ly = leftUpPos.y() + p.y();
    int rx = rightDownPos.x() + p.x();
    int ry = rightDownPos.y() + p.y();

    if (lx < 0) {
        lx = 0;
        rx -= p.x();
    }

    if (ly < 0) {
        ly = 0;
        ry -= p.y();
    }

    if (rx > maxWidth)  {
        rx = maxWidth;
        lx -= p.x();
    }

    if (ry > maxHeight) {
        ry = maxHeight;
        ly -= p.y();
    }

    leftUpPos = QPoint(lx, ly);
    rightDownPos = QPoint(rx, ry);
    startPos = leftUpPos;
    endPos = rightDownPos;
}

void Screen::cmpPoint(QPoint &leftTop, QPoint &rightDown)
{
    QPoint l = leftTop;
    QPoint r = rightDown;

    if (l.x() <= r.x()) {
        if (l.y() <= r.y()) {
            ;
        } else {
            leftTop.setY(r.y());
            rightDown.setY(l.y());
        }
    } else {
        if (l.y() < r.y()) {
            leftTop.setX(r.x());
            rightDown.setX(l.x());
        } else {
            QPoint tmp;
            tmp = leftTop;
            leftTop = rightDown;
            rightDown = tmp;
        }
    }
}

QScopedPointer<ScreenWidget> ScreenWidget::self;
ScreenWidget *ScreenWidget::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new ScreenWidget);
        }
    }

    return self.data();
}

ScreenWidget::ScreenWidget(QWidget *parent) : QWidget(parent)
{
    //this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);

    menu = new QMenu(this);
    menu->addAction("保存当前截图", this, SLOT(saveScreen()));
//    menu->addAction("保存全屏截图", this, SLOT(saveFullScreen()));
//    menu->addAction("截图另存为", this, SLOT(saveScreenOther()));
//    menu->addAction("全屏另存为", this, SLOT(saveFullOther()));
    menu->addAction("退出截图", this, SLOT(close()));

    //取得屏幕大小
    screen = new Screen(deskGeometry.size());
    //保存全屏图像
    fullScreen = new QPixmap();
    setAttribute(Qt::WA_DeleteOnClose);
}

ScreenWidget::~ScreenWidget(){

}

void ScreenWidget::paintEvent(QPaintEvent *)
{
    int x = screen->getLeftUp().x();
    int y = screen->getLeftUp().y();
    int w = screen->getRightDown().x() - x;
    int h = screen->getRightDown().y() - y;

    QPainter painter(this);

    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.drawPixmap(0, 0, *bgScreen);//画模糊背景

    if (w != 0 && h != 0) {
        painter.drawPixmap(x, y, fullScreen->copy(x, y, w, h));//模拟去模糊
    }

    painter.drawRect(x, y, w, h);

    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawText(x + 2, y - 8, tr("截图范围：( %1 x %2 ) - ( %3 x %4 )  图片大小：( %5 x %6 )")
                     .arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
}

void ScreenWidget::showEvent(QShowEvent *)
{
    QPoint point(-1, -1);
    screen->setStart(point);
    screen->setEnd(point);

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    QScreen *pscreen = QApplication::primaryScreen();
    *fullScreen = pscreen->grabWindow(0, 0, 0, screen->width(), screen->height());
#else
    *fullScreen = fullScreen->grabWindow(0, 0, 0, screen->width(), screen->height());
#endif

    //设置透明度实现模糊背景
    QPixmap pix(screen->width(), screen->height());
    pix.fill((QColor(160, 160, 160, 200)));
    bgScreen = new QPixmap(*fullScreen);
    QPainter p(bgScreen);
    p.drawPixmap(0, 0, pix);
}

void ScreenWidget::saveScreen()
{
//    int x = screen->getLeftUp().x();
//    int y = screen->getLeftUp().y();
//    int w = screen->getRightDown().x() - x;
//    int h = screen->getRightDown().y() - y;

//    QString fileName = QString("%1/screen_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
//    fullScreen->copy(x, y, w, h).save(fileName, "png");
    QImage image(QSize(m_scene->width(), m_scene->height()), QImage::Format_RGB32);
    QPainter painter(&image);
    m_scene->render(&painter);
    //QStringList file_name = QFileDialog::getSaveFileName(this, "保存图片", ".", "jpg files(*.jpg)");
    QString strSaveName = QFileDialog::getSaveFileName(this,tr("保存图片"),tr("."),tr("jpg files(*.jpg)"));
    if(!strSaveName.isEmpty())
        image.save(strSaveName);
    close();
}

void ScreenWidget::saveFullScreen()
{
    QString fileName = QString("%1/full_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
    fullScreen->save(fileName, "png");
    close();
}

void ScreenWidget::saveScreenOther()
{
    QString name = QString("%1.png").arg(STRDATETIME);
    QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
    if (!fileName.endsWith(".png")) {
        fileName += ".png";
    }

    if (fileName.length() > 0) {
        int x = screen->getLeftUp().x();
        int y = screen->getLeftUp().y();
        int w = screen->getRightDown().x() - x;
        int h = screen->getRightDown().y() - y;
        fullScreen->copy(x, y, w, h).save(fileName, "png");
        close();
    }
}

void ScreenWidget::saveFullOther()
{
    QString name = QString("%1.png").arg(STRDATETIME);
    QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
    if (!fileName.endsWith(".png")) {
        fileName += ".png";
    }

    if (fileName.length() > 0) {
        fullScreen->save(fileName, "png");
        close();
    }
}

void ScreenWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (screen->getStatus() == Screen::SELECT) {
        screen->setEnd(e->pos());
    }
//    else if (screen->getStatus() == Screen::MOV) {
//        QPoint p(e->x() - movPos.x(), e->y() - movPos.y());
//        screen->move(p);
//        movPos = e->pos();
//    }

    this->update();
}

void ScreenWidget::mousePressEvent(QMouseEvent *e)
{
    int status = screen->getStatus();

    if (status == Screen::SELECT) {
        screen->setStart(e->pos());
    }
//    else if (status == Screen::MOV) {
//        if (screen->isInArea(e->pos()) == false) {
//            screen->setStart(e->pos());
//            screen->setStatus(Screen::SELECT);
//        } else {
//            movPos = e->pos();
//            this->setCursor(Qt::SizeAllCursor);
//        }
//    }

    this->update();
}

void ScreenWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (screen->getStatus() == Screen::SELECT) {
        screen->setStatus(Screen::EDIT);
        initView();
    }
//    else if (screen->getStatus() == Screen::MOV) {
//        this->setCursor(Qt::ArrowCursor);
//    }
}

void ScreenWidget::contextMenuEvent(QContextMenuEvent * ev)
{
    int x = screen->getLeftUp().x();
    int y = screen->getLeftUp().y();
    int w = screen->getRightDown().x() - x;
    int h = screen->getRightDown().y() - y;

    QRect rect =  QRect(x, y, w, h);
    if(!rect.contains(ev->pos()))
    {
        this->setCursor(Qt::ArrowCursor);
        menu->exec(cursor().pos());
    }
}

void ScreenWidget::initView()
{
    m_view = new GraphicView(this);
    m_undoStack = new QUndoStack(this);//存放操作的栈
    int x = screen->getLeftUp().x();
    int y = screen->getLeftUp().y();
    int w = screen->getRightDown().x() - x;
    int h = screen->getRightDown().y() - y;
    m_view->setGeometry(x, y, w, h);
    m_view->setFixedSize(w, h);
    QPixmap pmap = fullScreen->copy(x, y, w, h);
    m_scene = new GraphicScene(nullptr);
    m_scene->setSceneRect(QRectF(-w/2, -h/2, w, h));
    m_scene->update();
    m_view->setScene(m_scene);
    QGraphicsPixmapItem * item = m_scene->addPixmap(pmap); // add scene background pixmap
    item->setPos(QPointF(-w/2, -h/2));
    m_view->setFocus();
    m_view->show();
    layout = new QVBoxLayout();
    QHBoxLayout *m_layout = new QHBoxLayout();
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    QPushButton * line = new QPushButton();
    line->setText("line");
    line->setFixedSize(40, 40);
    connect(line, &QPushButton::clicked, this, &ScreenWidget::addNewLine);
    QPushButton * rect = new QPushButton();
    rect->setText("rect");
    rect->setFixedSize(40, 40);
    connect(rect, &QPushButton::clicked, this, &ScreenWidget::addNewRect);
    QPushButton * text = new QPushButton();
    text->setText("text");
    text->setFixedSize(40, 40);
    connect(text, &QPushButton::clicked, this, &ScreenWidget::addNewText);
    QPushButton * undo = new QPushButton();
    undo->setText("undo");
    undo->setFixedSize(40, 40);
    connect(undo, &QPushButton::clicked, this, &ScreenWidget::undoAct);
    QPushButton * redo = new QPushButton();
    redo->setText("redo");
    redo->setFixedSize(40, 40);
    connect(redo, &QPushButton::clicked, this, &ScreenWidget::redoAct);
    QPushButton * save = new QPushButton();
    save->setText("save");
    save->setFixedSize(40, 40);
    connect(save, &QPushButton::clicked, this, &ScreenWidget::saveScreen);
    QPushButton * quit = new QPushButton();
    quit->setText("quit");
    quit->setFixedSize(40, 40);
    connect(quit, &QPushButton::clicked, this, &ScreenWidget::close);
    QSpacerItem * space = new QSpacerItem(20, 40, QSizePolicy::Expanding);
    m_layout->addSpacerItem(space);
    m_layout->addWidget(line);
    m_layout->addWidget(rect);
    m_layout->addWidget(text);
    m_layout->addWidget(undo);
    m_layout->addWidget(redo);
    m_layout->addWidget(save);
    m_layout->addWidget(quit);
    QHBoxLayout *m_tool = new QHBoxLayout();
    m_tool->setMargin(0);
    m_tool->setSpacing(0);
    QSpacerItem * space1 = new QSpacerItem(20, 40, QSizePolicy::Expanding);
    QPushButton * color = new QPushButton();
    QPalette pal = color->palette();
    pal.setColor(QPalette::Button, Qt::black);
    color->setPalette(pal);
    color->setAutoFillBackground(true);
    color->setFlat(true);
    color->setText("color");
    color->setFixedSize(40, 40);
    connect(color, &QPushButton::clicked, this, &ScreenWidget::setColor);
    QPushButton * size = new QPushButton();
    size->setText("size");
    size->setFixedSize(40, 40);
    QPushButton * font = new QPushButton();
    font->setText("font");
    font->setFixedSize(40, 40);
    m_tool->addSpacerItem(space1);
    m_tool->addWidget(color);
    m_tool->addWidget(size);
    m_tool->addWidget(font);
    layout->addWidget(m_view);
    layout->addItem(m_layout);
    layout->addItem(m_tool);
    setLayoutVisible(layout, 2, false);
    QWidget * widget = new QWidget(this);
    widget->setAttribute(Qt::WA_TranslucentBackground, true);
    layout->setMargin(0);
    layout->setSpacing(0);
    widget->setLayout(layout);
    widget->setGeometry(x, y, w, h + 80);
    connect(m_scene, &GraphicScene::itemMoveSignal,this, &ScreenWidget::itemMoved);
    connect(m_scene, &GraphicScene::itemAddSignal, this, &ScreenWidget::itemAdd);
    widget->show();
}

void ScreenWidget::setLayoutVisible(QVBoxLayout * layout, int row, bool bEnable)
{
    if(layout == nullptr) return;
    QHBoxLayout  * h_layout;
    QLayoutItem * it = layout->itemAt(row);
    if(it->layout() != nullptr)
    {
        h_layout = qobject_cast<QHBoxLayout *>(it->layout());
        for(int i = 0; i < h_layout->count(); i++)
        {
            it = h_layout->itemAt(i);
            if(it->spacerItem() != nullptr)
                continue;
            if(bEnable)
                it->widget()->show();
            else {
                it->widget()->hide();
            }
        }
    }
    //QHBoxLayout h_layout = layout.
}

void ScreenWidget::addNewLine()
{
    setLayoutVisible(layout, 2, true);
    m_scene->setToolType(1);
}

void ScreenWidget::addNewRect()
{
    setLayoutVisible(layout, 2, true);
    m_scene->setToolType(2);
}

void ScreenWidget::addNewText()
{
    setLayoutVisible(layout, 2, true);
    m_scene->setToolType(3);
}

void ScreenWidget::setColor()
{
    QPushButton* btn = (QPushButton*)sender();
    QPalette pal = btn->palette();
    QColor color = QColorDialog::getColor(pal.color(QPalette::Button), this);
    pal.setColor(QPalette::Button, color);
    btn->setPalette(pal);
    btn->setAutoFillBackground(true);
    btn->setFlat(true);
    m_scene->setColor(color);
}

void ScreenWidget::redoAct()
{
    m_undoStack->redo();//存放操作的栈
}

void ScreenWidget::undoAct()
{
    m_undoStack->undo();//存放操作的栈
}

void ScreenWidget::itemAdd(QGraphicsItem * item)
{
    m_undoStack->push(new addCommand(m_scene, item));
}

void ScreenWidget::itemMoved(QGraphicsItem * item, QPointF pos)
{
    m_undoStack->push(new moveCommand(item, pos));
}
