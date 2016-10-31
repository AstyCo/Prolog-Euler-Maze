#include "cellobject.h"
#include "mazescene.h"

#include <QPainter>
#include <QWidget>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
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

void CellObject::setScene(MazeScene *scene)
{
    if(scene)
    {
        m_scene=scene;
        m_scene->addItem(this);
    }
}

void CellObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(m_scene.isNull())
        return;

    QBrush bgBrush;
    if(m_scene->isEntrace(m_cell.pos()) || m_scene->onPath(qMakePair(cell().c,cell().r)))
        bgBrush = QBrush(Qt::red);
    else
        bgBrush = mazeBrush;



    switch(m_scene->modeForViewport(widget))
    {
    case MazeScene::mazeNoInfo:
    {
        painter->fillRect(boundingRect(),foneBrush);
        painter->fillRect(1*EDGE_BY_3,1*EDGE_BY_3,EDGE_BY_3,EDGE_BY_3,bgBrush);

        if(!m_cell.left)
            painter->fillRect(0*EDGE_BY_3,1*EDGE_BY_3,EDGE_BY_3,EDGE_BY_3,bgBrush);
        if(!m_cell.top)
            painter->fillRect(1*EDGE_BY_3,0*EDGE_BY_3,EDGE_BY_3,EDGE_BY_3,bgBrush);
        if(!m_cell.right)
            painter->fillRect(2*EDGE_BY_3,1*EDGE_BY_3,EDGE_BY_3,EDGE_BY_3,bgBrush);
        if(!m_cell.bottom)
            painter->fillRect(1*EDGE_BY_3,2*EDGE_BY_3,EDGE_BY_3,EDGE_BY_3,bgBrush);

        break;
    }
    case MazeScene::mazeWithInfo:
    {
        painter->fillRect(boundingRect(),bgBrush);
        painter->setRenderHint(QPainter::Antialiasing,true);
        if(m_cell.left)
            painter->drawLine(0,0,0,EDGE);
        if(m_cell.top)
            painter->drawLine(0,0,EDGE,0);
        if(m_cell.right)
            painter->drawLine(EDGE,0,EDGE,EDGE);
        if(m_cell.bottom)
            painter->drawLine(0,EDGE,EDGE,EDGE);

        const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
        if(lod>=1)
        {
            if(m_cell.euler)
            {
                float factor = boundingRect().adjusted(1,1,-2,-2).width() / painter->fontMetrics().width(QString::number(m_cell.index));
                QFont f = painter->font();
                f.setPointSizeF(7);

                if (factor < 1)
                {
                    f.setPointSizeF(f.pointSizeF()*factor);
                }
                painter->setFont(f);
                painter->drawText(boundingRect(),QString::number(m_cell.index),QTextOption(Qt::AlignCenter));
            }
        }
        break;
    }
    default:
        Q_ASSERT(false);
    }
}

void CellObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
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
    setVisible(true);
}

QDebug operator<<(QDebug d, const Cell &cell) {

    if(cell.euler)
    {
        d << "Cell("
          <<"id: "
         <<cell.index
        <<",[right: "
        <<cell.right
        <<",bottom: "
        <<cell.bottom
        <<",left: "
        <<cell.left
        <<",top: "
        <<cell.top
        <<"])";
    }
    else
    {
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
    }

    return d;
}
