#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parameterswidget.h"
#include "cellobject.h"
#include "generationstate.h"
#include "prolog.h"


#include <QPainter>
#include <QPaintEvent>
#include <QSlider>
#include <QWidgetAction>
#include <QToolButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QTimer>


#include <QDebug>

QString pl_display(term_t t);

void MainWindow::read_entrace(term_t entraceT, QPair<int,int>& entrace)
{
    term_t entraceX = PL_new_term_ref(),
            entraceY = PL_new_term_ref();

    if(!PL_get_arg(1,entraceT,entraceX))
        Q_ASSERT(false);
    if(!PL_get_arg(2,entraceT,entraceY))
        Q_ASSERT(false);
    if(!PL_get_integer(entraceX,&entrace.first))
        Q_ASSERT(false);
    if(!PL_get_integer(entraceY,&entrace.second))
        Q_ASSERT(false);
}

void MainWindow::read_cell_var(term_t varT, bool& var)
{
    char *s;
    if(!PL_get_chars(varT, &s, CVT_ALL))
        Q_ASSERT(false);
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

    if(!PL_get_arg(1,cellT,rightT))
        Q_ASSERT(false);
    if(!PL_get_arg(2,cellT,bottomT))
        Q_ASSERT(false);

    read_cell_var(rightT,cell.right);
    read_cell_var(bottomT,cell.bottom);
}

void MainWindow::read_euler_cell(term_t eulerCellT, Cell &cell)
{
    term_t cellT = PL_new_term_ref(), indexT = PL_new_term_ref();

    if(!PL_get_arg(1,eulerCellT,indexT))
        Q_ASSERT(false);
    if(!PL_get_arg(2,eulerCellT,cellT))
        Q_ASSERT(false);
//    qDebug() << pl_display(eulerCellT);
    read_cell(cellT,cell);

    cell.euler = true;
    if(!PL_get_integer(indexT,&cell.index))
        Q_ASSERT(false);
}

