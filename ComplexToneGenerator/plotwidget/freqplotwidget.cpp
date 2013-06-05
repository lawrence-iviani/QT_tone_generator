#include "freqplotwidget.h"
#include "freqplotwigetparams.h"
#include "freqplotwidgetui.h"

//New spectrogram
SpectrogramData::SpectrogramData(double SR,double minTime,double maxTime)
{
    //Init a matrix
    setIntervals(SR,minTime,maxTime);
    uint nwin=qRound((maxTime-minTime)*SR/4096);
    m_fftArray.fill(FREQPLOTWIDGET_MIN_MAGNITUDE,nwin*4096);
    setValueMatrix(m_fftArray, nwin);
    setResampleMode(QwtMatrixRasterData::BilinearInterpolation);
}

SpectrogramData::~SpectrogramData() { /* if (m_fftArray != NULL) delete m_fftArray;*/ }

void SpectrogramData::setIntervals(double SR,double minTime,double maxTime) {
    setSampleRate(SR);
    setTimeLength(minTime,maxTime);
    m_intervalMagnitude=QwtInterval( FREQPLOTWIDGET_MIN_MAGNITUDE, FREQPLOTWIDGET_MAX_MAGNITUDE );
    setInterval( Qt::ZAxis, m_intervalMagnitude);
}

void SpectrogramData::setSampleRate(double SR) {
    m_intervalFreq=QwtInterval( 10, SR /2.0,QwtInterval::ExcludeBorders );
    setInterval( Qt::YAxis, m_intervalFreq );
}

void SpectrogramData::setTimeLength(double minTime, double maxTime) {
    m_intervalTime=QwtInterval( minTime, maxTime, QwtInterval::ExcludeMaximum);
    setInterval( Qt::XAxis, m_intervalTime);
}

void SpectrogramData::setData(const double * array, uint arraySize, uint binsPerWindow, QString windowType, qreal percentOverlap)
{
    uint _numberOfBins=binsPerWindow;//Number of rows
    qreal _percentileOverlap=percentOverlap/100.0;
    Q_ASSERT(percentOverlap>=0.0 && percentOverlap <100.0);

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,
                    "Calculate number of window for an array of %1 samples, with an overlap of %2 % and window of %3")
                      .arg(arraySize)
                      .arg(percentOverlap)
                      .arg(windowType));

    //------------- VERY BAD CODE -------------
    //Calculate the number of way in iterative way, this should be in a closed form.
    uint pos=0;
    uint _numberOfWindows=0;
    while (pos < arraySize) {
        uint _lowIndex=qFloor((1.0-_percentileOverlap)*_numberOfWindows*_numberOfBins);
        uint _hiIndex=qMin( _lowIndex+_numberOfBins, arraySize);
        //qDebug() << "Calculating win length ("<<_numberOfWindows<<")"<< _lowIndex << "-" << _hiIndex<<"/"<< arraySize;
        _numberOfWindows++;
        pos=_hiIndex;
    }
    //------------- VERY BAD CODE END-------------


    //Calculate the internal array, it will can be a little bit larger than
    uint _sizeArray=_numberOfWindows+(_numberOfWindows) * _numberOfBins/2;
    m_fftArray.resize(_sizeArray);
   // m_fftArray.fill(FREQPLOTWIDGET_MIN_MAGNITUDE+_percentileOverlap*10, _sizeArray);//DEBUG
   // m_fftArray.fill(FREQPLOTWIDGET_MIN_MAGNITUDE, _sizeArray);

    //Calculate & Setting the STFT
    fftw_complex * in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*_numberOfBins);
    //double* in=(double*)malloc(m_numberOfBins*sizeof(double));
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*_numberOfBins);
    fftw_plan my_plan;
    //my_plan = fftw_plan_dft_r2c_1d(nfft, in, out, FFTW_ESTIMATE);//FFTW_FORWARD, FFTW_ESTIMATE);
    my_plan = fftw_plan_dft_1d(_numberOfBins, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    //Create window
    double _arrayWindow[_numberOfBins];
    TransformWindow::generateWindow(_arrayWindow,_numberOfBins,windowType);

//    qDebug() << Q_FUNC_INFO << "Starting calculation with an array of "<< arraySize << "  _numberOfWindows="<<_numberOfWindows
//             <<"and m_numberOfBins="<< _numberOfBins << " each window. _numberOfSlices="<< _numberOfSlices;

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,
                      "Starting calculation of an array of %1 samples, with an overlap of %2 (%3 total samples) with  %4 windows, each has %5 bins, the total size of the internal array is %6")
                      .arg(arraySize)
                      .arg(percentOverlap)
                      .arg(_sizeArray)
                      .arg(_numberOfWindows)
                      .arg(_numberOfBins)
                      .arg(_sizeArray));
    uint i;
    for (uint nWin=0; nWin < _numberOfWindows; nWin++) {
        uint _lowIndex=nWin*_numberOfBins-qRound(nWin*_numberOfBins*_percentileOverlap);
        uint _hiIndex=_lowIndex+_numberOfBins-1;
        //setting input signal for window nWin
        i=_lowIndex;//IF OVERLAP????
       //  {
  //      qDebug() << "Mapping signal win("<<nWin<<")"<< _lowIndex << "-" << _hiIndex<<"/"<< arraySize;
        for (uint nBin=0;nBin<_numberOfBins;nBin++) {
           in[nBin][0]= _arrayWindow[nBin]*((i < arraySize) ? array[i++] : 0.0);//Setting real part of the input signal
           in[nBin][1]=0.0;//Setting imag part of the input signal
        }
        //Test to verify if the input array was extracted correctly
        if (nWin==(_numberOfWindows-1)) Q_ASSERT(i==arraySize);
        else Q_ASSERT(_hiIndex==i-1);

        //calculate FFT for nWin
        fftw_execute(my_plan);

        uint nBin;
        for (nBin=0;nBin<(_numberOfBins+1)/2;nBin++) {
            m_fftArray[nWin+_numberOfWindows*nBin]=POWER_SPECTRUM(out[nBin][0],out[nBin][1],_numberOfBins);
        }
//        qDebug() << "\tInto FFT array win("<<nWin<<") from "
//                 << nWin << "-"
//                 << (nWin+_numberOfWindows*nBin)
//                 <<"/"<< _sizeArray;
        if (nWin==(_numberOfWindows-1)) Q_ASSERT(nWin+_numberOfWindows*nBin==_sizeArray-1);
    }
    fftw_destroy_plan(my_plan);
    fftw_free(in);
    fftw_free(out);

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"Set data with %1 windows, each has %2 bins, the total size of the internal array is %3")
                      .arg(_numberOfWindows)
                      .arg(_numberOfBins)
                      .arg(_sizeArray));

    setValueMatrix(m_fftArray, _numberOfWindows);
}

