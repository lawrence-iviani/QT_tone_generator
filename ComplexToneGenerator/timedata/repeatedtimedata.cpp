#include "repeatedtimedata.h"


RepeatedTimeData::RepeatedTimeData(QWidget *widget) :
    PartialTimeData(widget),
    m_blankTime(TIMEDATA_DEFAULT_BLANK)
{
    updateRepetitions();
    m_repeatedTimeDataUI=new RepeatedTimeDataUI(this,widget);
    connectSignal();
}

RepeatedTimeData::RepeatedTimeData(TimePlotParams * timePlotParams ,QWidget *widget) :
    PartialTimeData(timePlotParams),
    m_blankTime(TIMEDATA_DEFAULT_BLANK)
{
    updateRepetitions();
    m_repeatedTimeDataUI=new RepeatedTimeDataUI(this,widget);
    connectSignal();   
}

void RepeatedTimeData::connectSignal() {

    //Connect the slot to change the max duration
    connect(this ,SIGNAL(maxDurationChanged(qreal)),this,SLOT(updateRepetitions()));
    connect(this ,SIGNAL(startTimeChanged(qreal)),this,SLOT(updateRepetitions()));
    connect(this ,SIGNAL(durationChanged(qreal)),this,SLOT(updateRepetitions()));
    connect(this ,SIGNAL(maxDurationChanged(qreal)),m_repeatedTimeDataUI,SLOT(durationChange(qreal)));
    //EXAMPLE, to connect your class signal to update UI, use this example, connecting to the main control widget.
    //connect(this ,SIGNAL(SOME SIGNAL),getControlWidget(),SLOT(updateUI());
}

void RepeatedTimeData::updateRepetitions() {
    //Get the total time available for all the repetitions (including the first and orginal one)
    qreal _availableDuration=maxDuration()-startTime();

    //Calculate the one shot duration
    qreal _singleShotDuration=duration()+m_blankTime;

    m_repetitions=qFloor(_availableDuration/(_singleShotDuration));
    qDebug() << "RepeatedTimeData::updateRepetitions _availableDuration/"<< _availableDuration<<"_singleShotDuration"<<_singleShotDuration<< "="<< _availableDuration/_singleShotDuration;;
    qDebug() << "RepeatedTimeData::updateRepetitions m_repetitions=" <<m_repetitions;
}


void RepeatedTimeData::setBlankTime(qreal blanktime) {
    //the max  blank time must be less of the total length minus the length and start time of one shot signal (PartialTimeData)
    qreal _maxBlankTime=maxDuration()-duration()+startTime();
    Q_ASSERT(_maxBlankTime>=0);

    blanktime=(blanktime>=0 ? blanktime:0);
    //Clip to the max blanktime available
    m_blankTime=(blanktime < _maxBlankTime ? blanktime : _maxBlankTime);
    updateRepetitions();
    createData();
}

bool RepeatedTimeData::insertSignalValue(qint64 index, qreal value) {
    bool retval=false;
    const qreal * envelopData;
    qreal *_s=signalData();

    qint64 _sampleShotDuration=qFloor((duration()+m_blankTime)*sampleRate());

    qint64 lowestIndex=lowestSampleIndexForModification();
    if (isEnvelopeEnabled()) {
        envelopData=getEnvelopeData()->getEnvelope();
    }
    qreal _tVal;
    if ((lowestIndex<=index) && (index<=highestSampleIndexForModification())  ) {
        _tVal=(isEnvelopeEnabled() ? value*envelopData[index-lowestIndex] : value);
        qint64 i;
        for (unsigned int _rep=0; _rep < m_repetitions; _rep++) {
            i=_rep*_sampleShotDuration+index;
            Q_ASSERT(i<=sampleNumber());
            _s[i]=_tVal;
        }
        retval=true;
    }
    return retval;
}
