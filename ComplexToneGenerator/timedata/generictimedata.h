#ifndef GENERICTIMEDATA_H
#define GENERICTIMEDATA_H

#include <QObject>
#include <CTG_constants.h>
#include <qwt_series_data.h>
#include <qwt_plot_curve.h>
#include <math.h>
#include  <QTime>
#include <envelope/dataenvelope.h>
#include <envelope/dataenvelopeparameters.h>
#include "timedata/generictimedataui.h"
#include "timedata/timedatacontrolui.h"

class GenericTimeDataUI;
class TimeDataControlUI;

/**
  * This class handle a generic time data object. Is useful to handle time based series as data container for a time data.
  */
class GenericTimeData : public QObject
{
    Q_OBJECT
public:
    GenericTimeData(QObject *parent=0,QWidget *widget=0);
    GenericTimeData(qreal maxDuration, qreal SRGen,QObject *parent=0,QWidget *widget=0);
    virtual ~GenericTimeData();
    QwtPlotCurve * getCurve() {return m_curve;}
    QwtCPointerData * getData() {return m_data;}
    TimeDataControlUI * getControlWidget() {return m_timeDataUI; }// Give back a QWidget that can be used to control this class.

    qreal duration() {return m_MaxDuration;}
    qreal maxDuration() {return m_MaxDuration;}
    qreal sampleRate() {return m_SR;}
    qreal minStartTime() {return m_Min_t0;}//Return the  min start time, for now is fix to 0.0, future version may allows different values.
    qint64 sampleNumber() {return m_sample;}
    bool isEnabled() { return m_curveEnabled;}
    bool isShowEnabled() { return m_curve->isVisible();}
    bool isEnvelopeEnabled() { return m_enableEnvelope;}
    QString name() {return  m_name;}
    QColor color() {return m_curve->pen().color(); }

    bool insertSignalValue(qint64 index, qreal value);
    const qreal * getTimeData()   {return (const qreal*) m_t;}//return the pointer to internal data of the time signal. This should be a duplicate??
    const qreal * getSignalData() {return (const qreal*) m_s;}//return the pointer to internal data of the signal. This should be a duplicate??
    DataEnvelope * getEnvelopeData() {return m_envelope;}
    // setAndConvertFrequencyData(GenericFrequencyData * f); //Questo servira' a generare i dati partendo da una classe simile nel dominio frequenziale.
signals:
    /**
      * This signal is emitted whenever the data, and therefore the curve are updated
      */
     void dataUpdated();

     /**
       * This signal is emitted when only the name is updated (this avoid curve recalc and data plot)
       */
     void nameChanged();

     /**
       * This signal is emitted whenever curve attribute (color, name etc) are updated, and therefore only the curve need to be reploted.
       */
     void curveAttributeUpdated();

public slots:
     /**
       * Set the max duration of this signal and update for data, redefine in the inerithed class if you want change beaviour. (ie don't call an update)
       * you have to change the singal length. Tipically you should override this method.
       */
     virtual void setMaxDuration(qreal maxDuration);

     /**
       * Set the max duration of this signal and update for data, redefine in the inerithed class if you want change beaviour. (ie don't call an update).
       * Normally you shouldn't override this method
       */
     virtual void setSampleRate(qreal SR);//Set the SR of this signal

     //void setMinStartTimeAndMaxDuration(qreal t0, qreal maxDuration);//Set the min start time and the max duration of the signal.
     //void setMinStartTime(qreal t0);//Set the min start timethis signal.
     //void setMaxEndTime(qreal t1);//Set the max start time  of this signal.

     void setName(QString name);
     void setColor(QColor color);

     /**
       * Enable the curve in any eventual calculation (ie digest)
       */
     void setEnableCurve(bool enable);

     /**
       * Show/hide the curve but keep enabled in the calculation (ie digest)
       */
     void setShowCurve(bool enable);

     /**
       * Enable/disable the envevope related to this data curve.
       */
     void setEnableEnvelope(bool enable);


     /**
       * Update the actual array of data, overwriting the data actually stored in. Ask only for a recalc and avoid to reinit all the array. Faster mtehod.
       */
     void updateData();

     /**
       * (Re)Create the internal data array, recalculating the sample number. Useful in case of length  array change.
       */
     void createData();

protected:
     /**
       * The method is called every time and updateData is called. In this way extension class can implement it own calculation to provide the signal data.
       * The inerithed class implement this method with its own code to generate signal data but eventually different time data (TIME DATA MODIFICATION NEVER TESTED BEFORE!!)
       */
     virtual void recalc() {} //Reimplement this method to update data with the want function, rembert to delete and regenearte m_data and m_curve.

     /**
       * The function delete all the following internal reference data:
       * - Signal
       * - Time
       * - Envelope
       */
     void deleteAllData();

     /**
       * The function init all the following internal data:
       * - Signal
       * - Time
       */
     void resetAllData();


     void createDataCurve();//Set

     /**
       * The method provide capability to the extended class to set modified time data
       */
     void setTimeData(qreal * t, qint64 len);

     /**
       * The method provide capability to the extended class to set modified signal data
       */
     void setSignalData(qreal * s, qint64 len);

     /**
       * This method leaves the decision to update data after set the duration or not, useful in some inerithed class when you want do other stuff before update
       */
     void setMaxDurationAndUpdate(qreal  maxDuration, bool updateData);

     /**
       * This method leaves the decision to update data after set the SR or not, useful in some inerithed class when you want do other stuff before update
       */
     void setSampleRateAndUpdate(qreal SR, bool updateData);

     /**
       * Return the lowest index available to modify the internal curve, is needed in inerithed class in order to insert correct recalculated values
       */
     virtual qint64 lowestSampleIndexForModification()  {
         qint64 retval=this->minStartTime()*this->sampleRate();
         Q_ASSERT(retval >=0);
         Q_ASSERT(retval <=this->sampleNumber());
         return retval;
     }

     /**
       * Return the lowest index available to modify the internal curve, is needed in inerithed class in order to insert correct recalculated values
       */
     virtual qint64 highestSampleIndexForModification() {
         qint64 retval=(this->minStartTime()+this->duration())*this->sampleRate();
         Q_ASSERT(retval >=0);
         Q_ASSERT(retval <=this->sampleNumber());
         return retval;
     }

     TimeDataControlUI *m_timeDataUI;

private:
     void init(QWidget *widget);
     void connectSignal();
     QwtPlotCurve *m_curve;
     QwtCPointerData *m_data;
     QString m_name;
     qreal m_MaxDuration;//Duration, it's possible modify any of the parameter duration,t0,t1 to make modification to the length of the signal
     qreal m_Min_t0;//Start time to make calculation, The min value of time allowable constrained externally. This is NOT YET USED!! MAY BE BUGGED, it's always used as 0.0
     qreal m_SR;//The SR
     qreal * m_t;//The pointer to array of base time
     qreal * m_s;//The pointer to array of the signal
     qint64 m_sample;//Number of sample of m_s and m_t
     bool m_curveEnabled;
     DataEnvelope *m_envelope;
     bool m_enableEnvelope;
     GenericTimeDataUI *m_genericTimeDataUI;
};

#endif // GENERICTIMEDATA_H
