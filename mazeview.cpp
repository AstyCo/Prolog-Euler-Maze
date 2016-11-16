#include "mazeview.h"

#include <QWheelEvent>

#include <QDebug>


MazeView::MazeView(QWidget *parent):
    QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
    setInteractive(false);
}

void MazeView::wheelEvent(QWheelEvent *event)
{
    scale(1 + event->delta() * 1.0 / 1200, 1 + event->delta() * 1.0 / 1200);
}

void MazeView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);

    QGraphicsScene * s = scene();
    if(s && !s->items().isEmpty())
        return;

    QPainter painter(viewport());
//    painter.begin(this);
    painter.setOpacity(0.3);
    painter.setFont(QFont("Segoe print",20,-1,false));
    painter.drawText(rect().adjusted(5,5,-5,-5),QString::fromUtf8("Сгенерируйте лабиринт"),QTextOption(Qt::AlignCenter));
    painter.setOpacity(1);
}