FreqPlotWidget::FreqPlotWidget(QWidget *parent) :
    PlotWidget(parent),
    m_timeData(NULL)
{
    m_freqPlotDelegate=new DataUiHandlerDelegate(
                dynamic_cast<DataUiHandlerProperty*>(new FreqPlotWigetParams((QObject*)parent)),
                dynamic_cast<DataUiHandlerUI*>(new FreqPlotWidgetUI()),//(new ZMP_Handler(this->canvas()))),
                (QObject*)parent);

    //Spectrogram
    m_spectrogram.setRenderThreadCount(0); // use system specific thread count
    m_spectrogram.setColorMap(new ColorMap());
    m_spectrogram.setData(new SpectrogramData());
    m_spectrogram.attach(this);

    // Add a color bar on the right axis
    QwtScaleWidget *_rightAxis = axisWidget(QwtPlot::yRight);
    QwtInterval _zInterval(FREQPLOTWIDGET_MIN_MAGNITUDE,FREQPLOTWIDGET_MAX_MAGNITUDE);
    this->setAxisScale(QwtPlot::yRight, _zInterval.minValue(), _zInterval.maxValue() );
    _rightAxis->setColorMap( _zInterval, new ColorMap());
    this->setAxisScale(QwtPlot::yRight, _zInterval.minValue() ,_zInterval.maxValue());
    this->enableAxis(QwtPlot::yRight);
    _rightAxis->setTitle("Magnitude (dB)");
    _rightAxis->setColorBarEnabled(true);

    //set Xscale
    setYScaleType(PlotWidget::Logarithmic);

    //Setscale limits, should check if these value changed...
    setBothAxesScale( TIMEDATA_DEFAULT_MIN_TIME,TIMEDATA_DEFAULT_PROJECT_TIME,10 , TIMEDATA_DEFAULT_SR/2 );

    FreqPlotWidgetUI *_ui=dynamic_cast< FreqPlotWidgetUI*> (getControlWidget());
    Q_ASSERT(_ui!=NULL);
    _ui->addZMPControlWidget(m_zmp->getControlWidget());

    //set title
    this->setPlotTitle("Digest curve spectrogram");
    connectSignals();
}

void FreqPlotWidget::connectSignals() {
    FreqPlotWigetParams* _freqParams=dynamic_cast<FreqPlotWigetParams*> (m_freqPlotDelegate->getProperty());
    Q_ASSERT(_freqParams);

    //Keep consitent duration and sample rate
    Q_ASSERT(connect(_freqParams,SIGNAL(binsNumberChanged(uint)),this,SLOT(dataUpdated())));
    Q_ASSERT(connect(_freqParams,SIGNAL(windowTypeChanged(QString)),this,SLOT(dataUpdated())));
    Q_ASSERT(connect(_freqParams,SIGNAL(overlapChanged(qreal)),this,SLOT(dataUpdated())));
    Q_ASSERT(connect(_freqParams,SIGNAL(logFreqScaleChanged(bool)),this,SLOT( enableLogFreqScale(bool))));

}

