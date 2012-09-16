#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ExportAudioFileDialog w;
    w.exec();
    QString file=w.getSelectedFileName();
    QString dir=w.getSelectedDirectoryName();
    int type=w.getSelecedType();

    qDebug() << "Select " << file << " in " << dir << " of type " << ExportAudioFileDialog::getNameFromType( type )<< " ";

}

MainWindow::~MainWindow()
{
    delete ui;
}
