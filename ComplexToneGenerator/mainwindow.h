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
#include <QTreeWidget>

//ALL THESE INCLUDES NEED TO BE REVIEWED
#include "plotwidget/timeplotwidget.h"
#include "plotwidget/freqplotwidget.h"
#include "plotwidget/plotwidget.h"
#include "CustomCurves/customcurvefactory.h"
#include "exportaudiofiledialog.h"
#include "selectmultiplecurveswindowdialog.h"
#include <errormessage.h>
#include "widgetstyle.h"


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
    void addCurve(GenericTimeData* gtd);
    void removeCurve();
    void exportDigestCurve();
    void updateCurvesName();
    void timeDataUpdated();
    void digestCurveChanged();
    void sampleRateChange(qreal SR);
    void streamPositionUpdate(qreal position);
    void removeAllCurves();
    void removeAllCurvesWithDialog();
    void duplicateCurves();
    void importCurve();
    void newProject();
    void load();
    void save();
    void saveAs();

private:
    void setupUI();
    void setupSplitters();
    QFrame *setupOptionsFrame();
    void setupPlots();
    QFrame *createButtonsFrame();
    void initAudio();
    void connectSignals();
    void connectMenusAndShortcut();
     bool importDomDocument(const QDomDocument& doc, ErrorMessage *err=NULL);


    QDomDocument composeDomDocument();
    void setupStyle();

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
        QFrame *toolboxOptionFrame;
        QFrame *buttonsFrame;
    } s_widgetUI;
    struct {
        QPushButton *addCurve;
        QPushButton *removeCurve;
        QPushButton *duplicateCurves;
#ifdef COMPLETE_FAST_SELECTION
        QPushButton *removeAllCurves;
        QPushButton *exportDigest;
        QPushButton *exportXML;
        QPushButton *importXML;
        QPushButton *showXML;
#endif
    } s_button;

    QTreeWidget * m_TreeWidgetshowXML;
    WidgetStyle m_widgetStyleUI;
    WidgetStyle m_widgetStylePlot;
};

#endif // MAINWINDOW_H
