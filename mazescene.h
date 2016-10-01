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
    void updateRow(const QVector<Cell> &row, int index);
    void setEntrace(int row, int column);

    int rows() const;
    int columns() const;
    void setRows(int rows);
    void setColumns(int columns);

    void setViewportMode(const QPointer<QWidget> &vp, viewMode mode);
    viewMode modeForViewport(const QPointer<QWidget> &vp) const;
    bool isEntrace(const QPair<int,int> & pos) const;
    void clearEntraces();
    int objectsSize() const;
    int objectsCols() const;



public slots:
    void updateEntraces();
    void convertRepresentation(bool only_top = false);
    void clear();
    void hideItems();
    void appendRow(const QVector<Cell> &row);

private:
    void updateEntracesBorders(int row, int column);

    QVector<QPair<int,int> > m_entraces;
    QMap<QPointer<QWidget>,viewMode> m_modes;
    QVector<QVector<QPointer<CellObject> > > m_objects;
    int m_rows,m_columns;
};

#endif // MAZESCENE_H
