#ifndef CELLOBJECT_H
#define CELLOBJECT_H

#include "global.h"

#include <QGraphicsObject>



struct Cell
{
    explicit Cell()
    {
        right = bottom = left = top = false;
    }

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


    Cell& cell() ;
    void setCell(const Cell &cell);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    Cell m_cell;
};

QDebug operator<< (QDebug d, const Cell &cell);

#endif // CELLOBJECT_H
