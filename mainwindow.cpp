#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <SWI-Stream.h>

#include <QPainter>
#include <QPaintEvent>


#include <QDebug>

QString pl_display(term_t t);

void read_entrace(term_t entraceT, QPair<int,int>& entrace)
{
    term_t entraceX = PL_new_term_ref(),
            entraceY = PL_new_term_ref();

    PL_get_arg(1,entraceT,entraceX);
    PL_get_arg(2,entraceT,entraceY);

    PL_get_integer(entraceX,&entrace.first);
    PL_get_integer(entraceY,&entrace.second);
}

void read_cell_var(term_t varT, bool& var)
{
    char *s;
    PL_get_chars(varT, &s, CVT_ALL);
    qDebug()<<s;

    if(s[0]=='y')
        var=true;
    else if (s[0]=='n')
        var=false;
    else
        Q_ASSERT(false);

    free(s);
}

void read_cell(term_t cellT, QPair<bool,bool>&cell)
{
    term_t rightT = PL_new_term_ref(), bottomT = PL_new_term_ref();

    PL_get_arg(1,cellT,rightT);
    PL_get_arg(2,cellT,bottomT);

    read_cell_var(rightT,cell.first);
    read_cell_var(bottomT,cell.second);
}

void read_maze(term_t mazeT,QPair<int,int>&in, QPair<int,int>& out, QVector<QVector<QPair<bool,bool> > > &matrix)
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

        while( PL_get_list(list, head, list) )
        {
            QVector<QPair<bool,bool> > row;

            term_t cellT = PL_new_term_ref();
            term_t list1 = PL_copy_term_ref(head);

            while( PL_get_list(list1, cellT, list1) )
            {
                QPair<bool,bool> cell;

                read_cell(cellT,cell);
                row.append(cell);
            }
            matrix.append(row);
        }
    }
}

QString pl_display(term_t t)
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


QVector<QVector<Cell> > convertMatrix(const QVector<QVector<QPair<bool,bool> > >& matrix)
{
    QVector<QVector<Cell> > res;

    int rows = matrix.size(),
        columns = ( (rows)?(matrix[0].size()):(0) );

    QVector<bool> lastRowB(columns);
    bool lastCellR;
    for(int i = 0; i<rows; ++i)
    {
        QVector<Cell> row;
        for(int j=0; j<columns; ++j)
        {
            QPair<bool,bool> cellM = matrix[i][j];

            Cell cell;

            cell.left = (!j || lastCellR);
            cell.right = (j==columns-1 || cellM.first);
            cell.top = (!i || lastRowB[j]);
            cell.bottom = (i==rows-1 || cellM.second);

            lastRowB[j] = cell.bottom;
            lastCellR = cell.right;
            row.append(cell);
        }
        res.append(row);
    }
    return res;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    PlCall(" consult( swi( 'swipl-win.rc' ) )"); //no needed
    PlCall("consult(\'../Prolog-Euler-Maze/maze.pl\')");
    term_t maze=PL_new_term_ref(),ans=PL_new_term_ref(),cols=PL_new_term_ref(),rows=PL_new_term_ref();

    int nRows = 30;
    int nColumns = 40;

    PL_put_integer(rows,nRows);
    PL_put_integer(cols,nColumns);


    functor_t mazeGenerator = PL_new_functor(PL_new_atom("mazeGenerator"),3);
    if(!PL_cons_functor(ans,mazeGenerator,rows,cols,maze))
        qDebug() << "!PL_cons_functor(ans,mazeGenerator,rows,cols,maze)";
    if(PL_call(ans,NULL))
    {
        qDebug() << "ans is NULL";
        QPair<int,int> in, out;
        QVector<QVector<QPair<bool,bool> > > matrix;


        read_maze(maze, in, out, matrix);

        qDebug() << pl_display(maze);



//        qDebug() << "in "<<in;
//        qDebug() << "out "<<out;
        qDebug() << "matrix "<<matrix;
//        qDebug() << "matrix size: "<< matrix[0][0];

        m_scene = new MazeScene(this);
        m_scene->initScene(nRows,nColumns);
        m_scene->updateItems(convertMatrix(matrix));

        m_scene->setEntrace(in.first,in.second);
        m_scene->setEntrace(out.first,out.second);

        ui->mazeView->setScene(m_scene);

    }
    else
        setHidden(true);





}

MainWindow::~MainWindow()
{
    delete ui;
}
