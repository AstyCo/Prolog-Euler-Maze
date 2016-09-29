#ifndef MAZESCENE_H
#define MAZESCENE_H

#include <QGraphicsScene>
#include <QVector>

#include <QWidget>
#include <QPointer>

#include <QMap>

#include "cellobject.h"

class MazeScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum viewMode
    {
        mazeNoInfo,
        mazeWithInfo
    };

    MazeScene(QObject * parent = 0);

    void initScene(int rows, int columns);
    void updateItems(const QVector<QVector<Cell> >&matrix);
    void setEntrace(int row, int column);

    int rows() const;
    int columns() const;
    void setViewportMode(const QPointer<QWidget> &vp, viewMode mode);
    viewMode modeForViewport(const QPointer<QWidget> &vp) const;
    bool isEntrace(const QPair<int,int> & pos) const;
    void clearEntraces();

private:
    QVector<QPair<int,int> > m_entraces;
    QMap<QPointer<QWidget>,viewMode> m_modes;
    QVector<QVector<QPointer<CellObject> > > m_objects;
    int m_rows,m_columns;
};

#endif // MAZESCENE_H
