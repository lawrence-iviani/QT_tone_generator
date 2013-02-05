#ifndef GENERICTIMEDATA_H
#define GENERICTIMEDATA_H

#include <QObject>
#include <CTG_constants.h>
#include <qwt_series_data.h>
#include <qwt_plot_curve.h>
#include <math.h>
#include "ctg_app.h"
#include "envelope/dataenvelope.h"
#include "DataUiHandler.h"
#include "timedata/generictimedataui.h"
#include "timedata/generictimedataparams.h"
#include "plotwidget/timeplotwidgetparams.h"

class GenericTimeDataUI;

//class GenericTimeDataDelegate : public DataUiHandlerDelegate
//{
//public:
//    GenericTimeDataDelegate(GenericTimeDataParams * params, GenericTimeDataUI * ui , QObject *parent = 0) :
//        DataUiHandlerDelegate((DataUiHandlerProperty *)params,(DataUiHandlerUI*)ui,"GTDDocument","Envelope_GTD",parent)
//    {}
//    virtual ~GenericTimeDataDelegate() {}
//};

/**
  * This class handle a generic time data object. Is useful to handle time based series as data container for a time data.
  */
class GenericTimeData : public QObject
{
    Q_OBJECT

public:
    explicit GenericTimeData(QObject *parent=0);
    explicit GenericTimeData(TimePlotParams * timePlotParams,QObject *parent=0);
    virtual ~GenericTimeData();
    inline QwtPlotCurve * getCurve() {return m_curve;}
    inline QwtCPointerData * getData() {return m_data;}
    QWidget * getControlWidget() {return dynamic_cast<QWidget*>(m_timeDataDelegate->getUI()); }// Give back a QWidget that can be used to control this class.
    DataUiHandlerProperty* getDataParameters() {return dynamic_cast<DataUiHandlerProperty*>(m_timeDataDelegate->getProperty());}
    inline DataUiHandlerDelegate* getDelegate() {return dynamic_cast<DataUiHandlerDelegate*>(m_timeDataDelegate);}
    inline quint64 getSampleNumber()  {return m_sample;}


    inline const qreal * getTimeData()   {return (const qreal*) m_t;}//return the pointer to internal data of the time signal. This should be a duplicate??
    inline const qreal * getSignalData() {return (const qreal*) m_s;}//return the pointer to internal data of the signal. This should be a duplicate??
    inline DataEnvelope * getEnvelopeData() {return m_envelope;}

    inline bool isEnableRecalc() {return m_enableRecalc;}
//    void forceRegenerateDomDocument() {this->regenerateDomDocument();}
//    virtual bool isImportableByDomData(const QDomDocument & doc);
//    virtual bool isImportableByDomData(const QDomDocument * doc);
//    virtual bool isImportableByDomData(QDomNode& node);

signals:
    // setAndConvertFrequencyData(GenericFrequencyData * f); //Questo servira' a generare i dati partendo da una classe simile nel dominio frequenziale.

    /**
      * This signal is emitted whenever the data, and therefore the curve are updated
      */
     void dataChanged();

     /**
       * This signal is emitted when only the name is updated (this avoid curve recalc and data plot)
       */
     void nameChanged(QString);

     /**
       * This signal is emitted whenever curve attribute (color, name etc) are updated, and therefore only the curve need to be reploted and not recalculated
       */
     void curveAttributeChanged();

//     /**
//      * @brief maxDurationChanged emit a signal for the inerithed subclass to signal the length is changed. The Inerithed class should not call createData
//      * because it's called by this function before exit, otherwise data will be created two times
//      * @param maxduration
//      */
//     void maxDurationChanged(qreal maxduration);

//     /**
//      * @brief sampleRateChanged emit a signal for the inerithed subclass to signal the SR is changed. The Inerithed class should not call createData
//      * because it's called by this function before exit, otherwise data will be created two times
//      * @param samplerate
//      */
//     void sampleRateChanged(qreal samplerate);

public slots:
     /**
       * Set the max duration of this signal and update for data, redefine in the inerithed class if you want change beaviour. (ie don't call an update)
       * you have to change the singal length. Tipically you should override this method.
       */
     virtual void maxDurationHasChanged(qreal maxDuration);

     /**
       * Set the max duration of this signal and update for data, redefine in the inerithed class if you want change beaviour. (ie don't call an update).
       * Normally you shouldn't override this method
       */
    virtual void sampleRateHasChanged(qreal SR);

