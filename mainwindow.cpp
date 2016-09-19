#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <SWI-Stream.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    PlCall(" consult( swi( 'swipl-win.rc' ) )");


}

MainWindow::~MainWindow()
{
    delete ui;
}
