#include "audioplayer.h"


AudioPlayer::AudioPlayer(QWidget *parent)
    :   QWidget(parent)
    ,   m_pullTimer(new QTimer(this))
    ,   m_modeButton(0)
    ,   m_suspendResumeButton(0)
    ,   m_deviceBox(0)
    ,   m_device(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_audioOutput(0)
    ,   m_output_IODev(0)
{
    m_durationSeconds=AUDIOPLAYER_DEFAULT_DURATION_SEC;
    m_toneFreq=AUDIOPLAYER_DEFAULT_TONE_FREQ;
    m_bufferLength=AUDIOPLAYER_DEFAULT_BUFFER_LEN;

    QAudioFormat format;
    format.setFrequency(AUDIOPLAYER_DEFAULT_SR);
    format.setChannels(AUDIOPLAYER_DEFAULT_NCHANNELS);
    format.setSampleSize(AUDIOPLAYER_DEFAULT_SAMPLESIZE);
    format.setSampleType(AUDIOPLAYER_DEFAULT_SAMPLETYPE);
    format.setCodec(AUDIOPLAYER_DEFAULT_CODEC);
    format.setByteOrder(AUDIOPLAYER_DEFAULT_BYTEORDER);

    m_buffer=QByteArray(m_bufferLength,0);

    m_pullMode = true;
    m_source=AudioPlayer::STREAM;
    connect(m_pullTimer, SIGNAL(timeout()), SLOT(pullTimerExpired()));


    initializeWidget();
    initializeAudio(format);
    m_inputStream=NULL;
    m_previousState=QAudio::IdleState;
    m_audioOutput->stop();

}

void AudioPlayer::initializeWidget()
{
    //QScopedPointer<QWidget> window(new QWidget);
    QVBoxLayout * layout=new QVBoxLayout();

    m_deviceBox = new QComboBox(this);
    foreach (const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
        m_deviceBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
    connect(m_deviceBox,SIGNAL(activated(int)),SLOT(deviceChanged(int)));
    layout->addWidget(m_deviceBox);

    m_modeButton = new QPushButton(this);
    m_modeButton->setText("Enable push mode");
    connect(m_modeButton, SIGNAL(clicked()), SLOT(toggleMode()));
    layout->addWidget(m_modeButton);

    m_startStopButton = new QPushButton(this);
    m_startStopButton->setText("Start audio");
    connect(m_startStopButton, SIGNAL(clicked()), SLOT(toggleStartStop()));
    layout->addWidget(m_startStopButton);

    m_suspendResumeButton = new QPushButton(this);
    m_suspendResumeButton->setText("Suspend playback");
    connect(m_suspendResumeButton, SIGNAL(clicked()), SLOT(toggleSuspendResume()));
    layout->addWidget(m_suspendResumeButton);

    m_sourceButton = new QPushButton(this);
    m_sourceButton->setText("Play file");
    connect(m_sourceButton, SIGNAL(clicked()), SLOT(toggleSource()));
    layout->addWidget(m_sourceButton);

    m_streamPosition = new QSlider(Qt::Horizontal,this);
    m_streamPosition->setTickPosition(QSlider::TicksBothSides);
    connect(m_streamPosition,SIGNAL(valueChanged(int)),this,SLOT(convertSliderPositionToStreamSample(int)));
    connect(this,SIGNAL(streamPositionChanged(qint64)),this,SLOT(convertStreamSampleToSliderPosition(qint64)));
    m_streamPosition->setMaximum(200);
    layout->addWidget(m_streamPosition);

    this->setLayout(layout);
    this->stopUI();
}

void AudioPlayer::initializeAudio(QAudioFormat format)
{
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported - trying to use nearest";
        format = info.nearestFormat(format);
    }

    if (m_audioOutput) {
        disconnect(m_audioOutput, SIGNAL(notify()), this,SLOT(notified()));
        disconnect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(stateChanged(QAudio::State)));
        delete m_audioOutput;
    }
    m_audioOutput = 0;
    m_audioOutput = new QAudioOutput(m_device, format, this);
    connect(m_audioOutput, SIGNAL(notify()), SLOT(notified()));
    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), SLOT(stateChanged(QAudio::State)));
    m_audioOutput->reset();
    m_audioOutput->setNotifyInterval(AUDIOPLAYER_NOTIFY_INTERVAL);
}

