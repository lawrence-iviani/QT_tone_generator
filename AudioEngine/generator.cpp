#include "generator.h"


Generator::Generator(quint64 durationUs, int frequency, int SR,QObject *parent) :
    QObject(parent)
{
    m_data=NULL;
    m_nSample=0;
    generateData(durationUs, frequency, SR);
}

Generator::Generator(quint64 durationUs, int frequency, int SR,int nloop, QObject *parent = 0) :
    QObject(parent)
{
    m_data=NULL;
    m_nSample=0;
    generateData(durationUs, frequency, SR,nloop);
}

qreal * Generator::generateData(quint64 durationUs, int frequency, int SR,int loop)
{
    quint64 lengthLoop = SR * (1.0/loop)*durationUs / 1000000;
    m_nSample = lengthLoop*loop;

    if (m_data) {
        qWarning() << "Generator::generateData deleting data @ "<<m_data;
        free(m_data);
    }
    m_data =(qreal*) malloc(m_nSample*sizeof(qreal));
    memset (m_data,0,m_nSample);

    quint64 i;
    quint64 t_len=0;

    while (t_len < m_nSample) {
        for (i=t_len; i< t_len+lengthLoop ;i++)
            m_data[i]=0.25*sin(2 * M_PI * frequency * i / SR);
        t_len +=2*lengthLoop;
    }
    qWarning() << "Generator::generateData allocated bytes length=" << m_nSample << " @"<<m_data;

    return m_data;
}
