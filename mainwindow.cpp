#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parameterswidget.h"
#include "cellobject.h"



#include <QPainter>
#include <QPaintEvent>
#include <QSlider>
#include <QWidgetAction>
#include <QToolButton>
#include <QGroupBox>


#include <QDebug>

QString pl_display(term_t t);

void MainWindow::read_entrace(term_t entraceT, QPair<int,int>& entrace)
{
    term_t entraceX = PL_new_term_ref(),
            entraceY = PL_new_term_ref();

    PL_get_arg(1,entraceT,entraceX);
    PL_get_arg(2,entraceT,entraceY);

    PL_get_integer(entraceX,&entrace.first);
    PL_get_integer(entraceY,&entrace.second);
}

void MainWindow::read_cell_var(term_t varT, bool& var)
{
    char *s;
    PL_get_chars(varT, &s, CVT_ALL);
//    qDebug()<<s;

    if(s[0]=='y')
        var=true;
    else if (s[0]=='n')
        var=false;
    else
    {
        qDebug() << s;
        Q_ASSERT(false);
    }

    free(s);
}

void MainWindow::read_cell(term_t cellT, Cell &cell)
{
    term_t rightT = PL_new_term_ref(), bottomT = PL_new_term_ref();

    PL_get_arg(1,cellT,rightT);
    PL_get_arg(2,cellT,bottomT);

    read_cell_var(rightT,cell.right);
    read_cell_var(bottomT,cell.bottom);
}

void MainWindow::read_euler_cell(term_t eulerCellT, Cell &cell)
{
    term_t cellT = PL_new_term_ref(), indexT = PL_new_term_ref();

    PL_get_arg(1,eulerCellT,indexT);
    PL_get_arg(2,eulerCellT,cellT);

    read_cell(cellT,cell);

    cell.euler = true;
    PL_get_integer(indexT,&cell.index);
}

void MainWindow::read_euler_maze(term_t mazeT,QPair<int,int>&in, QPair<int,int>& out, QVector<QVector<Cell > > &matrix)
{
    term_t inT = PL_new_term_ref(),outT = PL_new_term_ref(),matrixLT = PL_new_term_ref();

    PL_get_arg(1,mazeT,inT);
        read_entrace(inT,in);

    PL_get_arg(2,mazeT,outT);
        read_entrace(outT,out);

    PL_get_arg(3,mazeT,matrixLT);

    {
        term_t head = PL_new_term_ref();
        term_t list = PL_copy_term_ref(matrixLT);
        int i = 0;
        while( PL_get_list(list, head, list) )
        {
            QVector<Cell > row;
            read_euler_row(head,row,i);
            matrix.append(row);
            ++i;
        }
    }
}

void MainWindow::read_euler_row(term_t head, QVector<Cell> &row,int row_index)
{
    term_t eulerCellT = PL_new_term_ref();
    term_t list1 = PL_copy_term_ref(head);
    int j = 0;
    while( PL_get_list(list1, eulerCellT, list1) )
    {
        Cell cell;
        cell.r = row_index;
        cell.c = j;

        read_euler_cell(eulerCellT,cell);
        row.append(cell);
        ++j;
    }
}

void MainWindow::read_maze(term_t mazeT,QPair<int,int>&in, QPair<int,int>& out, QVector<QVector<Cell > > &matrix)
{
    term_t inT = PL_new_term_ref(),outT = PL_new_term_ref(),matrixLT = PL_new_term_ref();

    PL_get_arg(1,mazeT,inT);
        read_entrace(inT,in);

    PL_get_arg(2,mazeT,outT);
        read_entrace(outT,out);

    PL_get_arg(3,mazeT,matrixLT);

    {
        term_t head = PL_new_term_ref();
        term_t list = PL_copy_term_ref(matrixLT);
        int i = 0;
        while( PL_get_list(list, head, list) )
        {
            QVector<Cell > row;

            term_t cellT = PL_new_term_ref();
            term_t list1 = PL_copy_term_ref(head);

            int j = 0;
            while( PL_get_list(list1, cellT, list1) )
            {
                Cell cell;
                cell.r = i;
                cell.c = j;

                read_cell(cellT,cell);
                row.append(cell);
                ++j;
            }
            matrix.append(row);
            ++i;
        }
    }
}

QString MainWindow::pl_display(term_t t)
{
    QString res;
    functor_t functor;
    int arity, n;
    size_t len;
    char *s;

    switch( PL_term_type(t) )
    {
    case PL_VARIABLE:
    case PL_ATOM:
    case PL_INTEGER:
    case PL_FLOAT:
        PL_get_chars(t, &s, CVT_ALL);
        res=s;
        break;
    case PL_STRING:
        PL_get_string_chars(t, &s, &len);
        res=QString('\"')+s+'\"';
        break;
    case PL_TERM:
    {
        term_t a = PL_new_term_ref();
        atom_t name;
        int arity;

        PL_get_name_arity(t, &name, &arity);
        res+=PL_atom_chars(name);
        if(arity>0)
            res+="(";
        for(n=1; n<=arity; n++)
        {
        PL_get_arg(n, t, a);

        if ( n > 1 )
            res+=",";
        res+=pl_display(a);
        }
        res+=")";
        break;
    }
    case PL_LIST_PAIR:
    {
        term_t head = PL_new_term_ref();   /* the elements */
        term_t list = PL_copy_term_ref(t); /* copy (we modify list) */

        bool first = true;
        int len = 0;
        res+='[';
        while( PL_get_list(list, head, list) )
        {
            len++;
            if(!first)
                res+=',';
            res+=pl_display(head);
            first=false;
        }
        res+=']';
        break;
    }
    default:
        qDebug() << "default "<<res;
        return QString();                          /* should not happen */
    }

    return res;
}

