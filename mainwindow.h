#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mazescene.h"

#include <QMainWindow>

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <SWI-Stream.h>


namespace Ui {
class MainWindow;
}

class ParametersWidget;
class QSlider;
class GenerationState;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    static void read_entrace(term_t entraceT, QPair<int,int>& entrace);
    static void read_cell_var(term_t varT, bool& var);
    static void read_cell(term_t cellT, Cell &cell);
    static void read_euler_cell(term_t eulerCellT, Cell &cell);
    static void read_euler_maze(term_t mazeT,QPair<int,int>&in, QPair<int,int>& out, QVector<QVector<Cell > > &matrix);
    static void read_euler_row(term_t rowT,QVector<Cell> &row, int row_index);
    static void read_maze(term_t mazeT,QPair<int,int>&in, QPair<int,int>& out, QVector<QVector<Cell > > &matrix);
    static QString pl_display(term_t t);
    static QVector<QVector<Cell> > &convertRepresentation(QVector<QVector<Cell > >& matrix);

public slots:
    void initState();

private slots:
    void makeStep();
    void generateMaze();
    void on_actionInteractive_toggled(bool arg1);

    void on_actionRe_triggered();

private:
    void initScene();
    void installMenus();



    Ui::MainWindow *ui;

    GenerationState *m_state;
    QPointer<MazeScene> m_scene;
    QPointer<ParametersWidget> m_paramWidget;
    QPointer<QSlider> m_runParamsSlider;
};

#endif // MAINWINDOW_H
