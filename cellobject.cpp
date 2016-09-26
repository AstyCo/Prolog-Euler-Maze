#include "cellobject.h"

#include <QPainter>
#include <QDebug>

QBrush mazeBrush(Qt::white),
        foneBrush(Qt::black);




CellObject::CellObject(QGraphicsItem * parent):
    QGraphicsObject(parent)
{

}

QRectF CellObject::boundingRect() const
{
    return QRectF(0,0,EDGE,EDGE);
}

void CellObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

//    qDebug() << "CellObject::paint";

    painter->fillRect(boundingRect(),foneBrush);


    painter->fillRect(1*EDGE_BY_3,1*EDGE_BY_3,EDGE_BY_3,EDGE_BY_3,mazeBrush);

    if(!m_cell.left)
        painter->fillRect(0*EDGE_BY_3,1*EDGE_BY_3,EDGE_BY_3,EDGE_BY_3,mazeBrush);
    if(!m_cell.top)
        painter->fillRect(1*EDGE_BY_3,0*EDGE_BY_3,EDGE_BY_3,EDGE_BY_3,mazeBrush);
    if(!m_cell.right)
        painter->fillRect(2*EDGE_BY_3,1*EDGE_BY_3,EDGE_BY_3,EDGE_BY_3,mazeBrush);
    if(!m_cell.bottom)
        painter->fillRect(1*EDGE_BY_3,2*EDGE_BY_3,EDGE_BY_3,EDGE_BY_3,mazeBrush);

}

void CellObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "mousePressEvent "<<m_cell;
    update();
}

Cell& CellObject::cell()
{
    return m_cell;
}

void CellObject::setCell(const Cell &cell)
{
    m_cell = cell;
}

QDebug operator<<(QDebug d, const Cell &cell) {

    d << "Cell("
      <<"right: "
     <<cell.right
    <<",bottom: "
    <<cell.bottom
    <<",left: "
    <<cell.left
    <<",top: "
    <<cell.top
    <<")";

    return d;
}
