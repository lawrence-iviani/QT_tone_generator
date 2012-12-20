//#include <QtGui/QApplication>
#include "ctg_app.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  //  QApplication a(argc, argv);
    CTG_app a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