void AudioPlayer::setPlayMode(AudioPlayer::PlayMode playMode) {
    stopPlay();
    switch (playMode) {
        case AudioPlayer::PULL:
            qWarning() << "Setting push mode";
            m_modeButton->setText("Enable pull mode");
            m_pullMode = false;
            break;
        case AudioPlayer::PUSH:
            qWarning() << "Setting pull mode";
            m_modeButton->setText("Enable push mode");
            m_pullMode = true;
            break;
    }
    startPlay();
    m_suspendResumeButton->setText("Suspend playback");
}

void AudioPlayer::setStream(InternalStreamDevice * stream){
        this->setPause(true);
    if (m_inputStream && m_inputStream->isOpen()) {
        m_inputStream->close();
    }
    m_inputStream=stream;
}

void AudioPlayer::setAudioFormat(QAudioFormat format) {
    if (format!=m_audioOutput->format()) {
        qWarning() << "AudioPlayer::setAudioFormat changing file format";
        m_audioOutput->format()=format;
        initializeAudio(format);
    }
}

void AudioPlayer::setFileName(QString filename) {
    this->setPause(true);
    if (m_inputFile.isOpen()) {
        m_inputFile.close();
    }
    m_inputFile.setFileName(filename);
    qWarning() << "AudioPlayer::setFileName set file name " << m_inputFile.fileName();
}

void AudioPlayer::startPlayPush() {
    m_pullMode=false;
    switch (m_source) {
        case AudioPlayer::STREAM:
            qWarning() << "AudioPlayer::startPlayPush starting  with STREAM";
            m_inputStream->open();
            if (m_inputStream && m_inputStream->open()) {
                this->setAudioFormat(m_inputStream->getAudioFormat());//Change format only if needed
                m_audioOutput->start(m_inputStream);
                qWarning() << "AudioPlayer::startPlayPush started  with SR=" << m_audioOutput->format().sampleRate();
            } else {
                qWarning() << "AudioPlayer::startPlayPush nothing to play";
                this->setPause(true);
            }
            break;
        case AudioPlayer::FILE:
            qWarning() << "AudioPlayer::startPlayPush starting  with FILE";
            if (m_inputFile.open(QIODevice::ReadOnly)){
                this->setAudioFormat(this->readFileHeader(m_inputFile.fileName()));
                m_inputFile.seek(AUDIOPLAYER_HEADER_WAV_SAMPLES+4);
                m_audioOutput->start(&m_inputFile);
                qWarning() << "AudioPlayer::startPlayPush started  with SR=" << m_audioOutput->format().sampleRate();
            } else {
                qWarning() << "AudioPlayer::startPlayPush can't open "<< m_inputFile.fileName();
            }
            break;
    }
}

void AudioPlayer::startPlay() {
    stopPlay();
    if (m_pullMode) {
        startPlayPull();
    } else {
        startPlayPush();
    }
}

void AudioPlayer::startPlayPull() {
    m_pullMode=true;
    switch (m_source) {
        case AudioPlayer::STREAM:
            qWarning() << "AudioPlayer::startPlayPull starting  with STREAM";
            if (m_inputStream && m_inputStream->open()) {
                this->setAudioFormat(m_inputStream->getAudioFormat());//Change format only if needed
                m_output_IODev = m_audioOutput->start();
                m_pullTimer->start(AUDIOPLAYER_PULL_INTERVAL);
                qWarning() << "AudioPlayer::startPlayPull started  with SR=" << m_audioOutput->format().sampleRate();
            } else {
                qWarning() << "AudioPlayer::startPlayPull nothing to play, can't open stream";
                this->setPause(true);
            }
            break;
        case AudioPlayer::FILE:
            qWarning() << "AudioPlayer::startPlayPull starting  with FILE";
            if (m_inputFile.open(QIODevice::ReadOnly)){
                this->setAudioFormat(this->readFileHeader(m_inputFile.fileName()));
                m_inputFile.seek(AUDIOPLAYER_HEADER_WAV_SAMPLES+4);
                m_output_IODev = m_audioOutput->start();
                m_pullTimer->start(AUDIOPLAYER_PULL_INTERVAL);
                qWarning() << "AudioPlayer::startPlayPull started  with SR=" << m_audioOutput->format().sampleRate();
            } else {
                qWarning() << "AudioPlayer::startPlayPull can't open "<< m_inputFile.fileName();
            }
            break;
    }
}

