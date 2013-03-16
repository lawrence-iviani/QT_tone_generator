#ifndef FREQPLOTWIDGET_H
#define FREQPLOTWIDGET_H

#include "plotwidget.h"
#include <qwt_plot_spectrogram.h>
#include <qwt_color_map.h>
#include <CTG_constants.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_plot_layout.h>
#include<complex>//needed before fftw, accordingly with the fftw documentation.
#include <fftw3.h>

const qreal FREQPLOTWIDGET_MAX_MAGNITUDE=0; //dB
const qreal FREQPLOTWIDGET_MIN_MAGNITUDE=-80; //dB
const uint FREQPLOTWIDGET_SAMPLES_PER_WINDOW=8192;

class SpectrogramData: public QwtRasterData
{

public:
    SpectrogramData(GenericTimeData* timeData=NULL);

    virtual ~SpectrogramData();

    void setData(const qreal * array, uint arraySize, uint numberOfSlice, uint samplesPerSlice);
    virtual double value(double x, double y) const;

private:
    void setIntervals();
    GenericTimeData *m_timeData;
    QwtInterval m_intervalFreq;
    QwtInterval m_intervalTime;
    QwtInterval m_intervalMagnitude;
 //   qreal m_stepFreq;
 //   qreal m_stepTime;
 //   qreal m_stepMagnitude;
    uint m_numberOfBins;
    uint m_numberOfSlices;

    uint m_sizeArray;
    double* m_array;
    double m_stepFreq;
    double m_stepTime;
};

class ColorMap: public QwtLinearColorMap
{
public:
    ColorMap():
        QwtLinearColorMap(Qt::darkBlue, Qt::red)
    {
        addColorStop(0.1, Qt::darkCyan);
        addColorStop(0.6, Qt::green);
        addColorStop(0.95, Qt::yellow);
    }
};

class FreqPlotWidget : public PlotWidget
{
    Q_OBJECT
public:
    explicit FreqPlotWidget(QWidget *widget = 0);
    QWidget * getControlWidget() {return m_allControl;}
    void setTimeData(GenericTimeData* timeData) {
        m_timeData=timeData;
        m_spectrogram.setData(new SpectrogramData(m_timeData));
    }

signals:
    
public slots:
    void dataUpdated();
private slots:
    void freqOptionPressed();

private:
    void createControlWidget();//Create the the base contro
    void initBaseControlWidget();

    struct {
      QFrame * baseControlWidget;
      QPushButton * toggleButtonOptionEnable;
    } m_baseControl;
    QFrame * m_allControl;//The widget with all control (base and eventually extended
    QwtPlotSpectrogram m_spectrogram;
    GenericTimeData *m_timeData;
    uint m_binsPerWindow;
};





#endif // FREQPLOTWIDGET_H
