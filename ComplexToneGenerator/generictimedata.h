#ifndef GENERICTIMEDATA_H
#define GENERICTIMEDATA_H

#include <QObject>
#include <CTG_constants.h>
#include <qwt_series_data.h>
#include <qwt_plot_curve.h>
#include <math.h>
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QCheckBox>
#include <scaledsliderwidget.h>
#include <comboboxwidgetcolor.h>
#include  <QTime>

/**
  * This class handle a generic time data object. Is useful to handle time based series as data container for a time data.
  */

class GenericTimeData : public QObject
{
    Q_OBJECT
public:
    GenericTimeData();
    GenericTimeData(double maxDuration, double SRGen);
    virtual ~GenericTimeData();
    QwtPlotCurve * getCurve() {return m_curve;}
    QwtCPointerData * getData() {return m_data;}
    QWidget * getControlWidget() {return m_allControl; }// Give back a QWidget that can be used to control this class.

    double maxDuration(){return m_MaxDuration;}
    double sampleRate() {return m_SR;}
    double minStartTime(){return m_Min_t0;}//Return the  min start time, for now is fix to 0.0, future version may allows different values.
   // double maxEndTime(){return m_Max_t1;}
    long int sampleNumber() {return m_sample;}
    bool isEnabled() { return m_curveEnabled;}
    bool isShowEnabled() { return m_curve->isVisible();}
    QString name() {return  m_name;}
    QColor color() {return m_curve->pen().color(); }

    // setAndConvertFrequencyData(GenericFrequencyData * f); //Questo servira' a generare i dati partendo da una classe simile nel dominio frequenziale.
signals:
     void dataUpdated();
     //void maxDurationUpdate(double maxDuration);
    // void minStartTimeUpdate(double t0);
    // void maxEndTimeUpdate(double t1);

public slots:
     /**
       * Set the max duration of this signal and update for data, redefine in the inerithed class if you want change beaviour. (ie don't call an update)
       * you have to change the singal length. Tipically you should override this method.
       */
     virtual void setMaxDuration(double maxDuration);

     /**
       * Set the max duration of this signal and update for data, redefine in the inerithed class if you want change beaviour. (ie don't call an update).
       * Normally you shouldn't override this method
       */
     virtual void setSampleRate(double SR);//Set the SR of this signal

     //void setMinStartTimeAndMaxDuration(double t0, double maxDuration);//Set the min start time and the max duration of the signal.
     //void setMinStartTime(double t0);//Set the min start timethis signal.
     //void setMaxEndTime(double t1);//Set the max start time  of this signal.
     void setName(QString name);
     void setColor(QColor color);
     void setEnableCurve(bool enable);//Enable the curve in any eventual calculation (ie digest)
     void setShowCurve(bool enable);//Show/hide the curve but keep enabled in the calculation (ie digest)
     void nameUpdated();
     void updateData();
     void showBaseControl();
     void showExtendedControl();

     double * getTimeData()   {return m_t;}//return the pointer to internal data of the time signal. This should be a duplicate??
     double * getSignalData() {return m_s;}//return the pointer to internal data of the signal. This should be a duplicate??
protected:
     /**
       * The method is called every time and updateData is called. In this way extension class can implement it own calculation to provide the signal data.
       * The inerithed class implement this method with its own code to generate signal data but eventually different time data (TIME DATA MODIFICATION NEVER TESTED BEFORE!!)
       */
     virtual void recalc() {} //Reimplement this method to update data with the want function, rembert to delete and regenearte m_data and m_curve.
     void deleteAllData(); //Delete all referenced internal pointed data (m_t, m_y).
     void resetAllData(); //Reset all referenced internal pointed data (m_t, m_y).
     void createDataCurve();//Set

     /**
       * The method provide capability to the extended class to set modified time data
       */
     void setTimeData(double * t, long int len);

     /**
       * The method provide capability to the extended class to set modified signal data
       */
     void setSignalData(double * s, long int len);

     /**
       * This method is called when the extended class set itself widget to add more control
       */
     void setExtendedControl(QWidget * extendedWidget);// This method allows the extended class to add its own UI controllers

     /**
       * This method leaves the decision to update data after set the duration or not, useful in some inerithed class when you want do other stuff before update
       */
     void setMaxDurationAndUpdate(double  maxDuration, bool updateData);

     /**
       * This method leaves the decision to update data after set the SR or not, useful in some inerithed class when you want do other stuff before update
       */
     void setSampleRateAndUpdate(double SR, bool updateData);
private:
     QwtPlotCurve * m_curve;
     QwtCPointerData * m_data;
     QString m_name;
     double m_MaxDuration;//Duration, it's possible modify any of the parameter duration,t0,t1 to make modification to the length of the signal
     double m_Min_t0;//Start time to make calculation, The min value of time allowable constrained externally. This is NOT YET USED!! MAY BE BUGGED, it's always used as 0.0
     //double m_Max_t1;//End time to make calculation, The max value of time allowable constrained externally.
     double m_SR;//The SR
     double * m_t;//The pointer to array of base time
     double * m_s;//The pointer to array of the signal
     long int m_sample;//Number of sample of m_s and m_t
     bool m_curveEnabled;

     void initBaseControlWidget();//Create only the base control
     struct {
       QFrame * baseWidget;
       QLineEdit * lineName;
       ComboBoxWidgetColor * comboColor;
       QCheckBox * checkBoxEnableCurve;
       QCheckBox * checkBoxShowCurve;
       QPushButton * toggleButtonOptionControl;
     } m_baseControl; //This class control.

     struct {
        QFrame * extendedWidget;
        QPushButton * toggleButtonOptionControl;
     } m_extendedControl;//Extended class control, if a class extend this class can improve control

     void createBaseControl();//Create all the control, of the base class and of any eventual extended class. It llayouts all together.
     QFrame * m_allControl;//The widget with all control (base and extended)
};

#endif // GENERICTIMEDATA_H
