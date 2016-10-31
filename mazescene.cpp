#include "mazescene.h"
#include "mainwindow.h"

#include <QDebug>

MazeScene::MazeScene(QObject * parent):
    QGraphicsScene(parent)
{
    m_rows = m_columns = 0;
    _hiddenPath = true;
}

void MazeScene::initScene(int rows, int columns)
{
    if(!m_objects.isEmpty() || !m_entraces.isEmpty())
    {
        m_objects.clear();
        m_entraces.clear();
        QGraphicsScene::clear();
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
            cell->setVisible(false);

            row.append(cell);
        }

        m_objects.append(row);
    }

    if(m_objects.isEmpty())
        setSceneRect(0,0,0,0);
    else
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

void MazeScene::updateRow(const QVector<Cell> &rowC, int index)
{
    for(int j=0;j<rowC.size();++j)
    {
        m_objects[index][j]->setCell(rowC[j]);
    }
    update();
}

void MazeScene::setEntrace(int row, int column)
{
    m_entraces.append(QPair<int,int>(row,column));
    updateEntraces();
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

int MazeScene::objectsSize() const
{
    return m_objects.size();
}

int MazeScene::objectsCols() const
{
    if(m_objects.isEmpty())
        return 0;
    return m_objects[0].size();
}

bool MazeScene::onPath(const QPair<int, int> &index) const
{
    if(_hiddenPath)
        return false;
    return _path.contains(index);
}

void MazeScene::updateEntraces()
{
    foreach(const QVector<QPointer<CellObject> >& row, m_objects)
        foreach(const QPointer<CellObject> &o_p, row)
        {
            Cell &cell = o_p->cell();
            if(isEntrace(cell.pos()))
                updateEntracesBorders(cell.r,cell.c);
        }
}

void MazeScene::convertRepresentation(bool only_top)
{
    int rows = m_objects.size(),
            columns = m_columns;
    QVector<bool> lastRowB(columns);
    bool lastCellR;

    for(int i = 0; i<rows; ++i)
    {
        for(int j=0; j<columns; ++j)
        {
            Cell &cell = m_objects[i][j]->cell();

            cell.left = (!j || lastCellR);
            cell.right = (j==columns-1 || cell.right);
            cell.top = (!i || lastRowB[j]);
            if(!only_top)
                cell.bottom = (i==rows-1 || cell.bottom);

            lastRowB[j] = cell.bottom;
            lastCellR = cell.right;
        }
    }
    updateEntraces();
    update();
}

void MazeScene::clear()
{
    initScene(0,0);
}

void MazeScene::hideItems()
{
    for(int i=0;i<m_rows;++i)
    {
        for(int j=0;j<m_columns;++j)
        {
            m_objects[i][j]->setVisible(false);
        }
    }
    update();
}

void MazeScene::appendRow(const QVector<Cell> &rowC)
{
    int i = m_objects.size();
    QVector<QPointer<CellObject> > row;
    for(int j=0;j<rowC.size();++j)
    {
        CellObject *cell = new CellObject;

        cell->setScene(this);
        cell->setPos(EDGE*j, EDGE*i);
        cell->setCell(rowC[j]);

        row.append(cell);
    }

    m_objects.append(row);

    setSceneRect(itemsBoundingRect().adjusted(-2,-2,2,2));
}

void MazeScene::clearPath()
{
    _path.clear();
}

void MazeScene::setPath(const QSet<QPair<int, int> > &indexes)
{
    _path = indexes;
}


void MazeScene::updateEntracesBorders(int row, int column)
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

bool MazeScene::hiddenPath() const
{
    return _hiddenPath;
}

void MazeScene::setHiddenPath(bool hiddenPath)
{
    _hiddenPath = hiddenPath;
    update();
}

void MazeScene::setColumns(int columns)
{
    m_columns = columns;
}

void MazeScene::setRows(int rows)
{
    m_rows = rows;
}
//#include <QGraphicsSceneMouseEvent>

//void MazeScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    qDebug() << "scene clicked at "<< event->scenePos();
//    QGraphicsScene::mousePressEvent(event);
//}
