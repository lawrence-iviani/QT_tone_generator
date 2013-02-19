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
        Q_PROPERTY(qreal blankTime READ blankTime WRITE setBlankTime NOTIFY blankTimeChanged)
public:
    explicit RepeatedTimeDataParams(QObject *parent);
    explicit RepeatedTimeDataParams(PartialTimeDataParams * baseProperty,TimePlotParams* params,QObject *parent);

    inline const qreal blankTime() {return m_blankTime;}

signals:
    void blankTimeChanged(qreal);

public slots:
    void setBlankTime(qreal blankTime);

private:
    qreal m_blankTime;
};

#endif // REPEATEDTIMEDATAPARAMS_H