void FreqPlotWidget::dataUpdated() {
    if (!m_timeData) return;
    SpectrogramData* _spectrData=dynamic_cast<SpectrogramData*>(m_spectrogram.data());
    Q_ASSERT(_spectrData);
    GenericTimeDataParams *_gtdParams=dynamic_cast< GenericTimeDataParams*> (m_timeData->getDataParameters());
    Q_ASSERT(_gtdParams!=NULL);

    //Setscale limits, should check if these value changed...
    setBothAxesScale( _gtdParams->startTime(), _gtdParams->maxDuration() ,10 , _gtdParams->sampleRate()/2.0);
    _spectrData->setSampleRate(_gtdParams->sampleRate());
    _spectrData->setTimeLength(_gtdParams->startTime(),_gtdParams->maxDuration());

    FreqPlotWigetParams* _freqParams=dynamic_cast<FreqPlotWigetParams*> (m_freqPlotDelegate->getProperty());
    Q_ASSERT(_freqParams);

//    qDebug()<<Q_FUNC_INFO << "set data SR=" << _gtdParams->sampleRate() << " timemin="<<_gtdParams->startTime() << " timemax"<<_gtdParams->maxDuration();
    uint _expcetdSampleNumber=qCeil(_gtdParams->sampleRate()*(_gtdParams->maxDuration()-_gtdParams->startTime()));
//    qDebug()<<Q_FUNC_INFO << "digest m_timeData->getSampleNumber()="<< m_timeData->getSampleNumber() << " expected="<< _expcetdSampleNumber;
//    qDebug() << Q_FUNC_INFO << "Calling setdata with m_timeData->getSampleNumber()="<< m_timeData->getSampleNumber()<<
                //"_freqParams->binsNumber()="<< _freqParams->binsNumber() << " _freqParams->windowType() is "<< _freqParams->windowType();
    //Params:
    //m_timeData->getSignalData() pointer to data
    //m_timeData->getSampleNumber() the number of sample
    //m_binsPerWindow the numnber of bins of any window this is coincident with the fft number
    //m_windowName,  the window that has to be used (default, hamming???)
    //m_percentOverlap,  the percent of overlap between window..
    _spectrData->setData(m_timeData->getSignalData(),m_timeData->getSampleNumber(),_freqParams->binsNumber(),_freqParams->windowType(),_freqParams->overlap());
    //_spectrData->setData()
    this->replot();
}

void FreqPlotWidget::setRubberBandPosition(qreal position) {
    m_scrollRubberBand->setValue(position);
}


//----------------- TRANSFORM WINDOW -----------------
void TransformWindow::generateWindow(double array[], uint nBins, const QString& windowType) {
    if (windowType=="Hann") return TransformWindow::hannWindow(array,nBins);
    if (windowType=="Hamming") return TransformWindow::hammingWindow(array,nBins);
    if (windowType=="Bartlett") return TransformWindow::bartlettWindow(array,nBins);
    if (windowType=="Rectangular") return TransformWindow::rectangularWindow(array,nBins);

    PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,QString("Unknown window type --%1--, returning Hamming window").arg(windowType)));
    return TransformWindow::hammingWindow(array,nBins);
}

//POLYNOMIAL WINDOW
void TransformWindow::rectangularWindow(double array[], uint nBins){
    if (nBins==0)return;
    for (uint i = 0; i < nBins; i++) {
        array[i]= 1;
    }
}

void TransformWindow::bartlettWindow(double array[], uint nBins) {
    if (nBins==0)return;
    for (uint i = 0; i < nBins; i++) {
        double _t=(nBins-1.0)/2.0;
        array[i]=1 - qAbs((i-_t)/_t);
    }
    qDebug() << Q_FUNC_INFO << " array[0]=" <<array[0] << " array["<< (nBins-1) << "]= " << array[nBins-1];
    Q_ASSERT(array[0]==0);
    Q_ASSERT(array[nBins-1]==0);
}

//HAMMING WINDOW
void TransformWindow::hammingWindow(double array[], uint nBins) {
    TransformWindow::generalizeHammingWindow(array, nBins, 0.54, 0.46);
}

void TransformWindow::hannWindow(double array[], uint nBins) {
    TransformWindow::generalizeHammingWindow(array, nBins, 0.5, 0.5);
    Q_ASSERT(array[0]==0);
    Q_ASSERT(array[nBins-1]==0);
}

void TransformWindow::generalizeHammingWindow(double array[], uint nBins, double alpha, double beta) {
    if (nBins==0)return;
    register double _factor=2*M_PI/(nBins-1);
    for (uint i = 0; i < nBins; i++) {
        array[i]= alpha-beta *  cos(_factor*i);
    }
}
