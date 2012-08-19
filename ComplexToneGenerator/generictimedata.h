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
    GenericTimeData(double t0, double duration, double SRGen);
    virtual ~GenericTimeData();
    QwtPlotCurve * getCurve() {return m_curve;}
    QwtCPointerData * getData() {return m_data;}
    QWidget * getControlWidget() {return m_allControl; }// Give back a QWidget that can be used to control this class.

    double duration(){return m_duration;}
    double sampleRateGeneration() {return m_SR;}
    double startTime(){return m_t0;}
    double endTime(){return m_t1;}
    long int sampleNumber() {return m_sample;}
    bool isEnabled() { return m_curve->isVisible();}
    QString name() {return  m_name;}
    QColor color() {return m_curve->pen().color(); }

    // setAndConvertFrequencyData(GenericFrequencyData * f); //Questo servira' a generare i dati partendo da una classe simile nel dominio frequenziale.
signals:
     void dataUpdated();
public slots:
     void setDuration(double duration);
     void setSampleRate(double SR);
     void setStartTimeAndDuration(double t0, double duration);
     void setStartTime(double t0);
     void setEndTime(double t1);
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
     double m_duration;//Duration, it's possible modify any of the parameter duration,t0,t1 to make modification to the length of the signal
     double m_t0;//Start time to make calculation
     double m_t1;//End time to make calculation
     double m_SR;
     double * m_t;//The pointer to array of base time
     double * m_s;//The pointer to array of the signal
     long int m_sample;//Number of sample of m_s and m_t

     void initBaseControlWidget();//Create only the base control
     struct {
       QWidget * baseWidget;
       QLineEdit * lineName;
       ScaledSliderWidget * slider_SR;
       ScaledSliderWidget * slider_duration;
       ScaledSliderWidget * slider_t0;
       ComboBoxWidgetColor * comboColor;
       QCheckBox * checkBoxEnableCurve;
       QPushButton * toggleButtonOptionEnable;
     } m_baseControl; //This class control.

     struct {
        QWidget * extendedWidget;
        QPushButton * toggleButtonOptionEnable;
     } m_extendedControl;//Extended class control, if a class extend this class can improve control

     void createBaseControl();//Create all the control, of the base class and of any eventual extended class. It llayouts all together.
     QWidget * m_allControl;//The widget with all control (base and extended)
};

#endif // GENERICTIMEDATA_H
