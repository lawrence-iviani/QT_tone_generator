#include "freqplotwidget.h"


SpectrogramData::SpectrogramData(GenericTimeData* timeData) :
        m_timeData(timeData),
        m_numberOfBins(0),
        m_numberOfSlices(0),
        m_sizeArray(0),
        m_array(NULL),
        m_stepFreq(1),
        m_stepTime(1)
    { setIntervals(); }
SpectrogramData::~SpectrogramData() {  if (m_array != NULL) delete[] m_array; }

void SpectrogramData::setIntervals() {
    if (m_timeData) {
        GenericTimeDataParams *_gtdParams=dynamic_cast< GenericTimeDataParams*> (m_timeData->getDataParameters());
        Q_ASSERT(_gtdParams!=NULL);
        m_intervalFreq=QwtInterval( 10, _gtdParams->sampleRate() /2.0 );
        m_intervalTime=QwtInterval( _gtdParams->startTime(), _gtdParams->maxDuration() );
    } else {
        m_intervalFreq=QwtInterval( 10, TIMEDATA_DEFAULT_SR/2.0  );
        m_intervalTime=QwtInterval(TIMEDATA_DEFAULT_MIN_TIME, TIMEDATA_DEFAULT_PROJECT_TIME );
    }
    m_intervalMagnitude=QwtInterval( FREQPLOTWIDGET_MIN_MAGNITUDE, FREQPLOTWIDGET_MAX_MAGNITUDE );
    setInterval( Qt::XAxis, m_intervalFreq );
    setInterval( Qt::YAxis, m_intervalTime);
    setInterval( Qt::ZAxis, m_intervalMagnitude);

 //   m_stepFreq=(m_intervalFreq.maxValue()-m_intervalFreq.minValue())/2048.0;
 //   m_stepTime=0.001;
 //   m_stepMagnitude=0.01;
}

//double SpectrogramData::value(double x, double y) const
//{
//    if (!m_timeData)
//        return m_minMagnitude;
//    else
//        return  -80.0*((qreal)qrand())/((qreal)RAND_MAX);
//}

void SpectrogramData::setData(const qreal * array, uint arraySize, uint numberOfSlice, uint samplesPerSlice)
{
    //Setting the data matrix creating a new one, deleting the previous one if  it exists.
    numberOfSlice++;
    m_sizeArray= numberOfSlice * samplesPerSlice;
    Q_ASSERT(arraySize<=m_sizeArray);
    if (m_array != NULL)
        delete[] m_array;
    m_array = new double [m_sizeArray];
    //Fulfilling  of zero
    memset(m_array,0,m_sizeArray * sizeof(double));

    //Setting STFT params, missing another sampling (useful??), window type and window overlap
    uint numberOfWindows=numberOfSlice;//TODO this is based also on the overlap, now is equal to numberOfSlice
    m_numberOfBins=samplesPerSlice;
    m_numberOfSlices=numberOfSlice;
    m_stepFreq=(m_intervalFreq.maxValue() - m_intervalFreq.minValue())/m_numberOfBins;
    m_stepTime=(m_intervalTime.maxValue() - m_intervalTime.minValue())/m_numberOfSlices;

    //Calculate & Setting the STFT
    fftw_complex * in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*m_numberOfBins);
    //double* in=(double*)malloc(m_numberOfBins*sizeof(double));
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*m_numberOfBins);
    fftw_plan my_plan;
    //my_plan = fftw_plan_dft_r2c_1d(nfft, in, out, FFTW_ESTIMATE);//FFTW_FORWARD, FFTW_ESTIMATE);
    my_plan = fftw_plan_dft_1d(m_numberOfBins, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    for (uint nWin=0; nWin < numberOfWindows; nWin++) {
        int i=nWin*samplesPerSlice;
        for (uint nBin=0;nBin<m_numberOfBins;nBin++) {
            //in[nBin]=array[i++];
           in[nBin][0]=array[i++];
           in[nBin][1]=0.0;
        }
        fftw_execute(my_plan);
        i=nWin*samplesPerSlice;
        for (uint nBin=0;nBin<m_numberOfBins/2;nBin++) {
            double val=10*log10((out[nBin][0]*out[nBin][0]+out[nBin][1]*out[nBin][1])/(m_numberOfBins));
            m_array[i++]=val;
            m_array[i++]=val;
           // printf("out[%d][0]=%f, out[%d][1]=%f m_array[%d]=%f   ",nBin,out[nBin][0], nBin,out[nBin][1],i,m_array[i]);
        }
    }
    fftw_destroy_plan(my_plan);
    free(in);
    fftw_free(out);

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"setting data with numberOfSlice=%1 numberOfBins=%2, for a total of bins m_sizeArray=%3")
                      .arg(numberOfSlice)
                      .arg(samplesPerSlice)
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
//   uint index =timePos +  (m_intervalFreq.maxValue() - m_intervalFreq.minValue()) * freqPos;
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
    m_timeData(NULL),
    m_binsPerWindow(FREQPLOTWIDGET_SAMPLES_PER_WINDOW)
{
    m_spectrogram.setRenderThreadCount(0); // use system specific thread count
    m_spectrogram.setColorMap(new ColorMap());
    m_spectrogram.setData(new SpectrogramData(m_timeData));
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

    this->createControlWidget();
}

