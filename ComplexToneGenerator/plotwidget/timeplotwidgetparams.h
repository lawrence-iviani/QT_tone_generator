#ifndef PLOTWIDGETPARAMS_H
#define PLOTWIDGETPARAMS_H

#include <QObject>
#include <DataUiHandler.h>
#include "CTG_constants.h"
#include "XML_utils/domhelper.h"

/**
 * @brief The PlotWidgetParams class
 * setter/getter class to manage the time project parameters (SR, duration)
 * These need a deep redesign to use signal/slot
 */
class TimePlotParams : public DataUiHandlerProperty
{
    Q_OBJECT
    Q_PROPERTY(qreal maxDuration READ maxDuration WRITE setMaxDuration NOTIFY maxDurationChanged)
    Q_PROPERTY(qreal sampleRate READ sampleRate WRITE setSampleRate NOTIFY sampleRateChanged)
    Q_PROPERTY(qreal minTime READ minTime WRITE setMinTime NOTIFY minTimeChanged)

public:
    explicit TimePlotParams(QObject *object = 0);
    explicit TimePlotParams(qreal maxDuration, qreal sampleRate, DataUiHandlerProperty * baseProperty, QObject *object = 0);

    qreal sampleRate() {return m_sampleRate;}
    qreal maxDuration() {return m_maxDuration;}
    qreal minTime() {return m_t0;}

signals:
   void maxDurationChanged(qreal);
   void sampleRateChanged(qreal);
   void minTimeChanged(qreal);

public slots:
    void setMaxDuration(qreal maxduration);
    void setSampleRate(qreal sampleRate);
    void setMinTime(qreal minTime) {Q_UNUSED(minTime);}//do nothing for now, is mandatory set to 0

private:
    qreal m_maxDuration;
    qreal m_sampleRate;
    qreal m_t0;
};

#endif // PLOTWIDGETPARAMS_H
