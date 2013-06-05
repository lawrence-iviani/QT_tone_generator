#ifndef PINKNOISEDATA_H
#define PINKNOISEDATA_H

#include <QObject>
#include <QWidget>
#include "scaledsliderwidget.h"
#include "CTG_constants.h"

static qreal const PINKNOISE_DEFAULT_AVERAGE=0.0;
static qreal const PINKNOISE_DEFAULT_GAIN=1.0;

class PinkNoiseParams {
public:
    explicit PinkNoiseParams();
    explicit PinkNoiseParams(qreal average, qreal gain);

    inline qreal average() {return m_average;}
    inline qreal gain() {return m_gain;}

    inline bool setAverage(qreal average) {
        if (average>=-1.0 || average<=1.0)
            if (m_average!=average) {
                m_average=average;
                return true;
            }
        return false;
    }

    inline bool setGain(qreal gain) {
        if (gain>=0.0 || gain<=2.0)
            if (m_gain!=gain) {
                m_gain=gain;
                return true;
            }
        return false;
    }

private:
    qreal m_average;
    qreal m_gain;
};

class PinkNoiseUI : public QWidget {
    Q_OBJECT
public:
    explicit PinkNoiseUI(QWidget *widget = 0);
    virtual ~PinkNoiseUI() {}
signals:
    void averageUIChanged(qreal);
    void gainUIChanged(qreal);

public slots:
    void averageUIUpdate(qreal average);
    void gainUIUpdate(qreal gain);

private:
    void initControlWidget();

    struct {
      ScaledSliderWidget *sliderAverage;
      ScaledSliderWidget *sliderGain;
    } m_pinkNoiseDataControl;

};


#endif // PINKNOISEDATA_H
