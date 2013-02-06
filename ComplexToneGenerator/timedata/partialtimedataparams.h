#ifndef PARTIALTIMEDATAPARAMS_H
#define PARTIALTIMEDATAPARAMS_H

#include "DataUiHandler.h"
#include "errormessage.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "generictimedataparams.h"

class TimePlotParams;

class PartialTimeDataParams : public GenericTimeDataParams
{
    Q_OBJECT
    Q_PROPERTY(qreal duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(qreal t0 READ t0 WRITE sett0 NOTIFY t0Changed)

public:
    explicit PartialTimeDataParams(QObject *parent);
    explicit PartialTimeDataParams(GenericTimeDataParams * baseProperty,TimePlotParams* params,QObject *parent);

    inline qreal duration() {return m_duration;}
    inline qreal t0() {return m_t0;}

signals:
    void durationChanged(qreal);
    void t0Changed(qreal);

public slots:
    void setDuration(qreal duration);
    void sett0(qreal t0);

private:
    qreal m_t0;
    qreal m_duration;

};

#endif // PARTIALTIMEDATAPARAMS_H
