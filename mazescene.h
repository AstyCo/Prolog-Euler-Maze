#ifndef MAZESCENE_H
#define MAZESCENE_H

#include <QGraphicsScene>
#include <QVector>
#include <QPointer>

#include "cellobject.h"

class MazeScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MazeScene(QObject * parent = 0);

    void initScene(int rows, int columns);
    void updateItems(const QVector<QVector<Cell> >&matrix);
    void setEntrace(int row, int column);

//protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QVector<QVector<QPointer<CellObject> > > m_objects;
    int m_rows,m_columns;
};

#endif // MAZESCENE_H