void AudioPlayer::stopPlay() {

    m_pullTimer->stop();
    m_audioOutput->stop();
    m_audioOutput->reset();
   // m_audioOutput->disconnect();

    switch (m_source) {
        case AudioPlayer::STREAM:
            if (m_inputStream && m_inputStream->isOpen()) {
                m_inputStream->close();
                qWarning() << "AudioPlayer::stopPlay called close for Internal Generator";
            }
            break;
        case AudioPlayer::FILE:
            if (m_inputFile.isOpen()) {
                m_inputFile.close();
                qWarning() << "AudioPlayer::stopPlay called close for "<< m_inputFile.fileName();
            }
            break;
    }
    //Need to force UI.. why?
    this->stopUI();
}

bool AudioPlayer::setStreamSamplePosition(qint64 position) {
    bool retval=false;
    qint64 bytesPosition=0;
    if (m_audioOutput && (m_audioOutput->state() == QAudio::ActiveState || m_audioOutput->state() == QAudio::SuspendedState)) {
        switch (m_source) {
            case AudioPlayer::STREAM:
                if (m_inputStream && m_inputStream->isOpen()) {
                    bytesPosition=qMin(m_audioOutput->format().channels()*m_audioOutput->format().sampleSize()*position,m_inputStream->size() );
                    retval=m_inputStream->seek(bytesPosition);
                    if (retval)
                        qWarning() << "AudioPlayer::setBytesPosition set STREAM position@" << bytesPosition << "/"<< m_inputStream->size() << " bytes";
                    else
                        qWarning() << "AudioPlayer::setBytesPosition CAN'T' set STREAM position@" << bytesPosition << "/"<< m_inputStream->size() << " bytes";
                }
                break;
            case AudioPlayer::FILE:
                if (m_inputFile.isOpen()) {
                    bytesPosition=qMin(m_audioOutput->format().channels()*m_audioOutput->format().sampleSize()*position,m_inputFile.size() );
                    retval=m_inputFile.seek(bytesPosition+AUDIOPLAYER_HEADER_WAV_SAMPLES); //TODO: + header!
                    if (retval)
                            qWarning() << "AudioPlayer::setBytesPosition set FILE position@" << bytesPosition << "/"<< m_inputFile.size() << " bytes";
                        else
                            qWarning() << "AudioPlayer::setBytesPosition CAN'T' set FILE position@" << bytesPosition << "/"<< m_inputFile.size() << " bytes";
                }
                break;
        }
    } else {
         qWarning() << "AudioPlayer::setBytesPosition: DO NOTHING";
    }
    return retval;
}

qint64 AudioPlayer::actualStreamSamplePosition() {
    qint64 retval=0;
    switch (m_source) {
        case AudioPlayer::STREAM:
            if (m_inputStream && m_inputStream->isOpen()) {
                retval=m_inputStream->pos();
            }
            break;
        case AudioPlayer::FILE:
            if (m_inputFile.isOpen()) {
                retval=m_inputFile.pos();
            }
            break;
    }
    return retval/(m_audioOutput->format().channels()*m_audioOutput->format().sampleSize());
}

qint64 AudioPlayer::totalStreamSample() {
    qint64 retval=0;
    switch (m_source) {
        case AudioPlayer::STREAM:
            if (m_inputStream && m_inputStream->isOpen()) {
                retval=m_inputStream->size();
            }
            break;
        case AudioPlayer::FILE:
            if (m_inputFile.isOpen()) {
                retval=m_inputFile.size();
            }
            break;
    }
    return retval/(m_audioOutput->format().channels()*m_audioOutput->format().sampleSize());
}

qint64 AudioPlayer::remainingStreamSample() {
    qint64 retval=0;
    switch (m_source) {
        case AudioPlayer::STREAM:
            if (m_inputStream && m_inputStream->isOpen()) {
                retval=m_inputStream->bytesAvailable();
            }
            break;
        case AudioPlayer::FILE:
            if (m_inputFile.isOpen()) {
                retval=m_inputFile.bytesAvailable();
            }
            break;
    }
    return retval/(m_audioOutput->format().channels()*m_audioOutput->format().sampleSize());
}

void AudioPlayer::convertStreamSampleToSliderPosition(qint64 position) {
    if (!m_streamPosition->isSliderDown()) {
        int sliderPosition=(int) (m_streamPosition->maximum()* ((qreal)position/(qreal)this->totalStreamSample()));
        //qWarning() << "AudioPlayer::convertStreamSampleToSliderPosition sliderPosition=" << sliderPosition;
        bool sigStatus=m_streamPosition->blockSignals(true);
        m_streamPosition->setValue(sliderPosition);
        m_streamPosition->blockSignals(sigStatus);
    }
}

