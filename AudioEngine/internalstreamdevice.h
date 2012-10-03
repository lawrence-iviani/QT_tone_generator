#ifndef INTERNALSTREAMDEVICE_H
#define INTERNALSTREAMDEVICE_H

#include <QIODevice>
#include <QDebug>
#include <QVBoxLayout>

#include <QAudioOutput>
#include <QAudioDeviceInfo>

#include "generator.h"



class InternalStreamDevice : public QIODevice
{
    Q_OBJECT
public:
    InternalStreamDevice(QObject *parent=0);
    InternalStreamDevice(QAudioFormat format, QObject *parent=0);
    ~InternalStreamDevice();

    bool open();
    QAudioFormat getAudioFormat() { return m_format;}


    virtual void close();
    virtual qint64 readData(char *data, qint64 maxlen);
    virtual qint64 writeData(const char *data, qint64 len);
    virtual qint64 bytesAvailable() const;
    virtual qint64 pos() const;
    virtual qint64 size() const;
    virtual bool seek(qint64 pos);

signals:
    void dataChanged();

public slots:
   void setData(qreal * data, qint64 len);

private:
    void generateData(qint64 durationUs, int frequency);

private:
    qint64 m_pos;
    QByteArray m_buffer;
    QAudioFormat     m_format;
};


#endif // INTERNALSTREAMDEVICE_H