QVector<QVector<Cell> > &MainWindow::convertRepresentation(QVector<QVector<Cell > >& matrix)
{
    int rows = matrix.size(),
        columns = ( (rows)?(matrix[0].size()):(0) );

    QVector<bool> lastRowB(columns);
    bool lastCellR;

    for(int i = 0; i<rows; ++i)
    {
        for(int j=0; j<columns; ++j)
        {
            Cell &cell = matrix[i][j];

            cell.left = (!j || lastCellR);
            cell.right = (j==columns-1 || cell.right);
            cell.top = (!i || lastRowB[j]);
            cell.bottom = (i==rows-1 || cell.bottom);

            lastRowB[j] = cell.bottom;
            lastCellR = cell.right;
        }
    }

    return matrix;
}

void MainWindow::makeStep()
{

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    installMenus();
}

void MainWindow::generateMaze()
{
    qDebug() << "generateMaze";

    PlCall("consult(\'../Prolog-Euler-Maze/maze.pl\')");
    term_t maze=PL_new_term_ref(),ans=PL_new_term_ref(),cols=PL_new_term_ref(),rows=PL_new_term_ref();

    PL_put_integer(rows,m_paramWidget->row());
    PL_put_integer(cols,m_paramWidget->col());


    functor_t mazeGenerator = PL_new_functor(PL_new_atom("eulerMazeGenerator"),3);
    if(!PL_cons_functor(ans,mazeGenerator,rows,cols,maze))
        qDebug() << "!PL_cons_functor(ans,mazeGenerator,rows,cols,maze)";
    if(PL_call(ans,NULL))
    {
        QPair<int,int> in, out;
        QVector<QVector<Cell > > matrix;


        read_euler_maze(maze, in, out, matrix);

        if(m_scene.isNull())
        {
            m_scene = new MazeScene(this);
            ui->graphicsView1->setScene(m_scene);
            ui->graphicsView2->setScene(m_scene);

            m_scene->setViewportMode(ui->graphicsView1->viewport(),MazeScene::mazeNoInfo);
            m_scene->setViewportMode(ui->graphicsView2->viewport(),MazeScene::mazeWithInfo);
        }
        if(!(m_scene->rows()==m_paramWidget->row() && m_scene->columns()==m_paramWidget->col()))
            m_scene->initScene(m_paramWidget->row(),m_paramWidget->col());
        m_scene->updateItems(convertRepresentation(matrix));

        qDebug() << in << out;

        m_scene->clearEntraces();
        m_scene->setEntrace(in.first,in.second);
        m_scene->setEntrace(out.first,out.second);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::installMenus()
{
    // MAZE PARAMETERS INSTALL
    QWidgetAction *p_action = new QWidgetAction(this);
    m_paramWidget = new ParametersWidget(this);
    m_paramWidget->setRow(20);
    m_paramWidget->setCol(30);

    p_action->setDefaultWidget(m_paramWidget);
    ui->menuParameters->addAction(p_action);


    // RUN PARAMETERS INSTALL
    QWidgetAction *rp_action = new QWidgetAction(this);

    QGroupBox* speedSelectionWidget = new QGroupBox(tr("Generation speed"),ui->toolBar);
    QSlider *wd = new QSlider();
    wd->setOrientation(Qt::Horizontal);
    wd->setMinimum(0);
    wd->setMaximum(100);


    m_runParamsSlider = new QSlider(speedSelectionWidget);

    setStyleSheet(
                "QSlider::groove:horizontal {"
                    "border: 1px solid #999999;"
                    "height: 8px;"
                    "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);"
                    "margin: 2px 0;"
                "}"

                "QSlider::handle:horizontal {"
                  "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                  "border: 1px solid #0c457e;"
                  "width: 18px;"
                  "margin: -2px 0;"
                  "border-radius: 2px;"
                  "}");

    m_runParamsSlider->setOrientation(Qt::Horizontal);
    m_runParamsSlider->setMinimum(0);
    m_runParamsSlider->setMaximum(100);

    QVBoxLayout *gs_layout = new QVBoxLayout;
    gs_layout->addWidget(m_runParamsSlider);
    speedSelectionWidget->setLayout(gs_layout);

    rp_action->setDefaultWidget(speedSelectionWidget);
    QMenu *runMenu = new QMenu(ui->toolBar);
    runMenu->addAction(rp_action);
    ui->actionRun->setMenu(runMenu);

    QToolButton *p_button = qobject_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->actionRun));
    if(p_button)
        p_button->setPopupMode(QToolButton::InstantPopup);
}

void MainWindow::on_actionGenerate_triggered()
{
    generateMaze();
}