void AudioPlayer::convertSliderPositionToStreamSample(int position) {
    if (!m_streamPosition->isSliderDown()) {
        qint64 streamPosition=(qint64) ( (position*this->totalStreamSample())/(qreal)m_streamPosition->maximum());
        qWarning() << "AudioPlayer::convertSliderPositionToStreamSample setPosition@" << streamPosition << "/"<<this->totalStreamSample();
        this->setStreamSamplePosition(streamPosition);
    }
}

AudioPlayer::~AudioPlayer()
{
    stopPlay();
    m_audioOutput->disconnect();
}

void AudioPlayer::deviceChanged(int index) {
    stopPlay();
    m_audioOutput->disconnect(this);
    m_device = m_deviceBox->itemData(index).value<QAudioDeviceInfo>();
    this->initializeAudio(m_audioOutput->format());
}

void AudioPlayer::notified()
{    
//    qWarning() << "AudioPlayer::notified: bytesFree=" << m_audioOutput->bytesFree()
//               << ", " << "elapsedUSecs=" << m_audioOutput->elapsedUSecs()
//               << ", " << "processedUSecs=" << m_audioOutput->processedUSecs()
//               << ", " << "stream@sample/len=" << this->actualStreamSamplePosition()<< "/"<< this->totalStreamSample()
//               << ","  << " remaining sample" << this->remainingStreamSample();
    emit streamPositionChanged(this->actualStreamSamplePosition());
}

void AudioPlayer::pullTimerExpired()
{
    if (m_audioOutput && m_audioOutput->state() != QAudio::StoppedState) {
        int chunks = m_audioOutput->bytesFree()/m_audioOutput->periodSize();
        while (chunks) {
            qint64 len=0;
            switch (m_source) {
                case AudioPlayer::STREAM:
                    if (m_inputStream && m_inputStream->isOpen())
                        len = m_inputStream->read(m_buffer.data(), m_audioOutput->periodSize());
                    break;
                case AudioPlayer::FILE:
                    if (m_inputFile.isOpen())
                        len = m_inputFile.read(m_buffer.data(), m_audioOutput->periodSize());
                    break;
            }

           if (len) m_output_IODev->write(m_buffer.data(), len);
           if (len != m_audioOutput->periodSize()) //If the last read is shorter of the period size stream is finished and then exit.
               break;
           --chunks;
        }
    }
}

void AudioPlayer::toggleSuspendResume()
{
    if (m_audioOutput) {
        if (m_audioOutput->state() == QAudio::SuspendedState) {
            this->setPause(false);
        } else if (m_audioOutput->state() == QAudio::ActiveState) {
            this->setPause(true);
        } else if (m_audioOutput->state() == QAudio::IdleState) {
            qWarning() << "AudioPlayer::toggleSuspendResume status: IdleState, remaining idle.";
        }
    }
}

void AudioPlayer::toggleStartStop()
{
    if (m_audioOutput) {
        if (m_audioOutput->state() == QAudio::StoppedState) {
            this->setStart(true);
        } else if (m_audioOutput->state() == QAudio::ActiveState) {
            this->setStart(false);
        } else if (m_audioOutput->state() == QAudio::IdleState) {
            qWarning() << "AudioPlayer::toggleStartStop status: IdleState, going to start.";
           this->setStart(true);
        }  else if (m_audioOutput->state() == QAudio::SuspendedState) {
            qWarning() << "AudioPlayer::toggleStartStop status: IdleState, going to start.";
            this->setStart(false);
        }
    }
}

void AudioPlayer::toggleMode()
{
    setStart(false);
    if (m_pullMode) {
         qWarning() << "Setting push mode";
        m_modeButton->setText("Enable pull mode");
        m_pullMode = false;
    } else {
        qWarning() << "Setting pull mode";
        m_modeButton->setText("Enable push mode");
        m_pullMode = true;
    }
    setStart(true);
}

void AudioPlayer::toggleSource() {
    setStart(false);
    if (m_source==AudioPlayer::STREAM) {
        m_sourceButton->setText("Play internal generator");
        m_source=AudioPlayer::FILE;
    } else {
        m_sourceButton->setText("Play file");
        m_source=AudioPlayer::STREAM;
    }
    setStart(true);
}

