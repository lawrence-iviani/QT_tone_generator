#ifndef AUDIOUTILS_H
#define AUDIOUTILS_H

#include <QObject>
#include "sndfile.hh"
#include <QAudioFormat>
#include <QAudio>
#include <QTextStream>
#include <QDebug>
#include <QtEndian>
#include <qmath.h>

const int AUDIOUTILS_DEFAULT_SR = 48000;
const int AUDIOUTILS_DEFAULT_NCHANNELS=2;
const int AUDIOUTILS_DEFAULT_SAMPLESIZE=16;
const QAudioFormat::SampleType AUDIOUTILS_DEFAULT_SAMPLETYPE=QAudioFormat::SignedInt;
const QString AUDIOUTILS_DEFAULT_CODEC="audio/pcm";
const QAudioFormat::Endian AUDIOUTILS_DEFAULT_BYTEORDER=(QAudioFormat::Endian) QSysInfo::ByteOrder;

typedef struct {
    qreal peak;
    qreal rms;
} AudioUtils_structMeter;

class AudioUtils : public QObject
{
    Q_OBJECT
    Q_ENUMS(StandardFormat)
public:
    enum StandardFormat {CD=0,DAT=1};
    explicit AudioUtils(QObject *parent = 0);
    
    //Generate Standard Format
    static QAudioFormat getStandardFormat(AudioUtils::StandardFormat format);
    //QAudioFormat to string
    static const QString audioFormatToString(QAudioFormat *format);
    static const QString audioStateToString(int state);
    static const QString audioSampleTypeToString(int sampleType);
    static const QString endianessFormatToString(QAudioFormat::Endian format);


    //libsndfile to QAudioFormat values
    static const QString decodeCodec(int format);
    static const QAudioFormat::Endian decodeEndianess(int format);
    static const int decodePCMSampleSizeFormat(int format);
    static const QAudioFormat::SampleType decodePCMSignFormat(int format);
    static const QAudioFormat readFileHeader (QString filename);

    //Convert stream value to a qreal peak between 0 & 1
    static const AudioUtils_structMeter getAudioPeak(const char *data, int length, QAudioFormat format );

signals:
    
public slots:
    
};

#endif // AUDIOUTILS_H
