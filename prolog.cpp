//#include "prolog.h"

//#include <QDebug>



//void read_entrace(term_t entraceT, QPair<int,int>& entrace)
//{
//    qDebug() << "read_entrace";
//    term_t entraceX = PL_new_term_ref(),
//            entraceY = PL_new_term_ref();

//    if(!PL_get_arg(1,entraceT,entraceX))
//        Q_ASSERT(false);
//    if(!PL_get_arg(2,entraceT,entraceY))
//        Q_ASSERT(false);
//    if(!PL_get_integer(entraceX,&entrace.first))
//        Q_ASSERT(false);
//    if(!PL_get_integer(entraceY,&entrace.second))
//        Q_ASSERT(false);
//}

//void read_cell_var(term_t varT, bool& var)
//{
//    qDebug() << "read_cell_var";
//    char *s;
//    if(!PL_get_chars(varT, &s, CVT_ALL))
//        Q_ASSERT(false);
////    qDebug()<<s;

//    if(s[0]=='y')
//        var=true;
//    else if (s[0]=='n')
//        var=false;
//    else
//    {
//        qDebug() << s;
//        Q_ASSERT(false);
//    }

//    free(s);
//}

//void read_cell(term_t cellT, Cell &cell)
//{
//    qDebug() << "read_cell";
//    term_t rightT = PL_new_term_ref(), bottomT = PL_new_term_ref();

//    if(!PL_get_arg(1,cellT,rightT))
//        Q_ASSERT(false);
//    if(!PL_get_arg(2,cellT,bottomT))
//        Q_ASSERT(false);

//    read_cell_var(rightT,cell.right);
//    read_cell_var(bottomT,cell.bottom);
//}

//void read_euler_cell(term_t eulerCellT, Cell &cell)
//{
//    qDebug() << "read_euler_cell";
//    term_t cellT = PL_new_term_ref(), indexT = PL_new_term_ref();
//    qDebug() << pl_display(eulerCellT);

//    if(!PL_get_arg(1,eulerCellT,indexT))
//        Q_ASSERT(false);
//    if(!PL_get_arg(2,eulerCellT,cellT))
//        Q_ASSERT(false);
//    read_cell(cellT,cell);

//    cell.euler = true;
//    if(!PL_get_integer(indexT,&cell.index))
//        Q_ASSERT(false);
//}

//void read_euler_maze(term_t mazeT,QPair<int,int>&in, QPair<int,int>& out, QVector<QVector<Cell > > &matrix)
//{
//    qDebug() << "read_euler_maze";
//    term_t inT = PL_new_term_ref(),outT = PL_new_term_ref(),matrixLT = PL_new_term_ref();

//    if(!PL_get_arg(1,mazeT,inT))
//        Q_ASSERT(false);
//    read_entrace(inT,in);

//    if(!PL_get_arg(2,mazeT,outT))
//        Q_ASSERT(false);
//    read_entrace(outT,out);

//    if(!PL_get_arg(3,mazeT,matrixLT))
//        Q_ASSERT(false);

//    {
//        term_t head = PL_new_term_ref();
//        term_t list = PL_copy_term_ref(matrixLT);
//        int i = 0;
//        while( PL_get_list(list, head, list) )
//        {
//            QVector<Cell > row;
//            read_euler_row(head,row,i);
//            matrix.append(row);
//            ++i;
//        }
//    }
//}

//void read_euler_row(term_t head, QVector<Cell> &row,int row_index)
//{
//    qDebug() << "read_euler_row";
//    term_t eulerCellT = PL_new_term_ref();
//    term_t list1 = PL_copy_term_ref(head);
//    int j = 0;
//    while( PL_get_list(list1, eulerCellT, list1) )
//    {
//        Cell cell;
//        cell.r = row_index;
//        cell.c = j;

//        read_euler_cell(eulerCellT,cell);
//        row.append(cell);
//        ++j;
//    }
//}