void MainWindow::read_euler_maze(term_t mazeT,QPair<int,int>&in, QPair<int,int>& out, QVector<QVector<Cell > > &matrix)
{
    term_t inT = PL_new_term_ref(),outT = PL_new_term_ref(),matrixLT = PL_new_term_ref();

    if(!PL_get_arg(1,mazeT,inT))
        Q_ASSERT(false);
    read_entrace(inT,in);

    if(!PL_get_arg(2,mazeT,outT))
        Q_ASSERT(false);
    read_entrace(outT,out);

    if(!PL_get_arg(3,mazeT,matrixLT))
        Q_ASSERT(false);

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

    if(!PL_get_arg(1,mazeT,inT))
        Q_ASSERT(false);
    read_entrace(inT,in);

    if(!PL_get_arg(2,mazeT,outT))
        Q_ASSERT(false);
    read_entrace(outT,out);

    if(!PL_get_arg(3,mazeT,matrixLT))
        Q_ASSERT(false);

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
    int n;
    size_t len;
    char *s;

    switch( PL_term_type(t) )
    {
    case PL_VARIABLE:
    case PL_ATOM:
    case PL_INTEGER:
    case PL_FLOAT:
        if(!PL_get_chars(t, &s, CVT_ALL))
            Q_ASSERT(false);
        res=s;
        break;
    case PL_STRING:
        if(!PL_get_string_chars(t, &s, &len))
            Q_ASSERT(false);
        res=QString('\"')+s+'\"';
        break;
    case PL_TERM:
    {
        term_t a = PL_new_term_ref();
        atom_t name;
        int arity;

        if(!PL_get_name_arity(t, &name, &arity))
            Q_ASSERT(false);
        res+=PL_atom_chars(name);
        if(arity>0)
            res+="(";
        for(n=1; n<=arity; n++)
        {
        if(!PL_get_arg(n, t, a))
            Q_ASSERT(false);

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

void MainWindow::makeStep(bool lock)
{
//    qDebug()<< "started";

    fid_t fid = PL_open_foreign_frame();

    if(!(ui->actionInteractive->isChecked()))
    {
        Q_ASSERT(false);
        return;
    }
    PlCall("consult(\'../Prolog-Euler-Maze/maze.pl\')");

    term_t rowT = PL_new_term_ref() ,
            ans =PL_new_term_ref();


    switch(m_state->state())
    {
    case GenerationState::completedState:
    {
        qDebug() << "completed";
        return;
    }
    case GenerationState::startState:
    {
        QPair<int,int> in,out;
        term_t inT = PL_new_term_ref(),outT = PL_new_term_ref();
        functor_t f = PL_new_functor(PL_new_atom("entraceGenerator"),4);

        term_t rT = PL_new_term_ref(),cT = PL_new_term_ref();

        if(!PL_put_integer(rT,m_paramWidget->row()))
            Q_ASSERT(false);;
        if(!PL_put_integer(cT,m_paramWidget->col()))
            Q_ASSERT(false);
        if(!PL_cons_functor(ans,f,rT,cT,inT,outT))
            Q_ASSERT(false);
        if(!PL_call(ans,NULL))
            Q_ASSERT(false);


        read_entrace(inT,in);
        read_entrace(outT,out);

        m_scene->clearEntraces();
        m_scene->setEntrace(in.first,in.second);
        m_scene->setEntrace(out.first,out.second);


        functor_t makeFirstHelperF = PL_new_functor(PL_new_atom("makeFirstHelper"),3);
        term_t colT = PL_new_term_ref(),
                indexT = PL_new_term_ref();

        if(!PL_put_integer(indexT,1))
            Q_ASSERT(false);
        if(!PL_put_integer(colT,m_paramWidget->col()))
            Q_ASSERT(false);

        if(!PL_cons_functor(ans,makeFirstHelperF,colT,indexT,rowT))
            Q_ASSERT(false);

        if(PL_call(ans,NULL))
        {
            QVector<Cell> row;
            read_euler_row(rowT,row,0);
            m_scene->appendRow(row);
            m_state->setLastRow(row);
        }
        m_state->setState(GenerationState::makeFirstHelperState);
        break;
    }
    case GenerationState::makeFirstHelperState:
    {
        functor_t setBordersF = PL_new_functor(PL_new_atom("setBorders"),2);
        if(!PL_cons_functor(ans,setBordersF,m_state->lastRowT(),rowT))
            Q_ASSERT(false);

        if(!PL_call(ans,NULL))
            Q_ASSERT(false);


        QVector<Cell> row;

        read_euler_row(rowT,row,0);

        m_scene->updateRow(row,0);
        m_state->onRowReaded(row);

        m_state->setState(GenerationState::setBordersState);
        break;
    }
    case GenerationState::setBordersState:
    {
        if(m_state->last())
        {
            m_state->setState(GenerationState::lastBodySetBottomBordersState);
            makeStep(false);
            if(lock)
                m_mutex.unlock();
            return;
        }
//        if(!PL_cons_functor(ans,m_state->m_newRowIndexesF,m_state->lastRowT(),rowT))
//            Q_ASSERT(false);

        term_t t0 = PL_new_term_refs(2),
                t1 = t0 + 1;
        t0 = PL_copy_term_ref(m_state->lastRowT());
        t1 = PL_copy_term_ref(rowT);
        predicate_t p = PL_pred(m_state->m_newRowIndexesF,NULL);
        if(!PL_call_predicate(NULL,PL_Q_NORMAL,p,t0))
            Q_ASSERT(false);

//        if(!PL_call(ans,NULL))
//            Q_ASSERT(false);
        QVector<Cell> row;

        read_euler_row(rowT,row,m_scene->objectsSize());

        m_scene->appendRow(row);
        m_state->setLastRow(row);

        m_state->setState(GenerationState::bodyNewRowIndexesState);
        break;
    }
    case GenerationState::bodyNewRowIndexesState:
    {
//        if(!PL_cons_functor(ans,m_state->m_setBottomBordersF,m_state->lastRowT(),rowT))
//            Q_ASSERT(false);

//        if(!PL_call(ans,NULL))
//            Q_ASSERT(false);

        term_t t0 = PL_new_term_refs(2),
                t1 = t0 + 1;
        t0 = PL_copy_term_ref(m_state->lastRowT());
        t1 = PL_copy_term_ref(rowT);
        predicate_t p = PL_pred(m_state->m_setBottomBordersF,NULL);
        if(!PL_call_predicate(NULL,PL_Q_NORMAL,p,t0))
            Q_ASSERT(false);

        QVector<Cell> row;

        read_euler_row(rowT,row,m_scene->objectsSize()-1);

        m_scene->updateRow(row,m_scene->objectsSize()-1);
        m_state->onRowReaded(row);

        m_state->setState(GenerationState::setBordersState);
        break;
    }
    case GenerationState::lastBodySetBottomBordersState:
    {
        if(!PL_cons_functor(ans,m_state->m_newRowIndexesF,m_state->lastRowT(),rowT))
            Q_ASSERT(false);

        if(!PL_call(ans,NULL))
            Q_ASSERT(false);

        QVector<Cell> row;

        read_euler_row(rowT,row,m_scene->objectsSize());

        m_scene->appendRow(row);
        m_state->setLastRow(row);

        m_state->setState(GenerationState::finishNewRowIndexesState);
        break;
    }
    case GenerationState::finishNewRowIndexesState:
    {
        functor_t f = PL_new_functor(PL_new_atom("finishRowH"),2);
        if(!PL_cons_functor(ans,f,m_state->lastRowT(),rowT))
            Q_ASSERT(false);

        if(!PL_call(ans,NULL))
            Q_ASSERT(false);

        QVector<Cell> row;

        read_euler_row(rowT,row,m_scene->objectsSize()-1);

        m_scene->updateRow(row,m_scene->objectsSize()-1);
        m_state->onRowReaded(row);

        m_state->setState(GenerationState::finishRowHState);
        break;
    }
    case GenerationState::finishRowHState:
    {
        m_scene->convertRepresentation();
        m_state->setState(GenerationState::completedState);
        return;
    }

    default:
        qDebug() << "default makeStep case";
        if(lock)
            m_mutex.unlock();
        return;
    }
    m_scene->convertRepresentation(true);
    if(lock)
        m_mutex.unlock();

    PL_close_foreign_frame(fid);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    setWindowTitle(QString::fromUtf8("Алгоритм Эллера [Qt & SWI-PROLOG]"));
    setWindowIcon(QIcon(":/resources/euler.bmp"));

    m_state = new GenerationState(this);
    m_timer = new QTimer(this);
    connect(ui->actionMake_step,SIGNAL(triggered(bool)),this,SLOT(makeStep()));
    connect(m_state,SIGNAL(stateChanged(GenerationState::State)),this,SLOT(onStateChanged(GenerationState::State)));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(makeStep()));
    initScene();
    installMenus();
}

void MainWindow::generateMaze()
{
    PlCall("consult(\'../Prolog-Euler-Maze/maze.pl\')");
    term_t maze=PL_new_term_ref(),ans=PL_new_term_ref(),cols=PL_new_term_ref(),rows=PL_new_term_ref();

    if(!PL_put_integer(rows,m_paramWidget->row()))
        Q_ASSERT(false);
    if(!PL_put_integer(cols,m_paramWidget->col()))
        Q_ASSERT(false);

    functor_t mazeGenerator = PL_new_functor(PL_new_atom("eulerMazeGenerator"),3);
    if(!PL_cons_functor(ans,mazeGenerator,rows,cols,maze))
        Q_ASSERT(false);

    if(PL_call(ans,NULL))
    {
        QPair<int,int> in, out;
        QVector<QVector<Cell > > matrix;

        read_euler_maze(maze, in, out, matrix);

        if(!(m_scene->objectsSize()==m_paramWidget->row() && m_scene->objectsCols()==m_paramWidget->col()))
            m_scene->initScene(m_paramWidget->row(),m_paramWidget->col());

        m_scene->updateItems(convertRepresentation(matrix));

        m_scene->clearEntraces();
        m_scene->setEntrace(in.first,in.second);
        m_scene->setEntrace(out.first,out.second);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *)
{
    QVector<Cell> testRow;
    Cell testCell1,testCell2;
    testCell1.euler = true;
    testCell1.index = 1;
    testCell1.r = 0;
    testCell1.c = 0;
    testCell2.euler = true;
    testCell2.index = 1;
    testCell2.r = 0;
    testCell2.c = 1;

    testRow.append(testCell1);
    testRow.append(testCell2);

    qDebug() << pl_display(row_to_term(testRow));
}

term_t MainWindow::row_to_term(const QVector<Cell> &row)
{
    functor_t  makeEulerCellF = PL_new_functor(PL_new_atom("makeEulerCell"),4);
    term_t l = PL_new_term_ref();
    PL_put_nil(l);

    for(int i = row.size() - 1; i>=0; --i)
    {
        Cell cell = row[i];
        term_t h = PL_new_term_ref(),
                indexT = PL_new_term_ref(),
                rightBorderT = PL_new_term_ref(),
                bottomBorderT = PL_new_term_ref(),
                ansT = PL_new_term_ref();

        if(!cell.euler)
            Q_ASSERT(false);
        if(!PL_put_integer(indexT,cell.index))
            Q_ASSERT(false);
        if(!PL_put_atom_chars(rightBorderT,(cell.right)?"y":"n"))
            Q_ASSERT(false);
        if(!PL_put_atom_chars(bottomBorderT,(cell.bottom)?"y":"n"))
            Q_ASSERT(false);

        if(!PL_cons_functor(ansT,makeEulerCellF,indexT,rightBorderT,bottomBorderT,h))
            Q_ASSERT(false);
        if(!PL_call(ansT,NULL))
            Q_ASSERT(false);

        if(!PL_cons_list(l,h,l))
            Q_ASSERT(false);
    }
    return l;
}

void MainWindow::installMenus()
{
    // MAZE PARAMETERS INSTALL
    QWidgetAction *p_action = new QWidgetAction(this);
    m_paramWidget = new ParametersWidget(this);

    connect(m_paramWidget.data(),SIGNAL(rowsChanged(int)),this,SLOT(initState()));
    connect(m_paramWidget.data(),SIGNAL(colsChanged(int)),this,SLOT(initState()));

    p_action->setDefaultWidget(m_paramWidget);
    ui->menuParameters->addAction(p_action);
    // ++
//    QWidget *igen = new QWidget(ui->menuParameters);
//    QHBoxLayout *lay = new QHBoxLayout(igen);

//    QCheckBox *checkBox = new QCheckBox();
//    lay->addWidget(checkBox);
//    QLabel *lab = new QLabel(igen);
//    lab->setText(QString::fromUtf8("Интерактивная генерация"));
//    lay->addWidget(lab);
//    igen->setLayout(lay);

//    QWidgetAction *checkableAction = new QWidgetAction(ui->menuParameters);
//    checkableAction->setDefaultWidget(igen);
//    ui->menuParameters->addAction(checkableAction);




    // RUN PARAMETERS INSTALL
    QWidgetAction *rp_action = new QWidgetAction(this);

    QGroupBox* speedSelectionWidget = new QGroupBox(QString::fromUtf8("Скорость генерации"),ui->toolBar);
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
    m_runParamsSlider->setMinimum(1);
    m_runParamsSlider->setMaximum(30);
    connect(m_runParamsSlider,SIGNAL(sliderMoved(int)),this,SLOT(on_actionRun_triggered()));

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

    // REMOVE INTERACTIVE ACTIONS FROM TOOLBAR (DEFAULT)
    ui->toolBar->removeAction(ui->actionRun);
    ui->toolBar->removeAction(ui->actionMake_step);

}

void MainWindow::initState()
{
    if(ui->actionInteractive->isChecked())
    {
        qDebug()<<"init state";
        if(m_scene && m_paramWidget)
        {
            m_scene->clear();
            m_scene->setRows(m_paramWidget->row());
            m_scene->setColumns(m_paramWidget->col());
        }
        if(m_state && m_paramWidget)
            m_state->initState(m_paramWidget->row());
    }
    else
        m_timer->stop();
}

void MainWindow::on_actionInteractive_toggled(bool val)
{
    if(val)
    {
        ui->toolBar->addAction(ui->actionRun);
        ui->toolBar->addAction(ui->actionMake_step);
    }
    else
    {
        ui->toolBar->removeAction(ui->actionRun);
        ui->toolBar->removeAction(ui->actionMake_step);
    }

    on_actionRe_triggered();
}

void MainWindow::on_actionRe_triggered()
{
    if(m_timer)
        m_timer->stop();

    if(ui->actionInteractive->isChecked())
    {
        initState();
        if(ui->actionRun->isChecked())
            ui->actionRun->setChecked(false);
    }
    else
    {
        generateMaze();
    }
}

void MainWindow::initScene()
{
    m_scene = new MazeScene(this);
    ui->graphicsView1->setScene(m_scene);
    ui->graphicsView2->setScene(m_scene);

    m_scene->setViewportMode(ui->graphicsView1->viewport(),MazeScene::mazeNoInfo);
    m_scene->setViewportMode(ui->graphicsView2->viewport(),MazeScene::mazeWithInfo);
}

void MainWindow::on_actionRun_triggered()
{
    qDebug() << "on_actionRun_triggered "<< m_runParamsSlider->value();
    if(!!m_timer.isNull())
        Q_ASSERT(false);
    if(!ui->actionInteractive->isChecked())
        Q_ASSERT(false);
    m_timer->stop();
    if(ui->actionRun->isChecked() && m_state && m_state->state() != GenerationState::completedState)
        m_timer->start(1000.0 / m_runParamsSlider->value());
}

void MainWindow::onStateChanged(GenerationState::State state)
{
    if(!!m_timer.isNull())
        Q_ASSERT(false);
    if(state == GenerationState::completedState)
        m_timer->stop();
}
