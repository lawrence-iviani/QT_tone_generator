#ifndef GENERICTIMEDATA_H
#define GENERICTIMEDATA_H

#include <QObject>
#include <qwt_series_data.h>
#include <qwt_plot_curve.h>
#include <math.h>
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QCheckBox>
#include <scaledsliderwidget.h>
#include <comboboxwidgetcolor.h>

#define BASE_SIZE 10
#define CONTROL_WIDTH 200

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
    double sampleRateGeneration() {return m_SR;}
    double minStartTime(){return m_Min_t0;}
   // double maxEndTime(){return m_Max_t1;}
    long int sampleNumber() {return m_sample;}
    bool isEnabled() { return m_curve->isVisible();}
    QString name() {return  m_name;}
    QColor color() {return m_curve->pen().color(); }

    // setAndConvertFrequencyData(GenericFrequencyData * f); //Questo servira' a generare i dati partendo da una classe simile nel dominio frequenziale.
signals:
     void dataUpdated();
     void maxDurationUpdate(double maxDuration);
    // void minStartTimeUpdate(double t0);
    // void maxEndTimeUpdate(double t1);
public slots:
     void setMaxDuration(double maxDuration);//Set the max duration of this signal
     void setSampleRate(double SR);//Set the SR of this signal
     //void setMinStartTimeAndMaxDuration(double t0, double maxDuration);//Set the min start time and the max duration of the signal.
     //void setMinStartTime(double t0);//Set the min start timethis signal.
     //void setMaxEndTime(double t1);//Set the max start time  of this signal.
     void setName(QString name);
     void setColor(QColor color);
     void setEnableCurve(bool enable);
     void nameUpdated();
     void updateData();
     void showBaseControl();
     void showExtendedControl();

protected:
     virtual void recalc() {} //Reimplement this method to update data with the want function, rembert to delete and regenearte m_data and m_curve.
     void deleteAllData(); //Delete all referenced internal pointed data (m_t, m_y).
     void resetAllData(); //Reset all referenced internal pointed data (m_t, m_y).

     void createDataCurve();//Set
     double * getTimeData() {return m_t;}
     double * getSignalData() {return m_s;}
     void setTimeData(double * t, long int len);
     void setSignalData(double * s, long int len);
     void setExtendedControl(QWidget * extendedWidget);// This method allows the extended class to add its own UI controllers
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

     double static const MIN_TIME=0.0;
     double static const MAX_TIME=5.0;

     void initBaseControlWidget();//Create only the base control
     struct {
       QWidget * baseWidget;
       QLineEdit * lineName;
       ComboBoxWidgetColor * comboColor;
       QCheckBox * checkBoxEnableCurve;
       QPushButton * toggleButtonOptionControl;
     } m_baseControl; //This class control.

     struct {
        QWidget * extendedWidget;
        QPushButton * toggleButtonOptionControl;
     } m_extendedControl;//Extended class control, if a class extend this class can improve control

     void createBaseControl();//Create all the control, of the base class and of any eventual extended class. It llayouts all together.
     QWidget * m_allControl;//The widget with all control (base and extended)
};

#endif // GENERICTIMEDATA_H