//void read_maze(term_t mazeT,QPair<int,int>&in, QPair<int,int>& out, QVector<QVector<Cell > > &matrix)
//{
//    term_t inT = PL_new_term_ref(),outT = PL_new_term_ref(),matrixLT = PL_new_term_ref();

//    if(!PL_get_arg(1,mazeT,inT))
//        Q_ASSERT(false);
//    read_entrace(inT,in);

//    if(!PL_get_arg(2,mazeT,outT))
//        Q_ASSERT(false);
//    read_entrace(outT,out);

//    if(!PL_get_arg(3,mazeT,matrixLT))
//        Q_ASSERT(false);

//    {
//        term_t head = PL_new_term_ref();
//        term_t list = PL_copy_term_ref(matrixLT);
//        int i = 0;
//        while( PL_get_list(list, head, list) )
//        {
//            QVector<Cell > row;

//            term_t cellT = PL_new_term_ref();
//            term_t list1 = PL_copy_term_ref(head);

//            int j = 0;
//            while( PL_get_list(list1, cellT, list1) )
//            {
//                Cell cell;
//                cell.r = i;
//                cell.c = j;

//                read_cell(cellT,cell);
//                row.append(cell);
//                ++j;
//            }
//            matrix.append(row);
//            ++i;
//        }
//    }
//}

//QString pl_display(term_t t)
//{
//    qDebug() << "pl_display";
//    QString res;
//    int n;
//    size_t len;
//    char *s;

//    switch( PL_term_type(t) )
//    {
//    case PL_VARIABLE:
//    case PL_ATOM:
//    case PL_INTEGER:
//    case PL_FLOAT:
//        if(!PL_get_chars(t, &s, CVT_ALL))
//            Q_ASSERT(false);
//        res=s;
//        break;
//    case PL_STRING:
//        if(!PL_get_string_chars(t, &s, &len))
//            Q_ASSERT(false);
//        res=QString('\"')+s+'\"';
//        break;
//    case PL_TERM:
//    {
//        term_t a = PL_new_term_ref();
//        atom_t name;
//        int arity;

//        if(!PL_get_name_arity(t, &name, &arity))
//            Q_ASSERT(false);
//        res+=PL_atom_chars(name);
//        if(arity>0)
//            res+="(";
//        for(n=1; n<=arity; n++)
//        {
//        if(!PL_get_arg(n, t, a))
//            Q_ASSERT(false);

//        if ( n > 1 )
//            res+=",";
//        res+=pl_display(a);
//        }
//        res+=")";
//        break;
//    }
//    case PL_LIST_PAIR:
//    {
//        term_t head = PL_new_term_ref();   /* the elements */
//        term_t list = PL_copy_term_ref(t); /* copy (we modify list) */

//        bool first = true;
//        int len = 0;
//        res+='[';
//        while( PL_get_list(list, head, list) )
//        {
//            len++;
//            if(!first)
//                res+=',';
//            res+=pl_display(head);
//            first=false;
//        }
//        res+=']';
//        break;
//    }
//    default:
//        qDebug() << "default "<<res;
//        return QString();                          /* should not happen */
//    }

//    return res;
//}

//QVector<QVector<Cell> > &convertRepresentation(QVector<QVector<Cell > >& matrix)
//{
//    qDebug() << "convertRepresentation";
//    int rows = matrix.size(),
//        columns = ( (rows)?(matrix[0].size()):(0) );

//    QVector<bool> lastRowB(columns);
//    bool lastCellR;

//    for(int i = 0; i<rows; ++i)
//    {
//        for(int j=0; j<columns; ++j)
//        {
//            Cell &cell = matrix[i][j];

//            cell.left = (!j || lastCellR);
//            cell.right = (j==columns-1 || cell.right);
//            cell.top = (!i || lastRowB[j]);
//            cell.bottom = (i==rows-1 || cell.bottom);

//            lastRowB[j] = cell.bottom;
//            lastCellR = cell.right;
//        }
//    }

//    return matrix;
//}
