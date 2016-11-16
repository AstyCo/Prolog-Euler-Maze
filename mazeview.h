#ifndef MAZEVIEW_H
#define MAZEVIEW_H

#include <QGraphicsView>

class MazeView : public QGraphicsView
{
    Q_OBJECT

public:
    MazeView(QWidget *parent = 0);

protected:
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);

};

#endif // MAZEVIEW_H
