#ifndef SQUAREDATA_H
#define SQUAREDATA_H

#include <QObject>
#include <QWidget>
#include "scaledsliderwidget.h"
#include "CTG_constants.h"
#include <math.h>

static qreal const SQUAREDATA_DEFAULT_FREQUENCY=1000;
static qreal const SQUAREDATA_DEFAULT_AMPLITUDE=0.5;
static qreal const SQUAREDATA_DEFAULT_MEAN=0.0;

class SquareParams {
public:
    explicit SquareParams();
    explicit SquareParams(qreal amplitude, qreal frequency, qreal meanValue);

    inline qreal amplitude() {return m_amplitude;}
    inline qreal frequency() {return m_frequency;}
    inline qreal meanValue() {return m_mean;}


    inline bool setAmplitude(qreal amplitude) {
        if (m_amplitude!=amplitude) {
            m_amplitude=amplitude;
            return true;
        }
        return false;
    }
    inline bool setFrequency(qreal frequency) {
        if (m_frequency!=frequency) {
            m_frequency=frequency;
            return true;
        }
        return false;
    }

    inline bool setMeanValue(qreal meanValue) {
        if (m_mean!=meanValue) {
            m_mean=meanValue;
            return true;
        }
        return false;
    }

private:
    qreal m_amplitude;
    qreal m_frequency;
    qreal m_mean;
};

class SquareUI : public QWidget {
    Q_OBJECT
public:
    explicit SquareUI(QWidget *widget = 0);
    virtual ~SquareUI() {}
signals:
    void amplitudeUIChanged(qreal);
    void frequencyUIChanged(qreal);
    void meanValueUIChanged(qreal);

public slots:
    void amplitudeUIUpdate(qreal amplitude);
    void frequencyUIUpdate(qreal frequency);
    void meanValueUIUpdate(qreal meanValue);

private:
    void initControlWidget();

    struct {
      ScaledSliderWidget *sliderFrequency;
      ScaledSliderWidget *sliderAmplitude;
      ScaledSliderWidget *sliderMeanValue;
    } m_squareDataControl;
};

#endif // SQUAREDATA_H
