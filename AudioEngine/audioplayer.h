#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QWidget>

#include <QObject>
#include <QIODevice>
#include <QTimer>
#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include <QByteArray>
#include <QAudioOutput>
#include <QFile>

#include "internalstreamdevice.h"
#include "sndfile.hh"

const int AUDIOPLAYER_DEFAULT_DURATION_SEC = 5; //in seconds
const int AUDIOPLAYER_DEFAULT_TONE_FREQ = 1000;
const int AUDIOPLAYER_DEFAULT_SR = 44100;
const int AUDIOPLAYER_DEFAULT_BUFFER_LEN= 32768;
const int AUDIOPLAYER_DEFAULT_NCHANNELS=2;
const int AUDIOPLAYER_DEFAULT_SAMPLESIZE=16;
const QAudioFormat::SampleType AUDIOPLAYER_DEFAULT_SAMPLETYPE=QAudioFormat::SignedInt;
const QString AUDIOPLAYER_DEFAULT_CODEC="audio/pcm";
const QAudioFormat::Endian AUDIOPLAYER_DEFAULT_BYTEORDER=QAudioFormat::LittleEndian;
const int AUDIOPLAYER_NOTIFY_INTERVAL= 100;//ms
const int AUDIOPLAYER_PULL_INTERVAL=20; //ms
const qint64 AUDIOPLAYER_HEADER_WAV_SAMPLES=48; //number of sample of the WAV header.

class AudioPlayer : public QWidget
{
    Q_OBJECT
    Q_ENUMS(PlayMode)
    Q_ENUMS(SourceTest)
public:
    enum PlayMode {PUSH=0,PULL=1};
    enum SourceTest {STREAM=2,FILE=3};

    AudioPlayer(QWidget *parent = 0);
    ~AudioPlayer();

    //QAudioFormat to string
    static const QString audioFormatToString(QAudioFormat *format);
    static const QString audioStateToString(int state);
    static const QString audioSampleTypeToString(int sampleType);
    static const QString endianessFormatToString(QAudioFormat::Endian format);

    //libsndfile to QAudioFormat values
    static const QString decodeCodec(int format);
    const QAudioFormat::Endian decodeEndianess(int format);
    static const int decodePCMSampleSizeFormat(int format);
    static const QAudioFormat::SampleType decodePCMSignFormat(int format);

    //other func
    QAudioFormat getAudioFormat() {return m_audioOutput->format();}
    AudioPlayer::PlayMode playMode();
    bool pause();
    QAudioFormat readFileHeader (QString filename);
    QIODevice * stream();

    //Position function
    qint64 actualStreamSamplePosition();
    qint64 totalStreamSample();
    qint64 remainingStreamSample();


public slots:
    void setFileName(QString filename);
    void setPlayMode(AudioPlayer::PlayMode playMode);
    void setPause(bool pause);
    void setStart(bool start);
    void setStream(InternalStreamDevice *stream);
    bool setStreamSamplePosition(qint64 position);

signals:
    void streamPositionChanged(qint64 sample);

private:
    void initializeWidget();
    void initializeAudio(QAudioFormat format);
    void startPlayPull();
    void startPlayPush();
    void stopPlay();
    void startPlay();
    void pauseUI();
    void stopUI();
    void startUI();

    int m_durationSeconds;
    int m_toneFreq;
    int m_sr;
    int m_bufferLength;


   private:
    QTimer*          m_pullTimer;

    // Owned by layout
    QPushButton*     m_modeButton;
    QPushButton*     m_startStopButton;
    QPushButton*     m_suspendResumeButton;
    QPushButton*     m_sourceButton;
    QComboBox*       m_deviceBox;
    QSlider*         m_streamPosition;

    QAudioDeviceInfo m_device;
    InternalStreamDevice*       m_inputStream;
    QAudioOutput*    m_audioOutput;
    QIODevice*       m_output_IODev; // not owned
    int              m_source;
    QFile            m_inputFile;

    bool             m_pullMode;
    QByteArray       m_buffer;
    QAudio::State    m_previousState;


private slots:
    void notified();
    void pullTimerExpired();
    void toggleMode();
    void toggleSource();
    void toggleSuspendResume();
    void toggleStartStop();
    void stateChanged(QAudio::State state);
    void deviceChanged(int index);
    void setAudioFormat(QAudioFormat format);
    void convertSliderPositionToStreamSample(int position);
    void convertStreamSampleToSliderPosition(qint64 position);
};


#endif // AUDIOPLAYER_H
