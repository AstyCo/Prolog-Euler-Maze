#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mazescene.h"
#include "generationstate.h"

#include <QMainWindow>
#include <QMutex>

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <SWI-Stream.h>


namespace Ui {
class MainWindow;
}

class ParametersWidget;
class QSlider;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void keyPressEvent(QKeyEvent *);

    static term_t row_to_term(const QVector<Cell> &row);
    static void read_entrace(term_t entraceT, QPair<int,int>& entrace);
    static void read_cell_var(term_t varT, bool& var);
    static void read_cell(term_t cellT, Cell &cell);
    static void read_euler_cell(term_t eulerCellT, Cell &cell);
    static void read_euler_maze(term_t mazeT,QPair<int,int>&in, QPair<int,int>& out, QVector<QVector<Cell > > &matrix);
    static void read_euler_row(term_t rowT,QVector<Cell> &row, int row_index);
    static void read_maze(term_t mazeT,QPair<int,int>&in, QPair<int,int>& out, QVector<QVector<Cell > > &matrix);
    static QSet<QPair<int,int> > readPath(term_t pathT);
    static QString pl_display(term_t t);
    static QVector<QVector<Cell> > &convertRepresentation(QVector<QVector<Cell > >& matrix);

public slots:
    void initState();
    bool showPath(bool) const;

private slots:
    void makeStep(bool lock = true);
    void generateMaze();
    void on_actionInteractive_toggled(bool arg1);

    void on_actionRe_triggered();

    void on_actionRun_triggered();
    void onStateChanged(GenerationState::State);



private:
    void initScene();
    void installMenus();
    QSet<QPair<int,int> > getPath(term_t maze) const;



    Ui::MainWindow *ui;

    GenerationState *m_state;
    QPointer<MazeScene> m_scene;
    QPointer<ParametersWidget> m_paramWidget;
    QPointer<QSlider> m_runParamsSlider;
    QPointer<QTimer> m_timer;
    QMutex m_mutex;
};

#endif // MAINWINDOW_H
