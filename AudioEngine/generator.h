#ifndef GENERATOR_H
#define GENERATOR_H


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <QObject>
#include <QDebug>
#include <math.h>

class Generator : public QObject
{
    Q_OBJECT
public:
    Generator(quint64 durationUs, int frequency, int SR,QObject *parent );
    Generator(quint64 durationUs, int frequency, int SR,int nloop, QObject *parent );

    qreal * generateData(quint64 durationUs, int frequency, int SR, int loop=100);
    qreal * getData() {return m_data;}
    quint64 getNumberOfSample() {return m_nSample;}
signals:
    
public slots:
    
private:
    qreal * m_data;
    quint64 m_nSample;
};

#endif // GENERATOR_H
