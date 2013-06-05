#include "freqplotwigetparams.h"

FreqPlotWigetParams::FreqPlotWigetParams(QObject *object):
        DataUiHandlerProperty(object),
        m_binsNumber(FREQPLOTWIDGET_DEFAULT_BINS_PER_WINDOW),
        m_windowType(FREQPLOTWIDGET_DEFAULT_WINDOW),
        m_overlap(FREQPLOTWIDGET_DEFAULT_OVERLAP),
        m_logFreqScale(true)
{
}

void FreqPlotWigetParams::setBinsNumber(uint nBins) { //setMaxDuration(qreal maxduration) {
    if (nBins!=m_binsNumber) {
        m_binsNumber=nBins;
        emit (binsNumberChanged(nBins));
    }
}

void FreqPlotWigetParams::setOverlap(qreal overlap) { //setMaxDuration(qreal maxduration) {
    if (overlap<0.0 || overlap > 100.0) return;
    if (overlap!=m_overlap) {
        m_overlap=overlap;
        emit (overlapChanged(overlap));
    }
}

void FreqPlotWigetParams::setWindowType(QString windowName) {
    if (windowName!=m_windowType) {
        m_windowType=windowName;
        emit (windowTypeChanged(windowName));
    }
}

void FreqPlotWigetParams::setLogFreqScale(bool logScale) {
    if (logScale!=m_logFreqScale) {
        m_logFreqScale=logScale;
        emit (logFreqScaleChanged(logScale));
    }
}
