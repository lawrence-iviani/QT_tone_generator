#include "generictimedataparams.h"

GenericTimeDataParams::GenericTimeDataParams(QObject *object) :
    DataUiHandlerProperty(object),
    m_startTime(0),
    m_name(QString("no name")),
    m_curveEnabled(true) //manca showcurve!!!
{
    setSampleRate(TIMEDATA_DEFAULT_SR);
    setMaxDuration(TIMEDATA_DEFAULT_PROJECT_TIME);
}

GenericTimeDataParams::GenericTimeDataParams(DataUiHandlerProperty * baseProperty, TimePlotParams* params, QObject *parent ) :
    DataUiHandlerProperty(baseProperty,parent),
    m_startTime(0),
    m_name(QString("no name")),
    m_curveEnabled(true)  //manca showcurve!!!
{
    setSampleRate(params->sampleRate());
    setMaxDuration(params->maxDuration());
}


void GenericTimeDataParams::setSampleRate(qreal SR) {
    if (SR > TIMEDATA_DEFAULT_MAX_SR ) return;
    if ( (SR!=m_SR) && (SR >0) ) {
        m_SR=SR;
        emit (sampleRateChanged(SR));
    }
}

void GenericTimeDataParams::setMaxDuration(qreal  maxDuration) {
    if ( (maxDuration > TIMEDATA_DEFAULT_MAX_TIME) || (maxDuration < 0) ) return;
    if (maxDuration!=m_maxDuration) {
        if (maxDuration < 0) {
            m_maxDuration=0;
        } else {
            m_maxDuration=maxDuration;
        }
        emit(maxDurationChanged(maxDuration));
    }
}

void GenericTimeDataParams::setName(QString name) {
    if (m_name!=name) {
        m_name=name;
       // m_curve->setTitle(m_name);
        emit(nameChanged(name));
    }
}

void GenericTimeDataParams::setStartTime(qreal startTime) {
    Q_UNUSED(startTime);
    PRINT_WARNING(ErrorMessage::WARNING(Q_FUNC_INFO,"setStartTime not yet enabled"));
}

void GenericTimeDataParams::setColor(QColor color) {
    if (m_color!=color) {
        m_color=color;
        emit (colorChanged(color));
    }
}

void GenericTimeDataParams::setShowCurve(bool show) {
    if (m_showEnabled!=show) {
        if (m_curveEnabled) {
            m_showEnabled=show;
            emit (showCurveChanged(show));
        } else {
            m_showEnabled=false;
            emit (showCurveChanged(false));
        }
    }
}

void GenericTimeDataParams::setEnableCurve(bool enable){
    if (m_curveEnabled!=enable) {
        m_curveEnabled=enable;
        emit (enableCurveChanged(enable));
        if (!enable) {
            setShowCurve(false);
        } else {
            setShowCurve(true);
        }
    }

}
