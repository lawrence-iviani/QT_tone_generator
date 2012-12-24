/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Dec 24 13:27:47 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave_project;
    QAction *actionLoad_Project;
    QAction *actionImport_curve;
    QAction *actionExport_audio_file;
    QAction *actionRemove_all_curves;
    QAction *actionRemove_curve;
    QAction *actionAdd_curve;
    QAction *actionDuplicate_curves;
    QAction *actionAbout_CTG;
    QAction *actionNew_Project;
    QAction *actionShow_Proj_struct;
    QAction *actionSave_as;
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *globalGridLayout;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuCurves;
    QMenu *menuAbout;
    QMenu *menuShow;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(644, 486);
        actionSave_project = new QAction(MainWindow);
        actionSave_project->setObjectName(QString::fromUtf8("actionSave_project"));
        actionLoad_Project = new QAction(MainWindow);
        actionLoad_Project->setObjectName(QString::fromUtf8("actionLoad_Project"));
        actionImport_curve = new QAction(MainWindow);
        actionImport_curve->setObjectName(QString::fromUtf8("actionImport_curve"));
        actionExport_audio_file = new QAction(MainWindow);
        actionExport_audio_file->setObjectName(QString::fromUtf8("actionExport_audio_file"));
        actionRemove_all_curves = new QAction(MainWindow);
        actionRemove_all_curves->setObjectName(QString::fromUtf8("actionRemove_all_curves"));
        actionRemove_curve = new QAction(MainWindow);
        actionRemove_curve->setObjectName(QString::fromUtf8("actionRemove_curve"));
        actionAdd_curve = new QAction(MainWindow);
        actionAdd_curve->setObjectName(QString::fromUtf8("actionAdd_curve"));
        actionDuplicate_curves = new QAction(MainWindow);
        actionDuplicate_curves->setObjectName(QString::fromUtf8("actionDuplicate_curves"));
        actionAbout_CTG = new QAction(MainWindow);
        actionAbout_CTG->setObjectName(QString::fromUtf8("actionAbout_CTG"));
        actionNew_Project = new QAction(MainWindow);
        actionNew_Project->setObjectName(QString::fromUtf8("actionNew_Project"));
        actionShow_Proj_struct = new QAction(MainWindow);
        actionShow_Proj_struct->setObjectName(QString::fromUtf8("actionShow_Proj_struct"));
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(0, 0, 641, 461));
        globalGridLayout = new QGridLayout(gridLayoutWidget);
        globalGridLayout->setObjectName(QString::fromUtf8("globalGridLayout"));
        globalGridLayout->setContentsMargins(0, 0, 0, 0);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 644, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuCurves = new QMenu(menubar);
        menuCurves->setObjectName(QString::fromUtf8("menuCurves"));
        menuAbout = new QMenu(menubar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        menuShow = new QMenu(menubar);
        menuShow->setObjectName(QString::fromUtf8("menuShow"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuCurves->menuAction());
        menubar->addAction(menuShow->menuAction());
        menubar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionNew_Project);
        menuFile->addAction(actionLoad_Project);
        menuFile->addAction(actionSave_project);
        menuFile->addAction(actionSave_as);
        menuFile->addAction(actionImport_curve);
        menuFile->addAction(actionExport_audio_file);
        menuCurves->addAction(actionAdd_curve);
        menuCurves->addAction(actionDuplicate_curves);
        menuCurves->addAction(actionRemove_all_curves);
        menuCurves->addAction(actionRemove_curve);
        menuAbout->addAction(actionAbout_CTG);
        menuShow->addAction(actionShow_Proj_struct);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionSave_project->setText(QApplication::translate("MainWindow", "Save project", 0, QApplication::UnicodeUTF8));
        actionLoad_Project->setText(QApplication::translate("MainWindow", "Load Project", 0, QApplication::UnicodeUTF8));
        actionImport_curve->setText(QApplication::translate("MainWindow", "Import curve", 0, QApplication::UnicodeUTF8));
        actionExport_audio_file->setText(QApplication::translate("MainWindow", "Export audio file...", 0, QApplication::UnicodeUTF8));
        actionRemove_all_curves->setText(QApplication::translate("MainWindow", "Remove all curves", 0, QApplication::UnicodeUTF8));
        actionRemove_curve->setText(QApplication::translate("MainWindow", "Remove curve", 0, QApplication::UnicodeUTF8));
        actionAdd_curve->setText(QApplication::translate("MainWindow", "Add curve", 0, QApplication::UnicodeUTF8));
        actionDuplicate_curves->setText(QApplication::translate("MainWindow", "Duplicate curves", 0, QApplication::UnicodeUTF8));
        actionAbout_CTG->setText(QApplication::translate("MainWindow", "About CTG", 0, QApplication::UnicodeUTF8));
        actionNew_Project->setText(QApplication::translate("MainWindow", "New Project", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionNew_Project->setToolTip(QApplication::translate("MainWindow", "Create a new Project", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionShow_Proj_struct->setText(QApplication::translate("MainWindow", "Show Proj struct", 0, QApplication::UnicodeUTF8));
        actionSave_as->setText(QApplication::translate("MainWindow", "Save as...", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuCurves->setTitle(QApplication::translate("MainWindow", "Curves", 0, QApplication::UnicodeUTF8));
        menuAbout->setTitle(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        menuShow->setTitle(QApplication::translate("MainWindow", "Show", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
