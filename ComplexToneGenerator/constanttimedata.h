#ifndef PARTIALCONSTANTTIMEDATA_H
#define PARTIALCONSTANTTIMEDATA_H

#include "partialtimedata.h"
#include "generictimedata.h"

class PartialConstantTimeData : public PartialTimeData
{
    Q_OBJECT
public:

    PartialConstantTimeData(double duration, double SRGen);
    double amplitude() {return m_amplitude;}

signals:

public slots:
    void setAmplitude(double amplitude);

protected:
    virtual void recalc();

 private:

    static double const CONSTDATA_DEFAULT_AMPLITUDE=0.5;

    double m_amplitude;
    QFrame * m_widgetControl;
    void initControl();

    struct {
      ScaledSliderWidget * sliderAmplitude;
    } m_dataControl;
};

#endif // PARTIALCONSTANTTIMEDATA_H

#ifndef CONSTANTTIMEDATA_H
#define CONSTANTTIMEDATA_H

#include "generictimedata.h"

class ConstantTimeData : public GenericTimeData
{
    Q_OBJECT
public:
    ConstantTimeData(double duration, double SRGen);
    double amplitude() {return m_amplitude;}

signals:

public slots:
    void setAmplitude(double amplitude);

protected:
    virtual void recalc();

 private:

    static double const CONSTDATA_DEFAULT_AMPLITUDE=0.5;

    double m_amplitude;
    QFrame * m_widgetControl;
    void initControl();

    struct {
      ScaledSliderWidget * sliderAmplitude;
    } m_dataControl;
};

#endif // CONSTANTTIMEDATA_H
