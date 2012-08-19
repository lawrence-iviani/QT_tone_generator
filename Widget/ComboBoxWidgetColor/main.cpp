#include <QtGui/QApplication>
#include "comboboxwidgetcolor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ComboBoxWidgetColor w;
    QFont * f=new QFont();
    f->setPointSize(10);
    w.setFont(*f);
    w.setText("Box color");
    w.setColor(Qt::blue);
    w.show();
    
    return a.exec();
}
