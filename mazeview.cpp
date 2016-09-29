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
