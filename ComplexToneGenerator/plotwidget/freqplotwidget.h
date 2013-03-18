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

//Available bins added in a stringlist
#define FREQPLOTWIDGET_BINS(stringList)  stringList << "128" << "256" << "512" << "1024" << "2048" << "4096" << "8192" << "16384" << "32768" << "65536"
#define FREQPLOTWIDGET_OVERLAP(stringList)  stringList <<  "0 %" << "25 %" << "50 %" << "75 %"
#define FREQPLOTWIDGET_WINDOWTYPE(stringList)  stringList << "Rectangular" << "Bartlett" << "Hann" << "Hamming"

//rectangle 0.015625
#define POWER_SPECTRUM(real,imag,nbins) 10*log10(1.0*(real*real+imag*imag)/(nbins));

class SpectrogramData: public QwtRasterData
{

public:
    SpectrogramData(double SR=TIMEDATA_DEFAULT_SR,double minTime=TIMEDATA_DEFAULT_MIN_TIME,double maxTime=TIMEDATA_DEFAULT_PROJECT_TIME);

    virtual ~SpectrogramData();

    void setData(const double * array, uint arraySize, uint binsPerWindow, QString windowType,qreal percentOverlap=1.0);
    virtual double value(double x, double y) const;
    void setSampleRate(double SR);
    void setTimeLength(double minTime,double maxTime);

    static void generalizeHammingWindow(double array[], uint nBins, double alpha, double beta);
    static void hammingWindow(double array[], uint nBins) {return SpectrogramData::generalizeHammingWindow(array, nBins, 0.54, 0.46);}
    static void hannWindow(double array[], uint nBins) {return SpectrogramData::generalizeHammingWindow(array, nBins, 0.5, 0.5);}

private:
    void setIntervals(double SR,double minTime,double maxTime);
    QwtInterval m_intervalFreq;
    QwtInterval m_intervalTime;
    QwtInterval m_intervalMagnitude;
    uint m_numberOfBins;
    uint m_sizeArray;
    double* m_array;
    double m_stepFreq;
    double m_stepTime;
};

class TransformWindow {

public:
    static void generateWindow(double array[], uint nBins, const QString& windowType="Hamming");
    static void hammingWindow(double array[], uint nBins);
    static void hannWindow(double array[], uint nBins);
    static void bartlettWindow(double array[], uint nBins);
    static void rectangularWindow(double array[], uint nBins);

private:
    static void generalizeHammingWindow(double array[], uint nBins, double alpha, double beta);
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
    void setTimeData(GenericTimeData* timeData) {
        m_timeData=timeData;
        m_spectrogram.setData(new SpectrogramData());
    }

    //virtual void setRubberBandPosition(qreal position);
    inline QWidget * getControlWidget() {return dynamic_cast<QWidget*>(m_freqPlotDelegate->getUI()); }// Give back a QWidget that can be used to control this class.
    inline DataUiHandlerProperty* getDataParameters() {return m_freqPlotDelegate->getProperty();}
    inline QDomDocument exportXML() {return getDelegate()->getDomDocument();}

protected:
    inline DataUiHandlerDelegate* getDelegate() {return dynamic_cast<DataUiHandlerDelegate*>(m_freqPlotDelegate);}
    
public slots:
    void dataUpdated();
private:
    void connectSignals();

    struct {
      QFrame * baseControlWidget;
      QPushButton * toggleButtonOptionEnable;
    } m_baseControl;
    QwtPlotSpectrogram m_spectrogram;
    GenericTimeData *m_timeData;

    DataUiHandlerDelegate * m_freqPlotDelegate;
};





#endif // FREQPLOTWIDGET_H
