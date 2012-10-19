#ifndef INTERNALSTREAMDEVICE_H
#define INTERNALSTREAMDEVICE_H

#include <QBuffer>
#include <QDebug>
#include <QVBoxLayout>

#include <QAudioOutput>
#include <QAudioDeviceInfo>

#include "generator.h"

/**
  * This class is an estension of the QBuffer class that allows to insert in the internal buffer a formatted audio data.
  * All the functions are inerithed from the IODevice, thus the class can be used as a stream.
  */
class InternalStreamDevice : public QBuffer
{
    Q_OBJECT
public:
    InternalStreamDevice(QObject *parent=0);
    InternalStreamDevice(QAudioFormat format, QObject *parent=0);
    ~InternalStreamDevice();

    /**
      * Return the audio format associated to this stream
      */
    QAudioFormat getAudioFormat() { return m_format;}

    /**
      * Return the actual position in sample
      */
    const qint64 getAudioBufferPos();

    /**
      * Return the length in byte of a single sample
      */
    const int getSampleLength();

    /**
      * Return the total length of the stream in sample
      */
    const qint64 getAudioBufferLength();

signals:
    /**
      * This signal is emitted whenever a new Audio data set is inserted.
      */
    void dataChanged();

public slots:
    /**
      * set the outside data inside the internal buffer
      * \param data pointer to the buffer the data that needs to be handled
      * \param len the length of the data needs to be handled
      */
   bool setAudioData(qreal * data, qint64 len);


private:
    //qint64 m_pos;
    QByteArray * m_buffer;
    QAudioFormat     m_format;
};


#endif // INTERNALSTREAMDEVICE_H
