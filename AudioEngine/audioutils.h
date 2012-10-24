#ifndef AUDIOUTILS_H
#define AUDIOUTILS_H

#include <QObject>
#include "sndfile.hh"
#include <QAudioFormat>
#include <QAudio>
#include <QTextStream>
#include <QDebug>

const int AUDIOUTILS_DEFAULT_SR = 48000;
const int AUDIOUTILS_DEFAULT_NCHANNELS=2;
const int AUDIOUTILS_DEFAULT_SAMPLESIZE=16;
const QAudioFormat::SampleType AUDIOUTILS_DEFAULT_SAMPLETYPE=QAudioFormat::SignedInt;
const QString AUDIOUTILS_DEFAULT_CODEC="audio/pcm";
const QAudioFormat::Endian AUDIOUTILS_DEFAULT_BYTEORDER=(QAudioFormat::Endian) QSysInfo::ByteOrder;

class AudioUtils : public QObject
{
    Q_OBJECT
    Q_ENUMS(StandardFormat)
public:
    enum StandardFormat {CD=0,DAT=1};
    explicit AudioUtils(QObject *parent = 0);
    
    //Generate Standard Format

    /**
      * Return a QAudioFormat based on the input format request
      * \param the enumerator AudioUtils::StandardFormat
      */
    static QAudioFormat getStandardFormat(AudioUtils::StandardFormat header);
    //QAudioFormat to string

    /**
      * Convert an audio format in a readable string
      */
    static const QString audioFormatToString(QAudioFormat *header);

    /**
      * Convert an audio device state in a readable string
      */
    static const QString audioStateToString(int state);

    /**
      * Convert an audio sample in a readable string
      */
    static const QString audioSampleTypeToString(int sampleType);

    /**
      * Convert the endianess flag in a readable string
      */
    static const QString endianessFormatToString(QAudioFormat::Endian header);


    //libsndfile to QAudioFormat values

    /**
      * Decode an header read with lib_soundfile in a readable string
      */
    static const QString decodeCodec(int header);

    /**
      * Decode an header read with lib_soundfile in a readable string
      */
    static const QAudioFormat::Endian decodeEndianess(int header);

    /**
      * Decode an header read with lib_soundfile in a readable string
      */
    static const int decodePCMSampleSizeFormat(int header);

    /**
      * Decode an header read with lib_soundfile in a readable string
      */
    static const QAudioFormat::SampleType decodePCMSignFormat(int header);

    /**
      * From a file try to understand ( if exista) the wav header.
      */
    static const QAudioFormat readFileHeader (QString filename);

signals:
    
public slots:
    
};

#endif // AUDIOUTILS_H
