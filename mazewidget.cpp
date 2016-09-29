#include "mazewidget.h"


#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

MazeWidget::MazeWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background-color:white;");
}

void MazeWidget::paintEvent(QPaintEvent *)
{
    QRect mazeRect = rect().adjusted(5,5,-5,-5);
    qreal dx = ((qreal)mazeRect.width()) / m_columns,
            dy = ((qreal)mazeRect.height()) / m_rows;

    QPainter painter(this);

    for(int i=0; i<m_rows;++i)
    {
        QColor color((i%2)?(Qt::white):(Qt::yellow));
        for(int j=0; j<m_columns; ++j)
        {
            painter.fillRect(QRect(j*dx,i*dy,(j+1)*dx,(i+1)*dy).adjusted(1,1,-2,-2),color);
            if(i==0 && !isEntrace(i,j) )
                painter.drawLine(j*dx,0,(j+1)*dx,0);
            if(i==m_rows-1 && !isEntrace(i,j) )
                painter.drawLine(j*dx,m_rows*dy,(j+1)*dx,m_rows*dy);

            if(j==0 && !isEntrace(i,j) )
                painter.drawLine(0,i*dy,0,(i+1)*dy);
            if(j==m_columns-1 && !isEntrace(i,j) )
                painter.drawLine(m_columns*dx,i*dy,m_columns*dx,(i+1)*dy);


            QPair<bool,bool> cell(m_matrix[i][j].right,m_matrix[i][j].bottom);

            if(cell.first)
                painter.drawLine((j+1)*dx,i*dy,(j+1)*dx,(i+1)*dy);
            if(cell.second)
                painter.drawLine(j*dx,(i+1)*dy,(j+1)*dx,(i+1)*dy);

        }

    }
}

bool MazeWidget::isEntrace(int x, int y) const
{
    return QPair<int,int>(x,y) == m_in || QPair<int,int>(x,y) == m_out;
}

int MazeWidget::columns() const
{
    return m_columns;
}

void MazeWidget::setColumns(int columns)
{
    m_columns = columns;
}

int MazeWidget::rows() const
{
    return m_rows;
}

void MazeWidget::setRows(int rows)
{
    m_rows = rows;
}

QVector<QVector<Cell> > MazeWidget::matrix() const
{
    return m_matrix;
}

void MazeWidget::setMatrix(const QVector<QVector<QPair<bool, bool> > > &matrix)
{

}

QPair<int, int> MazeWidget::out() const
{
    return m_out;
}

void MazeWidget::setOut(const QPair<int, int> &out)
{
    m_out = out;
}

QPair<int, int> MazeWidget::in() const
{
    return m_in;
}

void MazeWidget::setIn(const QPair<int, int> &in)
{
    m_in = in;
}
