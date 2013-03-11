#include <QtGui/QApplication>
#include "selectcurvewindowdialog.h"

void createExampleData(SelectCurveWindowHelper * h) {
    s_dataCurve t;
    t.name="Curve 1";
    t.description="This is description 1";
    h->addData(t);

    t.name="Curve 2";
    t.description="This is description 2";
    h->addData(t);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SelectCurveWindowHelper * h=SelectCurveWindowDialog::getDialogCurveHelper();
    createExampleData(h);
    SelectCurveWindowDialog * w=new SelectCurveWindowDialog(h);
    w->show();
    
    return a.exec();
}
