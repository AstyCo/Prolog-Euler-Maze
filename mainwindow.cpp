#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <SWI-Stream.h>

#include <QDebug>


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
        return QString();                          /* should not happen */
    }

    return res;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    PlCall(" consult( swi( 'swipl-win.rc' ) )");
    PlCall("consult(\'../labyrinth/maze.pl\')");
    term_t maze=PL_new_term_ref(),ans=PL_new_term_ref(),cols=PL_new_term_ref(),rows=PL_new_term_ref();

    PL_put_integer(cols,500);
    PL_put_integer(rows,500);
    functor_t mazeGenerator = PL_new_functor(PL_new_atom("mazeGenerator"),3);
    if(!PL_cons_functor(ans,mazeGenerator,rows,cols,maze))
        qDebug() << "!PL_cons_functor(ans,mazeGenerator,rows,cols,maze)";
    if(PL_call(ans,NULL))
    {
        qDebug() << "ans is NULL";

        qDebug()<<pl_display(maze);

//        term_t head = PL_new_term_ref();   /* the elements */
//        term_t list = PL_copy_term_ref(maze); /* copy (we modify list) */

//        while( PL_get_list(maze, head, list) )
//        {
//            char *s;

//            if ( PL_get_atom_chars(head, &s) )
//                qDebug() <<"pl_get_atoms ok "<<s;
//            else
//                qDebug() <<"pl_get_atoms fail";
//        }


//        (char *)*mazeString = new char[10000];
//        PL_get_chars(maze,mazeString,CVT_ALL);
//        qDebug() << *mazeString;
    }





}

MainWindow::~MainWindow()
{
    delete ui;
}
