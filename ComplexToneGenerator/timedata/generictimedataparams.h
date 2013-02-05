#ifndef GENERICTIMEDATAPARAMS_H
#define GENERICTIMEDATAPARAMS_H

#include "DataUiHandler.h"
#include "errormessage.h"
#include "plotwidget/timeplotwidgetparams.h"

class TimePlotParams;

class GenericTimeDataParams  : public DataUiHandlerProperty
{
    Q_OBJECT
    Q_PROPERTY(qreal maxDuration READ maxDuration WRITE setMaxDuration NOTIFY maxDurationChanged)
    Q_PROPERTY(qreal startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(qreal sampleRate READ sampleRate WRITE setSampleRate NOTIFY sampleRateChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool showCurve READ isShowEnabled WRITE setShowCurve NOTIFY showCurveChanged)
    Q_PROPERTY(bool enableCurve READ isCurveEnabled WRITE setEnableCurve NOTIFY enableCurveChanged)
    //Q_PROPERTY(bool enableenvelope READ isEnvelopeEnabled WRITE setEnableEnvelope)

public:
    explicit GenericTimeDataParams(QObject *object = 0);
    explicit GenericTimeDataParams(DataUiHandlerProperty * baseProperty, TimePlotParams* params=0, QObject *parent=0);

    inline qreal maxDuration() {return m_maxDuration;}
    inline qreal sampleRate() {return m_SR;}
    inline qreal startTime() {return m_startTime;}//Return the  min start time, for now is fix to 0.0, future version may allows different values.

    inline bool isEnabled() { return m_curveEnabled;}
    inline bool isShowEnabled() { return m_showEnabled;}
    inline bool isCurveEnabled() { return m_curveEnabled;}

    const QString& name() {return  m_name;}
    const QColor color() {return m_color;}
//    inline bool isEnvelopeEnabled() { return m_enableEnvelope;}
//    inline qint64 sampleNumber() {return m_sample;}


signals:
    void maxDurationChanged(qreal);
    void startTimeChanged(qreal);
    void sampleRateChanged(qreal);
    void nameChanged(QString);
    void colorChanged(QColor);
    void showCurveChanged(bool);
    void enableCurveChanged(bool);

public slots:
    void setMaxDuration(qreal duration);
    void setStartTime(qreal startTime);
    void setSampleRate(qreal SR);
    void setName(QString name);
    void setColor(QColor color);
    void setShowCurve(bool show);
    void setEnableCurve(bool enable);

private:
    qreal m_maxDuration;//Duration, it's possible modify any of the parameter duration,t0,t1 to make modification to the length of the signal
    qreal m_startTime;//Start time to make calculation, The min value of time allowable constrained externally. This is NOT YET USED!! MAY BE BUGGED, it's always used as 0.0
    qreal m_SR;//The SR
    QString m_name;
    bool m_curveEnabled;
    bool m_showEnabled;

   // bool m_enableEnvelope;
    QColor m_color;
};

#endif // GENERICTIMEDATAPARAMS_H
