#include "mainwindow.h"
#include "swipl32inputdialog.h"
#include <QApplication>

#include <QDebug>

#include <SWI-cpp.h>
#include <SWI-Prolog.h>
#include <SWI-Stream.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextStream ts( stdout );
    try{
//        char env[] = "SWI_HOME_DIR=D:\\Soft\\Prolog\\swipl32";
//        putenv(env);

        QByteArray path = qgetenv("SWI_HOME_DIR");
        if(path.isEmpty())
        {
            Swipl32InputDialog *dlg = new Swipl32InputDialog();
            if(dlg->exec())
            {
                path=dlg->sPathSwipl32().toLatin1();

                qputenv("SWI_HOME_DIR",path);
            }
            else
                return 1;
        }

        ts<<"Your path to swipl32/ is \"" << path <<'\"'<<endl;
        ts<<endl;
        ts<<"WARNING: If you see no dialog before crash, check your path environment SWI_HOME_DIR."<<endl;
        ts <<"WARNING: If you get "<<
            "\"SWI-Prolog: [FATAL ERROR: "
            "Could not find system resources]\" "<<
            "it is probably becouse of wrong swipl32/ path(notice x32)."<<endl;



        char * av []  =  {(char*)("libpl.dll"), NULL} ;

        if (PL_initialise(1 , av) == 0)
        {
//            PL_halt(1);
            ts << "lib initialize error."<<endl;
        }
        else
            ts << "lib initialize success."<<endl;

        MainWindow w;
        w.show();

        return a.exec();
    }
    catch(PlError &error)
    {
        ts << "error: "<<error.message <<endl;
    }
    catch(...)
    {
        ts << "error: "<<"..." <<endl;
    }



    return -1;
}
