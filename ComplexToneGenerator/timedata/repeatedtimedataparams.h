#ifndef REPEATEDTIMEDATAPARAMS_H
#define REPEATEDTIMEDATAPARAMS_H

#include "DataUiHandler.h"
#include "errormessage.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "partialtimedataparams.h"

class TimePlotParams;

class RepeatedTimeDataParams : public PartialTimeDataParams
{
    Q_OBJECT
        Q_PROPERTY(qreal blanktime READ blankTime WRITE setBlankTime NOTIFY blankTimeChanged)
public:
    explicit RepeatedTimeDataParams(PartialTimeDataParams * baseProperty,TimePlotParams* params,QObject *parent);

    inline qreal blankTime() {return m_blankTime;}

signals:
    void blankTimeChanged(qreal);

public slots:
    void setBlankTime(qreal blankTime);

private:
    qreal m_blankTime;
};

#endif // REPEATEDTIMEDATAPARAMS_H
