#include "mazescene.h"

#include <QDebug>

MazeScene::MazeScene(QObject * parent):
    QGraphicsScene(parent)
{

}

void MazeScene::initScene(int rows, int columns)
{
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
}

void MazeScene::setEntrace(int row, int column)
{
    qDebug() << "setEntrace " <<row<<','<<column;
    qDebug() << m_rows;

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
//#include <QGraphicsSceneMouseEvent>

//void MazeScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    qDebug() << "scene clicked at "<< event->scenePos();
//    QGraphicsScene::mousePressEvent(event);
//}
