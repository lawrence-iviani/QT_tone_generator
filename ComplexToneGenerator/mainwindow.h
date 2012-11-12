#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
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
#include <sndfile.hh>
#include <audioplayer.h>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *widget = 0);
    ~MainWindow();

signals:

public slots:
    void newCurve();
    void removeCurve();
    void exportDigestCurve();
    void updateCurvesName();
    void timeDataUpdated();
    void digestCurveChanged();
    void streamPositionUpdate(qreal position);

private:
    Ui::MainWindow *ui;
    TimePlotWidget * m_plotTime;
    FreqPlotWidget * m_plotFreq;
    QFrame * m_FramePlot;
    QFrame * m_FrameCurves;
    QFrame * m_FrameButton;
    QWidget * m_WidgetFreqPlot;
    QWidget * m_WidgetTimePlot;

    AudioPlayer * m_audioPlayer;
    InternalStreamDevice * m_digestCurveStream;

    void setupCurves(SelectCurveWindowHelper * selectCurveHelper);//This method creates all the possible tone generator and generate the help for the dialog window
    GenericTimeData *  decodeSelectedCurve(SelectCurveWindowHelper * selectCurveHelper);

    int m_indexGenerator; //It needs only to name a new curve with a progressive id, ie curve_1, curve_2, etc...
    int m_toolBoxFixedItem; //The number of fixed item for general purpose control, useful to know the first item used for a curve.
};

#endif // MAINWINDOW_H
