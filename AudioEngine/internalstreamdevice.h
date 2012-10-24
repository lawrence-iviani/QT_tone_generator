#ifndef INTERNALSTREAMDEVICE_H
#define INTERNALSTREAMDEVICE_H

#include <QBuffer>
#include <QDebug>
#include <QVBoxLayout>

#include <QAudioOutput>
#include <QAudioDeviceInfo>

#include "generator.h"


class InternalStreamDevice : public QBuffer
{
    Q_OBJECT
public:
    InternalStreamDevice(QObject *parent=0);
    InternalStreamDevice(QAudioFormat format, QObject *parent=0);
    ~InternalStreamDevice();

    QAudioFormat getAudioFormat() { return m_format;}
    const int getSampleLength();
    const qint64 getStreamLength();

signals:
    void dataChanged();

public slots:
    /**
      * set the outside data inside the internal buffer
      * \param data pointer to the buffer the data that needs to be handled
      * \param len the length of the data needs to be handled
      */
   bool setAudioData(qreal * data, qint64 len);

private:
    void generateData(qint64 durationUs, int frequency);

private:
    //qint64 m_pos;
    QByteArray * m_buffer;
    QAudioFormat     m_format;
};


#endif // INTERNALSTREAMDEVICE_H
