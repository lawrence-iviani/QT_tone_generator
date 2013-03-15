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

void SpectrogramData::setData(const qreal * array, uint arraySize, uint numberOfSlice, uint numberOfBins)
{
//        m_DataSize.x = numberOfSlice;
//        m_DataSize.y = numberOfBins;
   numberOfSlice++;
   m_sizeArray= (numberOfSlice) * numberOfBins;
   Q_ASSERT(arraySize<=m_sizeArray);
  //  MinMaxArrayValue(Array, size, &m_minValue, &m_maxValue);
    if (m_array != NULL)
        delete[] m_array;
    m_array = new double [m_sizeArray];
    memset(m_array,0,m_sizeArray * sizeof(double));
    memcpy(m_array, array, arraySize * sizeof(double));

    m_numberOfBins=numberOfBins;
    m_numberOfSlices=numberOfSlice;
    m_stepFreq=(m_intervalFreq.maxValue() - m_intervalFreq.minValue())/m_numberOfBins;
    m_stepTime=(m_intervalTime.maxValue() - m_intervalTime.minValue())/m_numberOfSlices;

    PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"setting data with numberOfSlice=%1 numberOfBins=%2, for a total of bins m_sizeArray=%3")
                      .arg(numberOfSlice)
                      .arg(numberOfBins)
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

   if (index>=m_sizeArray) {
       PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"request index=%1 out of range m_sizeArray=%2\n\t (x,y)=(%3,%4) and (indFreq,indTime)=(%5,%6), returning minval")
                   .arg(index)
                   .arg(m_sizeArray)
                   .arg(x)
                   .arg(y)
                   .arg(nBins)
                   .arg(nSlices));
       return m_intervalMagnitude.minValue();
   } else {
//       PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"Index OK, map (x=%1,y=%2) => (bins=%3,slice=%4)")
//                         .arg(x)
//                         .arg(y)
//                         .arg(freqPos)
//                         .arg(timePos));
       double retval=(double)10*log10(m_array[index]*m_array[index]);
//       PRINT_DEBUG_LEVEL(ErrorMessage::DEBUG_NOT_SO_IMPORTANT,ErrorMessage::DEBUG(Q_FUNC_INFO,"Index OK, retval=%1 map (x=%2,y=%3) => (bin=%4,slice=%5)")
//                         .arg(retval)
//                         .arg(x)
//                         .arg(y)
//                         .arg(nBins)
//                         .arg(nSlices));
       return retval;
   }
   //Q_ASSERT(index<m_sizeArray);
}

FreqPlotWidget::FreqPlotWidget(QWidget *parent) :
    PlotWidget(parent),
    m_timeData(NULL)
{
    m_spectrogram.setRenderThreadCount(0); // use system specific thread count
    m_spectrogram.setColorMap(new ColorMap());
    m_spectrogram.setData(new SpectrogramData(m_timeData));
    m_spectrogram.attach(this);
    // A color bar on the right axis
    QwtScaleWidget *_rightAxis = axisWidget(QwtPlot::yRight);

    QwtInterval _zInterval(FREQPLOTWIDGET_MIN_MAGNITUDE,FREQPLOTWIDGET_MAX_MAGNITUDE);
    this->setAxisScale(QwtPlot::yRight, _zInterval.minValue(), _zInterval.maxValue() );
    _rightAxis->setColorMap( _zInterval, new ColorMap());
    this->setAxisScale(QwtPlot::yRight,FREQPLOTWIDGET_MIN_MAGNITUDE,FREQPLOTWIDGET_MAX_MAGNITUDE);
    this->enableAxis(QwtPlot::yRight);
    _rightAxis->setTitle("Magnitude (dB)");
    _rightAxis->setColorBarEnabled(true);

    plotLayout()->setAlignCanvasToScales(true);

    this->createControlWidget();
}

void FreqPlotWidget::dataUpdated() {
    if (!m_timeData) return;
    SpectrogramData* _spectrData=dynamic_cast<SpectrogramData*>(m_spectrogram.data());
    Q_ASSERT(_spectrData);
    GenericTimeDataParams *_gtdParams=dynamic_cast< GenericTimeDataParams*> (m_timeData->getDataParameters());
    Q_ASSERT(_gtdParams!=NULL);
//        std::vector<double> _auxVector;
//        _auxVector.assign(m_timeData->getSignalData(), m_timeData->getSignalData() + m_timeData->getSampleNumber() );
//        QVector<double> _v = QVector<double>::fromStdVector(_auxVector);

//    QVector<qreal> _v(m_timeData->getSampleNumber());
//    _v.fill(qrand());
//    _matrixData->setValueMatrix(_v,m_timeData->getSampleNumber()/100);
    uint binsPerSlice=4096.0;//qCeil(((qreal)m_timeData->getSampleNumber())/(4096.0));
    qDebug() << Q_FUNC_INFO << "binsPerSlice=" <<((qreal)m_timeData->getSampleNumber()) << "/"<< (4096.0) << "=" <<binsPerSlice;
    uint slices=qCeil( ((qreal)m_timeData->getSampleNumber()) / ((qreal)binsPerSlice));
    _spectrData->setData(m_timeData->getSignalData(),m_timeData->getSampleNumber(),slices,binsPerSlice);
    //m_spectrogram.setData(new SpectrogramData(m_timeData));
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
