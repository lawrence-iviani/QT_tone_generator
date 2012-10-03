#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Engine eng;
    AudioPlayer * ap=new AudioPlayer(parent);
    ap->setFileName("/Volumes/MYDATA/github/QT_tone_generator/AudioEngine/test-48k.wav");
    //this->setCentralWidget(ap);
    ap->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
