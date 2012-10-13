#include "audioutils.h"

AudioUtils::AudioUtils(QObject *parent) :
    QObject(parent)
{
}

const QString AudioUtils::audioStateToString(int state) {
    QString retval="";
    switch (state) {
        case QAudio::ActiveState:
            retval="ActiveState";
            break;
        case QAudio::SuspendedState:
            retval="SuspendedState";
            break;
        case QAudio::StoppedState:
            retval="StoppedState";
            break;
        case QAudio::IdleState:
            retval="IdleState";
            break;
        default:
            retval="Unkwnon state";
        break;
    }
    return retval;
}

const QAudioFormat AudioUtils::readFileHeader (QString filename) {

    QAudioFormat retval;
    SndfileHandle file;

    file = SndfileHandle (filename.toLocal8Bit().data());
    qDebug() <<  "AudioUtils::readFileHeader hedaer of "<< filename << " format " <<  QString::number(file.format(),16) <<" SR=" << file.samplerate () <<" channels=" << file.channels ();

    retval.setCodec(AudioUtils::decodeCodec(file.format()));
    if (!retval.codec().compare("audio/pcm")) {
       qWarning() <<  "AudioUtils::readFileHeader unsupported type " <<  retval.codec();
    }
    retval.setChannelCount(file.channels());
    retval.setSampleRate(file.samplerate());
    retval.setByteOrder(AudioUtils::decodeEndianess(file.format()));
    retval.setSampleType(AudioUtils::decodePCMSignFormat(file.format()));
    retval.setSampleSize(AudioUtils::decodePCMSampleSizeFormat(file.format()));

    qDebug() <<  "AudioUtils::readFileHeader imported audio file is \n" << AudioUtils::audioFormatToString(&retval);
    return retval;
}

const QString AudioUtils::audioFormatToString(QAudioFormat *format) {
    QString retval;
    QTextStream text(&retval);
    text << "\tCHANNELS: " << format->channelCount() << "\n";
    text << "\tCODECS: " << format->codec()<< "\n";
    text << "\tSR: " << format->sampleRate()<< "\n";
    text << "\tSAMPLE SIZE: " << format->sampleSize ()<< "\n";
    text << "\tSAMPLE TYPE: " << AudioUtils::audioSampleTypeToString(format->sampleType())<< "\n";
    text << "\tENDIAN NESS: " << AudioUtils::endianessFormatToString(format->byteOrder()) << "\n";

    return  *text.string();
}

const QString AudioUtils::decodeCodec(int format) {
    QString retval;
    switch (format & SF_FORMAT_TYPEMASK ) {
        case SF_FORMAT_WAV:   /* Microsoft WAV format (little endian). */
            retval="audio/pcm";
            break;
        case SF_FORMAT_AIFF:  /* Apple/SGI AIFF format (big endian). */
            retval="audio/aiff";
            break;
        case SF_FORMAT_FLAC:  /* FLAC lossless file format */
            retval="audio/flac";
            break;
        default:
            retval="audio/unknown";
            break;
    }
    return retval;
}

const QAudioFormat::Endian AudioUtils::decodeEndianess(int format) {
    QAudioFormat::Endian retval=(QAudioFormat::Endian)QSysInfo::ByteOrder;
    switch (format & SF_FORMAT_ENDMASK ) {
        case SF_ENDIAN_LITTLE:
            retval=QAudioFormat::LittleEndian;
            break;
        case SF_ENDIAN_BIG:
            retval=QAudioFormat::BigEndian;
            break;
    }
    return retval;
}

const QString AudioUtils::endianessFormatToString(QAudioFormat::Endian format) {
    QString retval="";
    switch (format) {
        case QAudioFormat::LittleEndian:
            retval="Little Endian";
            break;
        case QAudioFormat::BigEndian:
            retval="Big Endian";
            break;
        default:
            retval="unknown";
    }
    return retval;
}

const QAudioFormat::SampleType AudioUtils::decodePCMSignFormat(int format) {
    QAudioFormat::SampleType retval;
    switch (format & SF_FORMAT_SUBMASK ) {
        case SF_FORMAT_PCM_16:/* Signed 16 bit data */
        case SF_FORMAT_PCM_24:/* Signed 24 bit data */
        case SF_FORMAT_PCM_32:/* Signed 32 bit data */
            retval=QAudioFormat::SignedInt;
            break;
        case SF_FORMAT_PCM_U8:/* Unsigned 8 bit data (WAV and RAW only) */
            retval=QAudioFormat::UnSignedInt;
            break;
        case SF_FORMAT_FLOAT:/* 32 bit float data */
            retval=QAudioFormat::Float;
            break;
        case SF_FORMAT_DOUBLE:/* 64 bit float data */
            retval=QAudioFormat::Float;//Is this correct?
            break;
        default:
            retval=QAudioFormat::Unknown;
            break;
    }
    return retval;
}

const int AudioUtils::decodePCMSampleSizeFormat(int format) {
    int retval;
    switch (format & SF_FORMAT_SUBMASK ) {
        case SF_FORMAT_PCM_S8:/* Signed 8 bit data */
            retval=8;
            break;
        case SF_FORMAT_PCM_16:/* Signed 16 bit data */
            retval=16;
            break;
        case SF_FORMAT_PCM_24:/* Signed 24 bit data */
            retval=24;
            break;
        case SF_FORMAT_PCM_32:/* Signed 32 bit data */
            retval=32;
            break;
        case SF_FORMAT_PCM_U8:/* Unsigned 8 bit data (WAV and RAW only) */
            retval=8;
            break;
        case SF_FORMAT_FLOAT:/* 32 bit float data */
            retval=32;
            break;
        case SF_FORMAT_DOUBLE:/* 64 bit float data */
            retval=64;
            break;
        default:
            retval=-1;
            break;
    }
    return retval;
}


const QString AudioUtils::audioSampleTypeToString(int sampleType) {
    QString retval="";
        switch (sampleType) {
            case QAudioFormat::Unknown:
                retval="Unknown";
                break;
            case QAudioFormat::SignedInt:
                retval="Signed int";
                break;
            case QAudioFormat::UnSignedInt:
                retval="Unsigned int";
                break;
            case QAudioFormat::Float:
                retval="Float";
                break;
            default:
                retval="Unknwon";
            break;

    }
    return retval;
}

QAudioFormat AudioUtils::getStandardFormat(AudioUtils::StandardFormat format) {
    QAudioFormat retval;

    retval.setSampleRate(AUDIOUTILS_DEFAULT_SR);
    retval.setSampleSize(AUDIOUTILS_DEFAULT_SAMPLESIZE);
    retval.setSampleType(AUDIOUTILS_DEFAULT_SAMPLETYPE);
    retval.setCodec(AUDIOUTILS_DEFAULT_CODEC);
    retval.setChannelCount(1);

    switch (format) {
        case AudioUtils::DAT:
            retval.setSampleRate(48000);
            break;
        case AudioUtils::CD:
            retval.setSampleRate(44100);
            break;
        default:
            qWarning() << "AudioUtils:: getStandardFormat unknown format";
            break;
    }

    return retval;
}
