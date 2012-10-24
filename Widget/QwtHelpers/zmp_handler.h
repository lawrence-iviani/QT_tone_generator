#ifndef ZMP_HANDLER_H
#define ZMP_HANDLER_H

#include <QObject>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>

class ZMP_Handler: public QObject
{
    Q_OBJECT
    public:
        ZMP_Handler(QwtPlotCanvas * canvas, QObject *parent = 0);

    signals:

    public slots:
        void enableZoomer();
        void enablePanner();

    private:
        //Tools for handle graph
        QwtPlotPanner * m_pan;
        QwtPlotMagnifier * m_magnifier;
        QwtPlotZoomer * m_zoom;
        bool m_togglePanZoom; //True, pan selected. False Zoom selected.
        void init(QwtPlotCanvas * canvas);
        void switchZP();
};

#endif // ZMP_HANDLER_H
