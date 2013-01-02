#ifndef GENERICTIMEDATA_H
#define GENERICTIMEDATA_H

#include <QObject>
#include <CTG_constants.h>
#include <qwt_series_data.h>
#include <qwt_plot_curve.h>
#include <math.h>
#include <QTime>
#include <QFileDialog>
#include <QMessageBox>
#include "ctg_app.h"
#include "envelope/dataenvelope.h"
#include "envelope/dataenvelopeparameters.h"
#include "timedata/generictimedataui.h"
#include "timedata/timedatacontrolui.h"
#include "plotwidget/timeplotwidgetparams.h"
#include "XML_utils/domhelper.h"

class GenericTimeDataUI;
class TimeDataControlUI;
class DataEnvelope;

/**
  * This class handle a generic time data object. Is useful to handle time based series as data container for a time data.
  */
class GenericTimeData : public QObject, public DomHelper
{
    Q_OBJECT
    Q_PROPERTY(qreal maxduration READ maxDuration WRITE setMaxDuration)
    Q_PROPERTY(qreal samplerate READ sampleRate WRITE setSampleRate)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(bool showcurve READ isShowEnabled WRITE setShowCurve)
    Q_PROPERTY(bool enable READ isEnabled WRITE setEnableCurve)
    Q_PROPERTY(bool enableenvelope READ isEnvelopeEnabled WRITE setEnableEnvelope)

public:
    explicit GenericTimeData(QWidget *widget=0);
    explicit GenericTimeData(TimePlotParams * timePlotParams,QWidget *widget=0);
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
    const QString& name() {return  m_name;}
    const QColor color() {return m_curve->pen().color(); }
    void setParent(QWidget* widget) {QObject::setParent((QObject*)widget);}

    /**
     * @brief insertSignalValue This function insert a qreal value in the sample position index and return true if the insertion is succesful, if return false the index is out of range
     * @param index the sample index where the value must be inserted
     * @param value the value to insert
     * @return True if the insert was succesful.
     */
    virtual bool insertSignalValue(qint64 index, qreal value);
    const qreal * getTimeData()   {return (const qreal*) m_t;}//return the pointer to internal data of the time signal. This should be a duplicate??
    const qreal * getSignalData() {return (const qreal*) m_s;}//return the pointer to internal data of the signal. This should be a duplicate??
    DataEnvelope * getEnvelopeData() {return m_envelope;}

    void forceRegenerateDomDocument() {this->regenerateDomDocument();}

    virtual bool isImportableByDomData(const QDomDocument & doc);
    virtual bool isImportableByDomData(const QDomDocument * doc);
    virtual bool isImportableByDomData(QDomNode& node);

signals:
    // setAndConvertFrequencyData(GenericFrequencyData * f); //Questo servira' a generare i dati partendo da una classe simile nel dominio frequenziale.

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

     /**
      * @brief maxDurationChanged emit a signal for the inerithed subclass to signal the length is changed. The Inerithed class should not call createData
      * because it's called by this function before exit, otherwise data will be created two times
      * @param maxduration
      */
     void maxDurationChanged(qreal maxduration);

     /**
      * @brief sampleRateChanged emit a signal for the inerithed subclass to signal the SR is changed. The Inerithed class should not call createData
      * because it's called by this function before exit, otherwise data will be created two times
      * @param samplerate
      */
     void sampleRateChanged(qreal samplerate);

public slots:
     /**
       * Set the max duration of this signal and update for data, redefine in the inerithed class if you want change beaviour. (ie don't call an update)
       * you have to change the singal length. Tipically you should override this method.
       */
     virtual bool setMaxDuration(qreal maxDuration);

     /**
       * Set the max duration of this signal and update for data, redefine in the inerithed class if you want change beaviour. (ie don't call an update).
       * Normally you shouldn't override this method
       */
     virtual bool setSampleRate(qreal SR);//Set the SR of this signal

     /**
      * @brief setName
      * @param name
      */
     void setName(QString name);

     /**
      * @brief setColor
      * @param color
      */
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
     /**
      * @brief updateData Update the actual array of data, overwriting the data actually stored in. Ask only for a recalc and avoid to reinit all the array. Faster mtehod.
       */
     void updateData();

