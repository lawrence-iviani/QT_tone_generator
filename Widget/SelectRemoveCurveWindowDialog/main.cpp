#include <QtGui/QApplication>
#include "selectremovecurvewindowdialog.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList list;
    list << "a" << "b" << "c";
    SelectRemoveCurveWindowDialog * w=new SelectRemoveCurveWindowDialog(&list);
    w->show();

    a.exec();

    foreach (int i, w->getRemoveCurvesIndex())
        qDebug() << "Selected item " <<i;

}
