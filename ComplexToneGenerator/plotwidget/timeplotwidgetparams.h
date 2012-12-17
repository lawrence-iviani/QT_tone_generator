#ifndef PLOTWIDGETPARAMS_H
#define PLOTWIDGETPARAMS_H

#include <QObject>
#include "CTG_constants.h"
#include "XML_utils/domhelper.h"

/**
 * @brief The PlotWidgetParams class
 * setter/getter class to manage the time project parameters (SR, duration)
 * These need a deep redesign to use signal/slot
 */
class TimePlotParams : public QObject , public DomHelper
{
    Q_OBJECT
    Q_PROPERTY(qreal duration READ duration WRITE setDuration)
    Q_PROPERTY(qreal sampleRate READ sampleRate WRITE setSampleRate)
    Q_PROPERTY(qreal minTime READ minTime WRITE setMinTime)

public:
    explicit TimePlotParams(QObject *parent = 0);
    explicit TimePlotParams(qreal duration, qreal sampleRate, QObject *parent = 0);
    
    qreal sampleRate() {return m_sampleRate;}
    qreal duration() {return m_duration;}
    qreal minTime() {return m_t0;}

signals:
    //NOT IMPLEMENTED, NEEDS A DEEP MODIFICATION IN TIMEPLOTWIDGET AND GENERICTIMEDATA
  //  void durationChanged(qreal);
  //  void sampleRateChanged(qreal);
  //  void minTimeChanged(qreal);

public slots:
    bool setDuration(qreal duration);
    bool setSampleRate(qreal sampleRate);
    void setMinTime(qreal minTime) {Q_UNUSED(minTime);}//do nothing for now, is mandatory set to 0
    void regenerateDomDocument();
private:
    qreal m_duration;
    qreal m_sampleRate;
    qreal m_t0;
};

#endif // PLOTWIDGETPARAMS_H
