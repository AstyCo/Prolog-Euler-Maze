#include "mazescene.h"

#include <QDebug>

MazeScene::MazeScene(QObject * parent):
    QGraphicsScene(parent)
{

}

void MazeScene::initScene(int rows, int columns)
{
    if(!m_objects.isEmpty() || !m_entraces.isEmpty())
    {
        m_objects.clear();
        m_entraces.clear();
        clear();
    }

    m_rows = rows;
    m_columns = columns;
    for(int i=0;i<rows;++i)
    {
        QVector<QPointer<CellObject> > row;
        for(int j=0;j<columns;++j)
        {
            CellObject *cell = new CellObject;

            cell->setScene(this);
            cell->setPos(EDGE*j, EDGE*i);

            row.append(cell);
        }

        m_objects.append(row);
    }

    setSceneRect(itemsBoundingRect().adjusted(-2,-2,2,2));
}

void MazeScene::updateItems(const QVector<QVector<Cell> > &matrix)
{
    for(int i=0;i<m_rows;++i)
    {
        for(int j=0;j<m_columns;++j)
        {
            m_objects[i][j]->setCell(matrix[i][j]);
        }
    }
    update();
}

void MazeScene::setEntrace(int row, int column)
{
    if(row==0)
    {
        m_objects[row][column]->cell().top = false;
    }
    else if(column == 0)
    {
        m_objects[row][column]->cell().left = false;
    }
    else if (row==m_rows-1)
    {
        m_objects[row][column]->cell().bottom = false;
    }
    else
    {
        m_objects[row][column]->cell().right = false;
    }

    m_entraces.append(QPair<int,int>(row,column));
}

int MazeScene::rows() const
{
    return m_rows;
}

int MazeScene::columns() const
{
    return m_columns;
}

void MazeScene::setViewportMode(const QPointer<QWidget> &vp, MazeScene::viewMode mode)
{
    if(vp)
    {
        if(m_modes.contains(vp))
            m_modes[vp] = mode;
        else
            m_modes.insert(vp,mode);
    }
}

MazeScene::viewMode MazeScene::modeForViewport(const QPointer<QWidget> &vp) const
{
    if(m_modes.contains(vp))
        return m_modes[vp];

    return mazeNoInfo;
}

bool MazeScene::isEntrace(const QPair<int,int> &pos) const
{
    return m_entraces.contains(pos);
}

void MazeScene::clearEntraces()
{
    m_entraces.clear();
}
//#include <QGraphicsSceneMouseEvent>

//void MazeScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    qDebug() << "scene clicked at "<< event->scenePos();
//    QGraphicsScene::mousePressEvent(event);
//}
