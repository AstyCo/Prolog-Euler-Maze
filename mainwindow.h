#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mazescene.h"

#include <QMainWindow>


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

    void generateMaze();


    ~MainWindow();

private slots:
    void on_actionGenerate_triggered();

private:
    void installMenus();


    Ui::MainWindow *ui;

    QPointer<MazeScene> m_scene;
    QPointer<ParametersWidget> m_paramWidget;
    QPointer<QSlider> m_runParamsSlider;
};

#endif // MAINWINDOW_H
