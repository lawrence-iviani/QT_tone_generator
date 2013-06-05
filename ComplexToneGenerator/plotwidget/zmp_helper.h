#ifndef ZMP_HELPER_H
#define ZMP_HELPER_H

// Zoom etc handler
#include <QObject>
#include <QColor>
#include <QEvent>
#include <QRadioButton>
#include <QGroupBox>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_layout.h>
#include <qwt_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_scale_widget.h>
#include "CTG_constants.h"
#include "plotwidget.h"

class PlotWidget;


class ZMP_helper: public QObject
{
    Q_OBJECT
public:
    ZMP_helper(PlotWidget *parentPlot);
    void setBaseColor(QColor color);
    QWidget* getControlWidget() {return ZMP_controlWidget;}
    void changeZoomBase(const QRectF& rect);
    void changeZoomBase(const QwtInterval& x, const QwtInterval& y);

signals:

public slots:
   // void axesChanged();
    void disableAll();
    void enableZoomer();
    void enablePanner();
    void enablePicker();

private:
    PlotWidget* m_parentPlot;
    //Tools for handle graph
    QwtPlotPanner * m_pan;
    QwtPlotMagnifier * m_magnifier;
    QwtPlotZoomer * m_zoom;
    QwtPlotPicker * m_picker;
    //bool m_togglePanZoom; //True, pan selected. False Zoom selected.
    void init();
    void initZMPControlWdiget();


    QWidget* ZMP_controlWidget;
    struct {
        QGroupBox* groupBox;
        QRadioButton* PICKER;
        QRadioButton* PAN;
        QRadioButton* ZOOM;
    } ZMP_RadioButton;//Zoom Move Pan Pick enable radio button
};

#endif // ZMP_HELPER_H
