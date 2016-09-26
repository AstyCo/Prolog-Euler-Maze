#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include <QWidget>

struct Cell
{
    bool right,
        bottom,
        left,
        top;
};

class MazeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MazeWidget(QWidget *parent = 0);

    QPair<int, int> in() const;
    void setIn(const QPair<int, int> &in);

    QPair<int, int> out() const;
    void setOut(const QPair<int, int> &out);

    QVector<QVector<Cell> > matrix() const;
    void setMatrix(const QVector<QVector<QPair<bool, bool> > > &matrix);

    int rows() const;
    void setRows(int rows);

    int columns() const;
    void setColumns(int columns);

protected:
    void paintEvent(QPaintEvent *);

private:
    bool isEntrace(int x, int y) const;

private:
    QPair<int,int> m_in, m_out;
    QVector<QVector<Cell> > m_matrix;
    int m_rows,m_columns;
};

#endif // MAZEWIDGET_H
