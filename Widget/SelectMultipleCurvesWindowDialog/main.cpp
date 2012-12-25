#include <QApplication>
#include "selectmultiplecurveswindowdialog.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList list;
    list << "a" << "b" << "c";
    SelectMultipleCurvesWindowDialog * w=new SelectMultipleCurvesWindowDialog(&list);
    w->show();

    a.exec();

    foreach (int i, w->getRemoveCurvesIndex())
        qDebug() << "Selected item " <<i;

}
