#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QLayout>
#include <QVBoxLayout>
#include <QToolBox>

#include <audioplayer.h>
#include <sndfile.hh>
#include "plotwidget/timeplotwidget.h"
#include "plotwidget/freqplotwidget.h"

//ALL THESE INCLUDES NEED TO BE REVIEWED
#include "plotwidget/timeplotwidget.h"
#include "plotwidget/freqplotwidget.h"
#include "plotwidget/plotwidget.h"
#include "CustomCurves/genericsinusdata.h"
#include "CustomCurves/partialsinusdata.h"
#include "CustomCurves/repeatedsinusdata.h"
#include "CustomCurves/constanttimedata.h"
#include "CustomCurves/repeatedsinusdata.h"
#include "selectcurvewindowdialog.h"
#include "selectremovecurvewindowdialog.h"
#include "exportaudiofiledialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void newCurve();
    void removeCurve();
    void exportDigestCurve();
    void updateCurvesName();
    void timeDataUpdated();
    void digestCurveChanged();
    void streamPositionUpdate(qreal position);

private:
    void setupUI();
    void setupSplitters();
    void setupOptionsWidget();
    void setupPlots();
    QWidget *createButtonSWidget();
    void initAudio();
    void connectSignals();

    //THESE FUNCTION MUST BE INSERTED IN SOME KIND OF EXTERNAL UTILITY
    void setupCurves(SelectCurveWindowHelper * selectCurveHelper);//This method creates all the possible tone generator and generate the help for the dialog window
    GenericTimeData *  decodeSelectedCurve(SelectCurveWindowHelper * selectCurveHelper);


    //Streaming stuff
    AudioPlayer * m_audioPlayer;
    InternalStreamDevice * m_digestCurveStream;

    //Plots
    TimePlotWidget * m_plotTime;
    FreqPlotWidget * m_plotFreq;

    //Internal counters
    int m_indexGenerator; //It needs only to name a new curve with a progressive id, ie curve_1, curve_2, etc...
    int m_toolBoxFixedItem; //The number of fixed item for general purpose control, useful to know the first item used for a curve.

    //UI Stuff
    Ui::MainWindow *ui;
    struct {
        QSplitter *plotSplitter;
        QSplitter *commandSplitter;
        QSplitter *globalSplitter;
        TimePlotWidget *m_plotTime;
        FreqPlotWidget *m_plotFreq;
        QToolBox *toolboxOption;
        QWidget *createButtonSWidget;
    } s_widgetUI;
    struct {
        QPushButton *addCurve;
        QPushButton *removeCurve;
        QPushButton *exportDigest;
    } s_button;
};

#endif // MAINWINDOW_H
