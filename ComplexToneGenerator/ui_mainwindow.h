/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Nov 9 22:06:21 2012
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
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QToolBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayoutCentralWidget;
    QFrame *framePlot;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *layoutPlotFrame;
    QWidget *widgetTimePlot;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *layoutTimePlot;
    QFrame *frameControlsTime;
    QWidget *widgetFreqPlot;
    QWidget *verticalLayoutWidget_5;
    QVBoxLayout *layoutFreqPlot;
    QFrame *frameControlsFreq;
    QFrame *frameCurves;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *layoutActualCurvesFrame;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *verticalLayoutWidget_6;
    QVBoxLayout *scrollAreaLayout;
    QToolBox *toolBoxOptions;
    QWidget *page_2;
    QFrame *frameButton;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *layoutButtonFrame;
    QPushButton *pushButtonRemoveCurve;
    QPushButton *pushButtonAddCurve;
    QPushButton *pushButtonExportDigetCurve;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(746, 593);
        MainWindow->setTabShape(QTabWidget::Rounded);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayoutWidget = new QWidget(centralWidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 711, 551));
        gridLayoutCentralWidget = new QGridLayout(gridLayoutWidget);
        gridLayoutCentralWidget->setSpacing(6);
        gridLayoutCentralWidget->setContentsMargins(11, 11, 11, 11);
        gridLayoutCentralWidget->setObjectName(QString::fromUtf8("gridLayoutCentralWidget"));
        gridLayoutCentralWidget->setContentsMargins(0, 0, 0, 0);
        framePlot = new QFrame(gridLayoutWidget);
        framePlot->setObjectName(QString::fromUtf8("framePlot"));
        framePlot->setFrameShape(QFrame::WinPanel);
        framePlot->setFrameShadow(QFrame::Raised);
        framePlot->setLineWidth(2);
        verticalLayoutWidget_2 = new QWidget(framePlot);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(-1, -1, 551, 541));
        layoutPlotFrame = new QVBoxLayout(verticalLayoutWidget_2);
        layoutPlotFrame->setSpacing(2);
        layoutPlotFrame->setContentsMargins(5, 5, 5, 5);
        layoutPlotFrame->setObjectName(QString::fromUtf8("layoutPlotFrame"));
        layoutPlotFrame->setContentsMargins(0, 0, 0, 0);
        widgetTimePlot = new QWidget(verticalLayoutWidget_2);
        widgetTimePlot->setObjectName(QString::fromUtf8("widgetTimePlot"));
        verticalLayoutWidget_4 = new QWidget(widgetTimePlot);
        verticalLayoutWidget_4->setObjectName(QString::fromUtf8("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(0, 0, 541, 131));
        layoutTimePlot = new QVBoxLayout(verticalLayoutWidget_4);
        layoutTimePlot->setSpacing(6);
        layoutTimePlot->setContentsMargins(11, 11, 11, 11);
        layoutTimePlot->setObjectName(QString::fromUtf8("layoutTimePlot"));
        layoutTimePlot->setContentsMargins(0, 0, 0, 0);

        layoutPlotFrame->addWidget(widgetTimePlot);

        frameControlsTime = new QFrame(verticalLayoutWidget_2);
        frameControlsTime->setObjectName(QString::fromUtf8("frameControlsTime"));
        frameControlsTime->setFrameShape(QFrame::Box);
        frameControlsTime->setFrameShadow(QFrame::Raised);
        frameControlsTime->setLineWidth(2);
        frameControlsTime->setMidLineWidth(0);

        layoutPlotFrame->addWidget(frameControlsTime);

        widgetFreqPlot = new QWidget(verticalLayoutWidget_2);
        widgetFreqPlot->setObjectName(QString::fromUtf8("widgetFreqPlot"));
        verticalLayoutWidget_5 = new QWidget(widgetFreqPlot);
        verticalLayoutWidget_5->setObjectName(QString::fromUtf8("verticalLayoutWidget_5"));
        verticalLayoutWidget_5->setGeometry(QRect(0, 0, 541, 341));
        layoutFreqPlot = new QVBoxLayout(verticalLayoutWidget_5);
        layoutFreqPlot->setSpacing(6);
        layoutFreqPlot->setContentsMargins(11, 11, 11, 11);
        layoutFreqPlot->setObjectName(QString::fromUtf8("layoutFreqPlot"));
        layoutFreqPlot->setContentsMargins(0, 0, 0, 0);

        layoutPlotFrame->addWidget(widgetFreqPlot);

        frameControlsFreq = new QFrame(verticalLayoutWidget_2);
        frameControlsFreq->setObjectName(QString::fromUtf8("frameControlsFreq"));
        frameControlsFreq->setFrameShape(QFrame::Box);
        frameControlsFreq->setFrameShadow(QFrame::Raised);
        frameControlsFreq->setLineWidth(2);
        frameControlsFreq->setMidLineWidth(0);

        layoutPlotFrame->addWidget(frameControlsFreq);

        layoutPlotFrame->setStretch(0, 25);
        layoutPlotFrame->setStretch(1, 5);
        layoutPlotFrame->setStretch(2, 65);
        layoutPlotFrame->setStretch(3, 5);

        gridLayoutCentralWidget->addWidget(framePlot, 0, 0, 2, 1);

        frameCurves = new QFrame(gridLayoutWidget);
        frameCurves->setObjectName(QString::fromUtf8("frameCurves"));
        frameCurves->setFrameShape(QFrame::WinPanel);
        frameCurves->setFrameShadow(QFrame::Raised);
        frameCurves->setLineWidth(2);
        verticalLayoutWidget = new QWidget(frameCurves);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 131, 391));
        layoutActualCurvesFrame = new QVBoxLayout(verticalLayoutWidget);
        layoutActualCurvesFrame->setSpacing(6);
        layoutActualCurvesFrame->setContentsMargins(11, 11, 11, 11);
        layoutActualCurvesFrame->setObjectName(QString::fromUtf8("layoutActualCurvesFrame"));
        layoutActualCurvesFrame->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(verticalLayoutWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 127, 387));
        verticalLayoutWidget_6 = new QWidget(scrollAreaWidgetContents);
        verticalLayoutWidget_6->setObjectName(QString::fromUtf8("verticalLayoutWidget_6"));
        verticalLayoutWidget_6->setGeometry(QRect(0, 0, 121, 361));
        scrollAreaLayout = new QVBoxLayout(verticalLayoutWidget_6);
        scrollAreaLayout->setSpacing(6);
        scrollAreaLayout->setContentsMargins(11, 11, 11, 11);
        scrollAreaLayout->setObjectName(QString::fromUtf8("scrollAreaLayout"));
        scrollAreaLayout->setContentsMargins(0, 0, 0, 0);
        toolBoxOptions = new QToolBox(verticalLayoutWidget_6);
        toolBoxOptions->setObjectName(QString::fromUtf8("toolBoxOptions"));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setEnabled(false);
        page_2->setGeometry(QRect(0, 0, 119, 325));
        toolBoxOptions->addItem(page_2, QString::fromUtf8("Page 2"));

        scrollAreaLayout->addWidget(toolBoxOptions);

        scrollArea->setWidget(scrollAreaWidgetContents);

        layoutActualCurvesFrame->addWidget(scrollArea);


        gridLayoutCentralWidget->addWidget(frameCurves, 0, 1, 1, 1);

        frameButton = new QFrame(gridLayoutWidget);
        frameButton->setObjectName(QString::fromUtf8("frameButton"));
        frameButton->setFrameShape(QFrame::WinPanel);
        frameButton->setFrameShadow(QFrame::Raised);
        frameButton->setLineWidth(2);
        verticalLayoutWidget_3 = new QWidget(frameButton);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(0, 0, 132, 131));
        layoutButtonFrame = new QVBoxLayout(verticalLayoutWidget_3);
        layoutButtonFrame->setSpacing(6);
        layoutButtonFrame->setContentsMargins(11, 11, 11, 11);
        layoutButtonFrame->setObjectName(QString::fromUtf8("layoutButtonFrame"));
        layoutButtonFrame->setContentsMargins(0, 0, 0, 0);
        pushButtonRemoveCurve = new QPushButton(verticalLayoutWidget_3);
        pushButtonRemoveCurve->setObjectName(QString::fromUtf8("pushButtonRemoveCurve"));

        layoutButtonFrame->addWidget(pushButtonRemoveCurve);

        pushButtonAddCurve = new QPushButton(verticalLayoutWidget_3);
        pushButtonAddCurve->setObjectName(QString::fromUtf8("pushButtonAddCurve"));

        layoutButtonFrame->addWidget(pushButtonAddCurve);

        pushButtonExportDigetCurve = new QPushButton(verticalLayoutWidget_3);
        pushButtonExportDigetCurve->setObjectName(QString::fromUtf8("pushButtonExportDigetCurve"));

        layoutButtonFrame->addWidget(pushButtonExportDigetCurve);


        gridLayoutCentralWidget->addWidget(frameButton, 1, 1, 1, 1);

        gridLayoutCentralWidget->setRowStretch(0, 3);
        gridLayoutCentralWidget->setRowStretch(1, 1);
        gridLayoutCentralWidget->setColumnStretch(0, 8);
        gridLayoutCentralWidget->setColumnStretch(1, 2);
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(pushButtonRemoveCurve, SIGNAL(clicked()), MainWindow, SLOT(newCurve()));
        QObject::connect(pushButtonAddCurve, SIGNAL(clicked()), MainWindow, SLOT(removeCurve()));
        QObject::connect(pushButtonExportDigetCurve, SIGNAL(clicked()), MainWindow, SLOT(exportDigestCurve()));

        toolBoxOptions->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        toolBoxOptions->setItemText(toolBoxOptions->indexOf(page_2), QApplication::translate("MainWindow", "Page 2", 0, QApplication::UnicodeUTF8));
        pushButtonRemoveCurve->setText(QApplication::translate("MainWindow", "Add curve", 0, QApplication::UnicodeUTF8));
        pushButtonAddCurve->setText(QApplication::translate("MainWindow", "Remove Curve", 0, QApplication::UnicodeUTF8));
        pushButtonExportDigetCurve->setText(QApplication::translate("MainWindow", "Export Digest", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
