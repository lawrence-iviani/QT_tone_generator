#ifndef REPEATEDTIMEDATA_H
#define REPEATEDTIMEDATA_H

#include <timedata/partialtimedata.h>
#include <timedata/repeatedtimedataui.h>
#include <QDebug>

class RepeatedTimeDataUI;

class RepeatedTimeData : public PartialTimeData
{
    Q_OBJECT
    Q_PROPERTY(qreal blanktime READ blankTime WRITE setBlankTime)
public:
    explicit RepeatedTimeData(QWidget * widget=0);
    explicit RepeatedTimeData(qreal maxDuration, qreal SRGen, QWidget *widget=0);
    

    /**
     * @brief blankTime return the blank time between curve repetitions
     * @return the blank time between repetitions
     */
    qreal blankTime() {return m_blankTime;}

    /**
     * @brief insertSignalValue This function insert a qreal value in the sample position index and return true if the insertion is succesful, if return false the index is out of range.
     *  This overloading allows to insert the one shot curve in multiple (repeated) position.
     *@param index the sample index where the value must be inserted
     * @param value the value to insert
     * @return True if the insert was succesful.
     */
    virtual bool insertSignalValue(qint64 index, qreal value);
signals:
    
public slots:
    /**
     * @brief setBlankTime set the blank time between curve repetitions
     * @param blanktime the blank time between repetitions to set
     */
    void setBlankTime(qreal blanktime);

private slots:
    /**
     * @brief updateRepetitions recalculate the number of repetitions
     */
    void updateRepetitions();
private:
    void connectSignal();

    RepeatedTimeDataUI *m_repeatedTimeDataUI;
    static qreal const TIMEDATA_DEFAULT_BLANK=1.0;
    qreal  m_blankTime;
    unsigned int m_repetitions;
};

#endif // REPEATEDTIMEDATA_H