     /**
       * Update the actual array of data, overwriting the data actually stored in. Ask only for a recalc and avoid to reinit all the array. Faster mtehod.
       * Override if you need some different beahviour (ie in this is ovveride in DigestTimeData
       */
     /**
      * @brief updateData Update the actual array of data, overwriting the data actually stored in. Ask only for a recalc and avoid to reinit all the array. Faster mtehod.
       */
     virtual void updateData();

     /**
      * @brief createData (Re)Create the internal data array (filling of zero), recalculating the sample number. Useful in case of length  array change.
      */
     virtual void createData();


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


     /**
      * @brief curveHasChanged Call this slot when some attributes curve changed
      */
     void curveHasChanged();

//     /**
//      * @brief copy Copy in an apposite application structure this curve
//      */
//     void copy();

//     /**
//      * @brief paste Paste from an apposite application structure to this curve
//      * @return
//      */
//     bool paste();

     //     /**
     //      * @brief exportXML open a dialog window and ask for the destionation XML file where to export the DOM data structure
     //      */
     //     void exportXML();

     //     /**
     //      * @brief exportXML export the DOM structure to a filename (path+file)
     //      * @param filename
     //      */
     //     void exportXML(const QString filename);

     //     /**
     //      * @brief importXML import an XML file by opening a file dialog
     //      * @return true if succesful
     //      */
     //     bool importXML();

     //     /**
     //      * @brief importXML import an XML file
     //      * @param fileName the file to import
     //      * @return  true if succesful
     //      */
     //     bool importXML(QString fileName);

     //     /**
     //      * @brief importXML import a QDomDocument
     //      * @param doc the DOM document to be imported
     //      * @return true if succesful
     //      */
     //     bool importXML(const QDomDocument * doc);

     //     /**
     //      * @brief importXML import a QDomDocument by node
     //      * @param node the node you want to import
     //      * @return true if succesful
     //      */
     //     bool importXML(const QDomNode *node);

     //     bool importXML(const QDomNode &node);

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
     inline void deleteAllData();

     /**
       * The function init all the following internal data:
       * - Signal
       * - Time
       */
     inline void resetAllData();


     void createDataCurve();//Set

     inline qreal * timeData() {return m_t;}
     inline qreal * signalData() {return m_s;}

     /**
       * The method provide capability to the extended class to set modified time data
       */
     void setTimeData(qreal * t, quint64 len);

     /**
       * The method provide capability to the extended class to set modified signal data
       */
     void setSignalData(qreal * s, quint64 len);

     /**
       * Return the lowest index available to modify the internal curve, is needed in inerithed class in order to insert correct recalculated values
       */
    inline virtual quint64 lowestSampleIndexForModification()  {
         GenericTimeDataParams *_gtdp=(GenericTimeDataParams *) getDataParameters();
         quint64 retval=_gtdp->startTime()*_gtdp->sampleRate();
         Q_ASSERT(retval<=m_sample);
         return retval;
     }

     /**
       * Return the lowest index available to modify the internal curve, is needed in inerithed class in order to insert correct recalculated values
       */
     inline virtual quint64 highestSampleIndexForModification() {
         GenericTimeDataParams *_gtdp=(GenericTimeDataParams *)getDataParameters();
         quint64 retval=(_gtdp->startTime()+_gtdp->maxDuration())*_gtdp->sampleRate();
         Q_ASSERT(retval<=m_sample);
         return retval;
     }

     /**
      * @brief setEnvelopeLength Set effective length of the envelope data, this class
      * @param length
      */
    // virtual void setEnvelopeLength(qreal length);

     /**
      * @brief connectSignals WHen inheriting from this class this method must be called after init to mantains connection, otherwise will be lost.
      * (This is an issue that must be fixed in the design).
      */
     virtual void connectSignals();

     /**
      * @brief insertSignalValue This function insert a qreal value in the sample position index and return true if the insertion is succesful, if return false the index is out of range
      * @param index the sample index where the value must be inserted
      * @param value the value to insert
      * @return True if the insert was succesful.
      */
     virtual bool insertSignalValue(quint64 index, qreal value);

protected slots:


private:
     void init(TimePlotParams * timePlotParams=0);
     void initTimePlotParams();
     bool m_enableRecalc;
    // TimePlotParams * m_TimePlotParams;
     DataUiHandlerDelegate * m_timeDataDelegate;
     DataEnvelope *m_envelope;
     QwtPlotCurve *m_curve;
     QwtCPointerData *m_data;

   //  const DataEnvelopeParameters * m_envParams;
     qreal * m_t;//The pointer to array of base time
     qreal * m_s;//The pointer to array of the signal
     quint64 m_sample;//Number of sample of m_s and m_t

    // QString m_fileName;
};

#endif // GENERICTIMEDATA_H