void FreqPlotWidget::dataUpdated() {
    if (!m_timeData) return;
    SpectrogramData* _spectrData=dynamic_cast<SpectrogramData*>(m_spectrogram.data());
    Q_ASSERT(_spectrData);
    GenericTimeDataParams *_gtdParams=dynamic_cast< GenericTimeDataParams*> (m_timeData->getDataParameters());
    Q_ASSERT(_gtdParams!=NULL);

    //Setscale limits, should check if these value changed...
    setBothAxisScale(10 , _gtdParams->sampleRate()/2.0 , _gtdParams->maxDuration(), _gtdParams->startTime());

    qDebug() << Q_FUNC_INFO << "binsPerWindow=" <<m_binsPerWindow;
    uint nWindow=qCeil( ((qreal)m_timeData->getSampleNumber()) / ((qreal)m_binsPerWindow));
    _spectrData->setData(m_timeData->getSignalData(),m_timeData->getSampleNumber(),nWindow,m_binsPerWindow);

    this->replot();
}

void FreqPlotWidget::createControlWidget() {
    m_allControl=new QFrame();
    m_allControl->setFrameShape(QFrame::WinPanel);
    m_allControl->setFrameShadow(QFrame::Raised);
    m_allControl->setLayout(new QVBoxLayout());
    QVBoxLayout * l=(QVBoxLayout*)m_allControl->layout();

    //Adding Option button control, this remains always visible
    m_baseControl.toggleButtonOptionEnable=new QPushButton("Freq. plot option");
    m_baseControl.toggleButtonOptionEnable->setCheckable(true);
    m_baseControl.toggleButtonOptionEnable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    l->addWidget(m_baseControl.toggleButtonOptionEnable ,1,Qt::AlignHCenter|Qt::AlignTop);
    connect(m_baseControl.toggleButtonOptionEnable,SIGNAL(clicked()),this,SLOT(freqOptionPressed()) );

    this->initBaseControlWidget();
    l->addWidget(m_baseControl.baseControlWidget,1,Qt::AlignHCenter|Qt::AlignTop);
}

void FreqPlotWidget::initBaseControlWidget() {
    //setting font base dimension
    QFont f=*(new QFont());
    f.setPointSize(PLOTWIDGET_DEFAULT_PLOT_DIMENSION);

    //Widget container and layout for the real control option
    m_baseControl.baseControlWidget=new QFrame();
    m_baseControl.baseControlWidget->setFrameShape(QFrame::WinPanel);
    m_baseControl.baseControlWidget->setFrameShadow(QFrame::Raised);
    QVBoxLayout * lBase=new QVBoxLayout();
    m_baseControl.baseControlWidget->setLayout(lBase) ;
    m_baseControl.baseControlWidget->hide();
    m_baseControl.baseControlWidget->setFont(f);
}

void FreqPlotWidget::freqOptionPressed() {
    Q_ASSERT(m_baseControl.baseControlWidget!=NULL);
    if (m_baseControl.toggleButtonOptionEnable->isChecked()) {
        m_baseControl.baseControlWidget->show();
    } else {
        m_baseControl.baseControlWidget->hide();
    }
}