     /**
      * @brief createData (Re)Create the internal data array (filling of zero), recalculating the sample number. Useful in case of length  array change.
      */
     void createData();

     /**
      * @brief exportXML open a dialog window and ask for the destionation XML file where to export the DOM data structure
      */
     void exportXML();

     /**
      * @brief exportXML export the DOM structure to a filename (path+file)
      * @param filename
      */
     void exportXML(const QString filename);

     /**
      * @brief importXML import an XML file by opening a file dialog
      * @return true if succesful
      */
     bool importXML();

     /**
      * @brief importXML import an XML file
      * @param fileName the file to import
      * @return  true if succesful
      */
     bool importXML(QString fileName);

     /**
      * @brief importXML import a QDomDocument
      * @param doc the DOM document to be imported
      * @return true if succesful
      */
     bool importXML(const QDomDocument * doc);

     /**
      * @brief importXML import a QDomDocument by node
      * @param node the node you want to import
      * @return true if succesful
      */
     bool importXML(const QDomNode *node);

     bool importXML(const QDomNode &node);

     /**
      * @brief enableRecalc enable/disable recalculation for this curve.
      * @param enable true, the curve is recalculated
      * @return the previous state.
      */
     bool setEnableRecalc(bool enable) {
         bool retval=m_enableRecalc;
         m_enableRecalc=enable;
         return retval;
     }

     /**
      * @brief inihbitUpdate Calling this function the curve becomes insensible to all the external call and function call
      */
     void inihbitUpdate();

     /**
      * @brief enableUpdate By calling this function the curve can be updated by an external signal or function call
      */
     void enableUpdate();

     /**
      * @brief setTimePlotParams
      * @param timePlotParams
      */
     void setTimePlotParams(TimePlotParams * timePlotParams);



     void copy();

     bool paste();

protected:
     /**
       * The method is called every time an updateData is called. In this way extension class can implement it own calculation to provide the signal data.
       * The inerithed class implement this method with its own code to generate signal data but eventually different time data (TIME DATA MODIFICATION NEVER TESTED BEFORE!!)
       */
     virtual void recalc() {} //Reimplement this method to update data with the want function, remeber to delete and regenearte m_data and m_curve.

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

     qreal * timeData() {return m_t;}
     qreal * signalData() {return m_s;}

     /**
       * The method provide capability to the extended class to set modified time data
       */
     void setTimeData(qreal * t, qint64 len);

     /**
       * The method provide capability to the extended class to set modified signal data
       */
     void setSignalData(qreal * s, qint64 len);

     /**
       * Return the lowest index available to modify the internal curve, is needed in inerithed class in order to insert correct recalculated values
       */
     virtual qint64 lowestSampleIndexForModification()  {
         qint64 retval=this->minStartTime()*this->sampleRate();
         Q_ASSERT(retval>=0);
         Q_ASSERT(retval<=this->sampleNumber());
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

     bool isValidMaxDuration(qreal  maxDuration) {return (m_TimePlotParams->duration()==maxDuration ?  true :  false);}

     bool isValidSampleRate(qreal  SR) {return (m_TimePlotParams->sampleRate()==SR ?  true :  false); }

     TimeDataControlUI *m_timeDataUI;
     bool m_enableRecalc;
     GenericTimeDataUI *m_genericTimeDataUI;

protected slots:
     void regenerateDomDocument();

private:
     void init(QWidget *widget);
     void connectSignal();
     void setTimePlotParams();
     QwtPlotCurve *m_curve;
     QwtCPointerData *m_data;
     QString m_name;
     TimePlotParams * m_TimePlotParams;
     qreal m_MaxDuration;//Duration, it's possible modify any of the parameter duration,t0,t1 to make modification to the length of the signal
     qreal m_Min_t0;//Start time to make calculation, The min value of time allowable constrained externally. This is NOT YET USED!! MAY BE BUGGED, it's always used as 0.0
     qreal m_SR;//The SR
     qreal * m_t;//The pointer to array of base time
     qreal * m_s;//The pointer to array of the signal
     qint64 m_sample;//Number of sample of m_s and m_t
     bool m_curveEnabled;
     DataEnvelope *m_envelope;
     bool m_enableEnvelope;
     QString m_fileName;


};

#endif // GENERICTIMEDATA_H
