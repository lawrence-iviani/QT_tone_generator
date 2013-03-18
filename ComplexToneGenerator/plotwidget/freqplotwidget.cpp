#include "freqplotwidget.h"
#include "freqplotwigetparams.h"
#include "freqplotwidgetui.h"

SpectrogramData::SpectrogramData(double SR,double minTime,double maxTime) :
        m_numberOfBins(4096),
        m_sizeArray(0),
        m_array(NULL),
        m_stepFreq(1),
        m_stepTime(1)
    { setIntervals(SR,minTime,maxTime); }

SpectrogramData::~SpectrogramData() {  if (m_array != NULL) delete[] m_array; }

void SpectrogramData::setIntervals(double SR,double minTime,double maxTime) {
    setSampleRate(SR);
    setTimeLength(minTime,maxTime);
    m_intervalMagnitude=QwtInterval( FREQPLOTWIDGET_MIN_MAGNITUDE, FREQPLOTWIDGET_MAX_MAGNITUDE );
    setInterval( Qt::ZAxis, m_intervalMagnitude);
}

void SpectrogramData::setSampleRate(double SR) {
    m_intervalFreq=QwtInterval( 10, SR /2.0,QwtInterval::ExcludeBorders );
    setInterval( Qt::XAxis, m_intervalFreq );

}

void SpectrogramData::setTimeLength(double minTime, double maxTime) {
    m_intervalTime=QwtInterval( minTime, maxTime, QwtInterval::ExcludeMaximum);
    setInterval( Qt::YAxis, m_intervalTime);
}

void SpectrogramData::setData(const double * array, uint arraySize, uint binsPerWindow, QString windowType, qreal percentOverlap)
{
    Q_UNUSED(percentOverlap); //TODO: impement overlap

    m_numberOfBins=binsPerWindow;
    //Setting the data matrix creating a new one, deleting the previous one if  it exists.
    uint _numberOfSlices=qCeil( ((qreal)arraySize) / ((qreal)m_numberOfBins));

    //Calculate the internal array, it will can be a little bit larger than

    //TODO this is based also on the overlap, now is equal to numberOfSlice
    //In the future overlap must be calculated here!
    uint _numberOfWindows=_numberOfSlices;
    m_sizeArray= _numberOfWindows * m_numberOfBins;
    Q_ASSERT(arraySize<=m_sizeArray);
    if (m_array != NULL)
        delete[] m_array;
    m_array = new double [m_sizeArray];

    //Fulfilling  of zero
    memset(m_array,0,m_sizeArray * sizeof(double));
    //Setting STFT params, missing another sampling (useful??), window type and window overlap

    m_stepFreq=(m_intervalFreq.maxValue() - m_intervalFreq.minValue())/m_numberOfBins;
    m_stepTime=(m_intervalTime.maxValue() - m_intervalTime.minValue())/_numberOfSlices;

    //Calculate & Setting the STFT
    fftw_complex * in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*m_numberOfBins);
    //double* in=(double*)malloc(m_numberOfBins*sizeof(double));
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*m_numberOfBins);
    fftw_plan my_plan;
    //my_plan = fftw_plan_dft_r2c_1d(nfft, in, out, FFTW_ESTIMATE);//FFTW_FORWARD, FFTW_ESTIMATE);
    my_plan = fftw_plan_dft_1d(m_numberOfBins, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    uint i;

    //Create window
    double _arrayWindow[m_numberOfBins];
    TransformWindow::generateWindow(_arrayWindow,m_numberOfBins,windowType);

    qDebug() << Q_FUNC_INFO << "Starting calculation with  _numberOfWindows="<<_numberOfWindows
             <<"and m_numberOfBins="<< m_numberOfBins << " each window. _numberOfSlices="<< _numberOfSlices;

    for (uint nWin=0; nWin < _numberOfWindows; nWin++) {
        uint _lowIndex=nWin*m_numberOfBins;//(nWin==0 ? 0:  nWin*m_numberOfBins+1);
        uint _hiIndex=nWin*m_numberOfBins+m_numberOfBins-1;
        //setting input signal for window nWin
        i=_lowIndex;//IF OVERLAP????
        for (uint nBin=0;nBin<m_numberOfBins;nBin++) {
            //in[nBin]=array[i++];
           in[nBin][0]= _arrayWindow[nBin]*((i < arraySize) ? array[i++]  : 0.0);//Setting real part of the input signal
           in[nBin][1]=0.0;//Setting imag part of the input signal
        }
        Q_ASSERT (i-1 < arraySize);
        //qDebug() << Q_FUNC_INFO << "Signal ready i="<<i-1<<"array lowInedx="<< _lowIndex << "hiIndex="<< _hiIndex;

        //calculate FFT for nWin
        fftw_execute(my_plan);

        //Calculate power spectrum for window nWin
        i=_lowIndex;//IF OVERLAP????
        m_array[i]=POWER_SPECTRUM(out[0][0],out[0][1],m_numberOfBins);//DC component
        for (uint nBin=0;nBin<(m_numberOfBins+1)/2;nBin++) {
            //val, the power spectrum
            double val=POWER_SPECTRUM(out[nBin][0],out[nBin][1],m_numberOfBins);
            m_array[i++]=val;
            m_array[i++]=val;
        // printf("out[%d][0]=%f, out[%d][1]=%f m_array[%d]=%f\n",nBin,out[nBin][0], nBin,out[nBin][1],i,m_array[i-1]);
        }
        //qDebug() << Q_FUNC_INFO << "Power specturm i="<<i-1<<"array lowInedx="<< _lowIndex << "hiIndex="<< _hiIndex;
        Q_ASSERT(i-1==_hiIndex);
    }

    qDebug() << Q_FUNC_INFO << "END last index i="<<i<<" shoudl be equal to m_sizeArray="<< m_sizeArray;
    Q_ASSERT(i==m_sizeArray);
    fftw_destroy_plan(my_plan);
    free(in);
    fftw_free(out);

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"setting data with _numberOfWindows=%1 m_numberOfBins=%2, for a total of bins m_sizeArray=%3")
                      .arg(_numberOfWindows)
                      .arg(m_numberOfBins)
                      .arg(m_sizeArray));
}

