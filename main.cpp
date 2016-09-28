#include "mainwindow.h"
#include <QApplication>

#include <QDebug>

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <SWI-Stream.h>

int main(int argc, char *argv[])
{
    try{
        char env[] = "SWI_HOME_DIR=C:\\Soft\\swipl32";
        putenv(env);
        static char * av []  =  {"libpl.dll", NULL} ;
        qDebug()<<env;
        if (PL_initialise(1 , av) == 0)
        {
            PL_halt(1);
            qDebug() << "lib initialize error -(";
        }
        else
            qDebug() << "lib initialize ok!";

    qDebug()<<"Ok";

    QApplication a(argc, argv);



    MainWindow w;
    w.show();

    return a.exec();
    }
    catch(PlError &error)
    {
        qDebug()<<"error: "<<error.message <<endl;
    }



    return -1;
}
