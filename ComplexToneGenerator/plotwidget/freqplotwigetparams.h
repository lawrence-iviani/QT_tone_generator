#ifndef FREQPLOTWIGETPARAMS_H
#define FREQPLOTWIGETPARAMS_H

#include <QObject>
#include <DataUiHandler.h>
#include "XML_utils/domhelper.h"

const uint FREQPLOTWIDGET_DEFAULT_BINS_PER_WINDOW=8192;
const QString FREQPLOTWIDGET_DEFAULT_WINDOW="Hamming";
const qreal FREQPLOTWIDGET_DEFAULT_OVERLAP=0.0;

class FreqPlotWigetParams :public DataUiHandlerProperty
{
    Q_OBJECT
    Q_PROPERTY(uint binsNumber READ binsNumber WRITE setBinsNumber NOTIFY binsNumberChanged)
    Q_PROPERTY(QString windowType READ windowType WRITE setWindowType NOTIFY windowTypeChanged)
    Q_PROPERTY(qreal overlap READ overlap WRITE setOverlap NOTIFY overlapChanged)
    Q_PROPERTY(bool logFreqScale READ logFreqScale WRITE setLogFreqScale NOTIFY logFreqScaleChanged)

public:
    explicit FreqPlotWigetParams(QObject *object = 0);

    uint binsNumber() {return m_binsNumber;}
    QString windowType() {return m_windowType;}
    qreal overlap() {return m_overlap;}
    bool logFreqScale() {return m_logFreqScale;}

signals:
    void binsNumberChanged(uint nBins);
    void windowTypeChanged(QString windowName);
    void overlapChanged(qreal overlap);
    void logFreqScaleChanged(bool logScale);

public slots:
    void setBinsNumber(uint nBins);
    void setWindowType(QString windowName);
    void setOverlap(qreal overlap);
    void setLogFreqScale(bool logScale);

private:
    uint m_binsNumber;
    QString m_windowType;
    qreal m_overlap;
    bool m_logFreqScale;
};

#endif // FREQPLOTWIGETPARAMS_H
