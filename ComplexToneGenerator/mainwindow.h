#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void dataUpdated();

private:
    Ui::MainWindow *ui;
    PlotWidget * m_plotTime;
    PlotWidget * m_plotFreq;
    QFrame * m_FramePlot;
    QFrame * m_FrameCurves;
    QFrame * m_FrameButton;
    QWidget * m_WidgetFreqPlot;
    QWidget * m_WidgetTimePlot;

    void setupCurves(SelectCurveWindowHelper * selectCurveHelper);//This method creates all the possible tone generator and generate the help for the dialog window
    GenericTimeData *  decodeSelectedCurve(SelectCurveWindowHelper * selectCurveHelper);

    int m_lastIndexCurve;//This is the last index in the combo box curve, we need to know what's what the last curve selected.
    int m_indexGenerator; //It needs only to name a new curve with a progressive id, ie curve_1, curve_2, etc...

    //static QString curveName={"Base curve","Tone generator"};
    //static QString curveDescription={"This curve generate a constant zero signal (not so useful :) )","This curve generate a pure tone"};
    //static int curveNumbers=2;

};

#endif // MAINWINDOW_H
