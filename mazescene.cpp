#include "mazescene.h"

#include <QDebug>

MazeScene::MazeScene(QObject * parent):
    QGraphicsScene(parent)
{

}

void MazeScene::initScene(int rows, int columns)
{
    if(!m_objects.isEmpty())
    {
        m_objects.clear();
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

            addItem(cell);
            cell->setPos(EDGE*j, EDGE*i);

            row.append(cell);
        }

        m_objects.append(row);
    }

    setSceneRect(itemsBoundingRect());
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
}

int MazeScene::rows() const
{
    return m_rows;
}

int MazeScene::columns() const
{
    return m_columns;
}
//#include <QGraphicsSceneMouseEvent>

//void MazeScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    qDebug() << "scene clicked at "<< event->scenePos();
//    QGraphicsScene::mousePressEvent(event);
//}
