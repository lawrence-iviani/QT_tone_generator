#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "timeplotwidget.h"
#include "freqplotwidget.h"
#include "plotwidget.h"
#include "selectcurvewindowdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void newCurve();
    void removeCurve();
    void changedCurve(int index);
    void timeDataUpdated();

private:
    Ui::MainWindow *ui;
    TimePlotWidget * m_plotTime;
    FreqPlotWidget * m_plotFreq;
    QFrame * m_FramePlot;
    QFrame * m_FrameCurves;
    QFrame * m_FrameButton;
    QWidget * m_WidgetFreqPlot;
    QWidget * m_WidgetTimePlot;

    void setupCurves(SelectCurveWindowHelper * selectCurveHelper);//This method creates all the possible tone generator and generate the help for the dialog window
    GenericTimeData *  decodeSelectedCurve(SelectCurveWindowHelper * selectCurveHelper);

    int m_lastIndexCurve;//This is the last index in the combo box curve, we need to know what's what the last curve selected.
    int m_indexGenerator; //It needs only to name a new curve with a progressive id, ie curve_1, curve_2, etc...


   // GenericTimeData m_digestCurve; //This curve is a summarize of all the curve. The calculation is made by inerithed class

};

#endif // MAINWINDOW_H
