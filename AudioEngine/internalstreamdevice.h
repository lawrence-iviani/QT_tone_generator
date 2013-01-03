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

signals:
    void dataChanged();

public slots:
   bool setAudioData(qreal * data, qint64 len);
   bool setAudioData(qreal * data, qint64 len, QAudioFormat format);
   void setAudioFormat(QAudioFormat format);

private:
    void generateData(qint64 durationUs, int frequency);

private:
    //qint64 m_pos;
    QByteArray * m_buffer;
    QAudioFormat     m_format;
};


#endif // INTERNALSTREAMDEVICE_H