double SpectrogramData::value(double x, double y) const
{
    //X frequency
    //Y Time
   if (!m_array) return m_intervalMagnitude.minValue();

   //calculating matrix indexes from x (frequency bins) and y (time slice)
   uint nBins = qRound(x/m_stepFreq);
   uint nSlices = qFloor(y/m_stepTime);

//   //getting the absolute index neareset
   uint index=nSlices*m_numberOfBins + nBins;// (m_intervalFreq.maxValue() - m_intervalFreq.minValue()) * freqPos;

   //Q_ASSERT(index<m_sizeArray);
   double retval;
   if (index>=m_sizeArray) {
       PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"request index=%1 out of range m_sizeArray=%2 (x,y)=(%3,%4) and (indFreq,indTime)=(%5,%6), returning minval")
                   .arg(index)
                   .arg(m_sizeArray)
                   .arg(x)
                   .arg(y)
                   .arg(nBins)
                   .arg(nSlices));
       retval=m_intervalMagnitude.minValue();
   } else {
       retval=(double)m_array[index];
//       PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"Index OK, retval=%1 map (x=%2,y=%3) => (bin=%4,slice=%5)")
//                         .arg(retval)
//                         .arg(x)
//                         .arg(y)
//                         .arg(nBins)
//                         .arg(nSlices));
   }
   return retval;
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
    setXScaleType(PlotWidget::Logarithmic);

    //Setscale limits, should check if these value changed...
    setBothAxisScale(10 , TIMEDATA_DEFAULT_SR/2 , TIMEDATA_DEFAULT_PROJECT_TIME, TIMEDATA_DEFAULT_MIN_TIME);
    plotLayout()->setAlignCanvasToScales(true);

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
}

void FreqPlotWidget::dataUpdated() {
    if (!m_timeData) return;
    SpectrogramData* _spectrData=dynamic_cast<SpectrogramData*>(m_spectrogram.data());
    Q_ASSERT(_spectrData);
    GenericTimeDataParams *_gtdParams=dynamic_cast< GenericTimeDataParams*> (m_timeData->getDataParameters());
    Q_ASSERT(_gtdParams!=NULL);

    //Setscale limits, should check if these value changed...
    setBothAxisScale(10 , _gtdParams->sampleRate()/2.0 , _gtdParams->maxDuration(), _gtdParams->startTime());
    _spectrData->setSampleRate(_gtdParams->sampleRate());
    _spectrData->setTimeLength(_gtdParams->startTime(),_gtdParams->maxDuration());

    FreqPlotWigetParams* _freqParams=dynamic_cast<FreqPlotWigetParams*> (m_freqPlotDelegate->getProperty());
    Q_ASSERT(_freqParams);

    qDebug()<<Q_FUNC_INFO << "set data SR=" << _gtdParams->sampleRate() << " timemin="<<_gtdParams->startTime() << " timemax"<<_gtdParams->maxDuration();
    uint _expcetdSampleNumber=qCeil(_gtdParams->sampleRate()*(_gtdParams->maxDuration()-_gtdParams->startTime()));
    qDebug()<<Q_FUNC_INFO << "digest m_timeData->getSampleNumber()="<< m_timeData->getSampleNumber() << " expected="<< _expcetdSampleNumber;
    qDebug() << Q_FUNC_INFO << "Calling setdata with m_timeData->getSampleNumber()="<< m_timeData->getSampleNumber()<<
                "_freqParams->binsNumber()="<< _freqParams->binsNumber() << " _freqParams->windowType() is "<< _freqParams->windowType();
    //Params:
    //m_timeData->getSignalData() pointer to data
    //m_timeData->getSampleNumber() the number of sample
    //m_binsPerWindow the numnber of bins of any window this is coincident with the fft number
    //m_windowName, MISSING the window that has to be used (default, hamming???)
    //m_percentOverlap, MISSING the percent of overlap between window..
    _spectrData->setData(m_timeData->getSignalData(),m_timeData->getSampleNumber(),_freqParams->binsNumber(),_freqParams->windowType());
    //_spectrData->setData()
    this->replot();
}

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
    qDebug() << Q_FUNC_INFO << " array[0]=" <<array[0] << " array["<< (nBins-1) << "]= " << array[nBins-1];
}
