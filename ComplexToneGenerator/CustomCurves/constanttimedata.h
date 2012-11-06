#ifndef PARTIALCONSTANTTIMEDATA_H
#define PARTIALCONSTANTTIMEDATA_H

#include "timedata/partialtimedata.h"
#include "timedata/generictimedata.h"

class PartialConstantTimeData : public PartialTimeData
{
    Q_OBJECT
public:

    PartialConstantTimeData(qreal duration, qreal SRGen);
    qreal amplitude() {return m_amplitude;}

signals:

public slots:
    void setAmplitude(qreal amplitude);

protected:
    virtual void recalc();

 private:

    static qreal const CONSTDATA_DEFAULT_AMPLITUDE=0.5;

    qreal m_amplitude;
    QFrame * m_widgetControl;
    void initControl();

    struct {
      ScaledSliderWidget * sliderAmplitude;
    } m_dataControl;
};

#endif // PARTIALCONSTANTTIMEDATA_H

#ifndef CONSTANTTIMEDATA_H
#define CONSTANTTIMEDATA_H

#include "timedata/generictimedata.h"

class ConstantTimeData : public GenericTimeData
{
    Q_OBJECT
public:
    ConstantTimeData(qreal duration, qreal SRGen);
    qreal amplitude() {return m_amplitude;}

signals:

public slots:
    void setAmplitude(qreal amplitude);

protected:
    virtual void recalc();

 private:

    static qreal const CONSTDATA_DEFAULT_AMPLITUDE=0.5;

    qreal m_amplitude;
    QFrame * m_widgetControl;
    void initControl();

    struct {
      ScaledSliderWidget * sliderAmplitude;
    } m_dataControl;
};

#endif // CONSTANTTIMEDATA_H