void AudioPlayer::setPause(bool pause) {
    if (m_audioOutput && (m_audioOutput->state()==QAudio::ActiveState || m_audioOutput->state()==QAudio::SuspendedState) )
        if (pause) {
            qWarning() << "AudioPlayer::setPause status: Active, suspend()";
            this->pauseUI();
            m_audioOutput->suspend();
        } else {
            qWarning() << "AudioPlayer::setPause status: Suspended, resume()";
            this->startUI();
            m_audioOutput->resume();
        }
    else {
        qWarning() << "AudioPlayer::setPause do nothing";
    }
}

void AudioPlayer::setStart(bool start) {
    if (m_audioOutput) {
        if (start) {
            qWarning() << "AudioPlayer::setStart status: start()";
            this->startUI();
            startPlay();
        } else {            
            qWarning() << "AudioPlayer::setStart stop()";
            this->stopUI();
            stopPlay();
        }
    } else {
        qWarning() << "AudioPlayer::setStart do nothing";
    }
}

void AudioPlayer::stopUI() {
    m_startStopButton->setText("Start audio");
    m_suspendResumeButton->setText("Pause playback");
}

void AudioPlayer::startUI() {
    m_startStopButton->setText("Stop Audio");
    m_suspendResumeButton->setText("Pause playback");
}

void AudioPlayer::pauseUI() {
    m_suspendResumeButton->setText("Resume playback");
}

void AudioPlayer::stateChanged(QAudio::State state) {
    qWarning() << "AudioPlayer::stateChanged previous state is " <<AudioPlayer::audioStateToString(m_previousState)<< " now is " << AudioPlayer::audioStateToString(state);
    switch (state) {
        case QAudio::ActiveState:
            this->startUI();
            break;
        case QAudio::SuspendedState:
            this->pauseUI();
            break;
        case QAudio::StoppedState:
            this->stopUI();
            break;
        case QAudio::IdleState:
            this->stopUI();
            break;
        default:
            break;
    }
    m_previousState=state;
}


const QString AudioPlayer::audioStateToString(int state) {

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

QAudioFormat AudioPlayer::readFileHeader (QString filename) {

    QAudioFormat retval;
    SndfileHandle file;

    file = SndfileHandle (filename.toLocal8Bit().data());
    qWarning() <<  "AudioPlayer::readFileHeader hedaer of "<< filename << " format " <<  QString::number(file.format(),16) <<" SR=" << file.samplerate () <<" channels=" << file.channels ();

    retval.setCodec(AudioPlayer::decodeCodec(file.format()));
    if (!retval.codec().compare("audio/pcm")) {
       qWarning() <<  "AudioPlayer::readFileHeader unsupported type " <<  retval.codec();
    }
    retval.setChannelCount(file.channels());
    retval.setSampleRate(file.samplerate());
    retval.setByteOrder(AudioPlayer::decodeEndianess(file.format()));
    retval.setSampleType(AudioPlayer::decodePCMSignFormat(file.format()));
    retval.setSampleSize(AudioPlayer::decodePCMSampleSizeFormat(file.format()));

    qWarning() <<  "AudioPlayer::readFileHeader imported audio file is \n" << AudioPlayer::audioFormatToString(&retval);
    return retval;
}
const QString AudioPlayer::audioFormatToString(QAudioFormat *format) {
    QString retval;
    QTextStream _text(&retval);
    _text << "\tCHANNELS: " << format->channelCount() << "\n";
    _text << "\tCODECS: " << format->codec()<< "\n";
    _text << "\tSR: " << format->sampleRate()<< "\n";
    _text << "\tSAMPLE SIZE: " << format->sampleSize ()<< "\n";
    _text << "\tSAMPLE TYPE: " << AudioPlayer::audioSampleTypeToString(format->sampleType())<< "\n";
    _text << "\tENDIAN NESS: " << AudioPlayer::endianessFormatToString(format->byteOrder()) << "\n";

    return  *_text.string();
}

const QString AudioPlayer::decodeCodec(int format) {
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

const QAudioFormat::Endian AudioPlayer::decodeEndianess(int format) {
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

const QString AudioPlayer::endianessFormatToString(QAudioFormat::Endian format) {
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

const QAudioFormat::SampleType AudioPlayer::decodePCMSignFormat(int format) {
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

const int AudioPlayer::decodePCMSampleSizeFormat(int format) {
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


const QString AudioPlayer::audioSampleTypeToString(int sampleType) {
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
