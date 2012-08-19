#include <QtGui/QApplication>
#include "comboboxnamewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ComboBoxNameWidget w;
    w.setText("Combo test");
    w.addItem("item 1");
    w.addItem("item 2");
    w.addItem("item 3");
    QFont f=*(new QFont());
    f.setPointSize(10);
    w.setFont(f);
    w.show();
    
    return a.exec();
}
