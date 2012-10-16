#ifndef ZMP_HANDLER_H
#define ZMP_HANDLER_H

#include <QObject>
#include <QColor>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>
#include <qwt_picker.h>
#include <qwt_picker_machine.h>

class ZMP_Handler: public QObject
{
    Q_OBJECT
    public:
        ZMP_Handler(QwtPlotCanvas * canvas, QObject *parent = 0);
        void setBaseColor(QColor color);
    signals:

    public slots:
        void disableAll();
        void enableZoomer();
        void enablePanner();
        void enablePicker();

    private:
        //Tools for handle graph
        QwtPlotPanner * m_pan;
        QwtPlotMagnifier * m_magnifier;
        QwtPlotZoomer * m_zoom;
        QwtPlotPicker * m_picker;
        //bool m_togglePanZoom; //True, pan selected. False Zoom selected.
        void init(QwtPlotCanvas * canvas);
};

#endif // ZMP_HANDLER_H
