#ifndef CELLOBJECT_H
#define CELLOBJECT_H

#include "global.h"

#include <QGraphicsObject>
#include <QWidget>
#include <QPointer>

class MazeScene;

struct Cell
{
    explicit Cell()
    {
        right = bottom = left = top = euler = false;
        index = -1;
    }

    QPair<int,int> pos() const{
        return QPair<int,int>(r,c);
    }

    int r, c;

    bool euler;
    int index;

    bool right,
        bottom,
        left,
        top;
};

class CellObject : public QGraphicsObject
{
    Q_OBJECT

public:
    CellObject(QGraphicsItem * parent = 0);

    QRectF boundingRect() const;
    void setScene(MazeScene *scene);

    Cell& cell() ;
    void setCell(const Cell &cell);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    Cell m_cell;
    QPointer<MazeScene> m_scene;
};

QDebug operator<< (QDebug d, const Cell &cell);

#endif // CELLOBJECT_H
