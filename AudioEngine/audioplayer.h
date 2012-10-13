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
#include "audioutils.h"

//TODO:
//1. Handle float stream
//2. Optimize allocation in InternalStreamDevice class
//3. Fix problem with Unsigned int 16 bit
//4. Noise in playback
//5. Seeking on start is not working

const int AUDIOPLAYER_DEFAULT_DURATION_SEC = 5; //in seconds
const int AUDIOPLAYER_DEFAULT_TONE_FREQ = 1000;
const int AUDIOPLAYER_DEFAULT_BUFFER_LEN= 32768;
const int AUDIOPLAYER_NOTIFY_INTERVAL= 100;//ms
const int AUDIOPLAYER_PULL_INTERVAL=20; //ms
const qint64 AUDIOPLAYER_HEADER_WAV_SAMPLES=48; //number of sample of the WAV header.

class AudioPlayer : public QObject
{
    Q_OBJECT
    Q_ENUMS(PlayMode)
    Q_ENUMS(SourceTest)
public:
    enum PlayMode {PUSH=0,PULL=1};
    enum SourceTest {STREAM=2,FILE=3};

    AudioPlayer(QObject *parent = 0);
    AudioPlayer(QWidget * parentWidget, QObject *parent = 0);
    ~AudioPlayer();



    //other func
    QAudioFormat getAudioFormat() {return m_audioOutput->format();}
    AudioPlayer::PlayMode playMode();
    bool pause();

    QIODevice * stream();

    //Position function
    qint64 actualStreamSamplePosition();
    qint64 totalStreamSample();
    qint64 remainingStreamSample();

    //Widget control function
    QWidget * getTestControlWidget()  {return  m_testControlWidget; }
    QWidget * getAudioControlWidget() {return  m_audioControlWidget; }
    QWidget * getAudioOptionWidget()  {return  m_audioOptionWidget; }

public slots:
    void setFileName(QString filename);
    void setPlayMode(AudioPlayer::PlayMode playMode);
    void setPause(bool pause);
    void setStart(bool start,int position=0);
    void setStream(InternalStreamDevice *stream);
    bool setStreamSamplePosition(qint64 position);

signals:
    void streamPositionChanged(qint64 sample);

private:
    void initClass();
    void initializeWidget(QWidget *parentWidget=0);
    void initializeAudio(QAudioFormat format);
    void startPlayPull(int position);
    void startPlayPush(int position);
    void stopPlay();
    void startPlay();
    void startPlay(int position);
    void pauseUI();
    void stopUI();
    void startUI();
    qint64 convertSliderPositionToStreamSample(int position);
    int convertStreamSampleToSliderPosition(qint64 position);

    int m_durationSeconds;
    int m_toneFreq;
    int m_sr;
    int m_bufferLength;

    QTimer*          m_pullTimer;

    // Owned by layout
    QPushButton*     m_modeButton;
    QPushButton*     m_startStopButton;
    QPushButton*     m_suspendResumeButton;
    QPushButton*     m_sourceButton;
    QComboBox*       m_deviceBox;
    QSlider*         m_streamPositionSlider;

    QAudioDeviceInfo m_device;
    InternalStreamDevice*       m_inputStream;
    QAudioOutput*    m_audioOutput;
    QIODevice*       m_output_IODev; // not owned
    int              m_source;
    QFile            m_inputFile;

    bool             m_pullMode;
    QByteArray       m_buffer;
    QAudio::State    m_previousState;

    //create widget for various control...
    QWidget *  m_testControlWidget;
    QWidget *  m_audioControlWidget;
    QWidget *  m_audioOptionWidget;

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
    void convertAndSetSliderPositionToStreamSample(int position);
    void convertAndSetStreamSampleToSliderPosition(qint64 position);
};


#endif // AUDIOPLAYER_H
