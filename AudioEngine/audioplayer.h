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
#include <QSizePolicy>
#include <QTime>
#include "qvumeter.h"

#include "internalstreamdevice.h"
#include "audioutils.h"

//TODO:
//1. Handle float stream
//2. Optimize allocation in InternalStreamDevice class
//3. Fix problem with Unsigned int 16 bit
//4. Noise in playback
//5. Seeking on start is not working

const int AUDIOPLAYER_DEFAULT_DURATION_SEC = 5; //in seconds
const int AUDIOPLAYER_DEFAULT_TONE_FREQ= 1000;
const int AUDIOPLAYER_DEFAULT_BUFFER_LEN= 32768;
const int AUDIOPLAYER_NOTIFY_INTERVAL= 50;//ms
const int AUDIOPLAYER_DEFAULT_PULL_INTERVAL=10; //ms
const qint64 AUDIOPLAYER_HEADER_WAV_SAMPLES=48; //number of sample of the WAV header.

/**
  * This class provide a programmable interface in order to play file and internal streams (see the class Internal Streams).
  * It's strong derived from the QT example.
  * It provides some reusable widget for fast integration into other sw (at cost of flexibility). But at this stage of the development i'm not looking for the perfet UI but only usability.
  * It's possible:
  * 1. Select the audio interface on the device
  * 2. Select the mode (push or pull) See REFERENCE TO THE QT AUDIO EXAMPLE
  * 3. Handle play/stop pause/resume and  position
  * 4.
  */

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
    /**
      * Return the audio format actually in use
      */
    QAudioFormat getAudioFormat() {return m_audioOutput->format();}

    /**
      * Return the play mode techinques actually in use format actually in use
      */
    AudioPlayer::PlayMode playMode();

    /**
      * Return the pause status.
      */
    bool pause();

    /**
      * Return the stream pointer actually in use
      */
    QIODevice * stream();

    //Position function
    /**
      * Return the actual stream index position
      */
    qint64 actualStreamSamplePosition();

    /**
      * Return the total number of index in the actual stream
      */
    qint64 actualStreamTotalSample();

    /**
      * Return the remaining number of sample of the actual stream
      */
    qint64 actualStreamRemainingSample();

    /**
      * Return the actual time position in sec.
      */
    qreal actualStreamTimePosition();

    /**
      * Return the total time of the actual stream in sec.
      */
    qreal actualStreamTotalTime();

    /**
      * Return the remaining time of the actual stream in sec.
      */
    qreal actualStreamRemainingTime();

    //Widget control function
    QWidget * getTestControlWidget()  {return  m_testControlWidget; }
    QWidget * getAudioControlWidget() {return  m_audioControlWidget; }
    QWidget * getAudioOptionWidget()  {return  m_audioOptionWidget; }

public slots:
    void setFileName(QString filename);
    void setPlayMode(AudioPlayer::PlayMode playMode);
    void setPause(bool pause);
    void setStart(bool start,int samplePosition=0);
    void setStream(InternalStreamDevice *stream);
    void setAudioFormat(QAudioFormat format);

    /**
      * This function is an abstarction to set a generic sample stream in the correct bytes position of the stream
      * Transform a generic index in byte index keeping in mind the sample len, the number of channels
      * \param samplePosition the index of the sample wants set.
      */
    bool setStreamSamplePosition(qint64 samplePosition);

signals:
    /**
      * This is emitted when the position of the stream is changed. It's emitted by the notified function with an  interval of AUDIOPLAYER_NOTIFY_INTERVAL
      * \param samplePosition the index of the new sample position
      */
    void streamSamplePositionChanged(qint64 samplePosition);

    /**
      * This is emitted when the position of the stream is changed. It's emitted by the notified function with an  interval of AUDIOPLAYER_NOTIFY_INTERVAL
      * \param timePosition the index of the new position in seconds
      */
    void streamTimePositionChanged(qreal timePosition);

private:
    void initClass();
    void initializeWidget(QWidget *parentWidget=0);

    /**
      * This function init the output device with the wanted device.
      * 1. Remove any previous output device.
      * 2. If the format is not avialble try to use the nearest format.
      * \param format the QAudioFormat you want init this output device
      */
    void initializeAudio(QAudioFormat format);

    /**
      * Start to play in pull mode at sample index.
      * \param samplePosition the index where to start
      */
    void startPlayPull(qint64 samplePosition);

    /**
      * Start to play in push mode at sample index.
      * \param samplePosition the index where to start
      */
    void startPlayPush(qint64 samplePosition);

    /**
      * Stop to play .any streams.
      */
    void stopPlay();

    /**
      * Start to play .the selected stream from the start
      */
    void startPlay();

    /**
      * Start to play .the selected stream from the samplePosition
      * \param samplePosition the index where to start
      */
    void startPlay(qint64 samplePosition);

    /**
      * Set the UI in pause position
      */
    void pauseUI();

    /**
      * Set the UI in stop position
      */
    void stopUI();

    /**
      * Set the UI in start position
      */
    void startUI();

    /**
      * Utility to convert the internal slider position to a valid index sample
      * \param sliderPosition the index of the slide
      */
    qint64 convertSliderPositionToStreamSample(int sliderPosition);

    /**
      * Utility to convert the stream position to the slider position
      * \param samplePosition the index of the stream that has to be converted.
      */
    int convertStreamSampleToSliderPosition(qint64 samplePosition);

    int m_bufferLength;

    int m_pullInterval;

    QTimer*          m_pullTimer;

    // Owned by layout
    QPushButton*     m_modeButton;
    QPushButton*     m_startStopButton;
    QPushButton*     m_suspendResumeButton;
    QPushButton*     m_sourceButton;
    QComboBox*       m_deviceBox;
    QSlider*         m_streamPositionSlider;
    QVUMeter*        m_qvumeter;

    QAudioDeviceInfo m_device;
    InternalStreamDevice*       m_inputStream;
    QAudioOutput*    m_audioOutput;
    QIODevice*       m_output_IODev; // not owned
    int              m_source;
    QFile            m_inputFile;

    bool             m_pullMode;
    QByteArray       m_buffer;
    QAudio::State    m_previousState;
    qint64           m_previousPosition;

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
    void sliderToggled();
    void convertAndSetSliderPositionToStreamSample(int sliderPosition);
    void convertAndSetStreamSampleToSliderPosition(qint64 samplePosition);
};


#endif // AUDIOPLAYER_H
