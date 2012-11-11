#include "audioplayer.h"


AudioPlayer::AudioPlayer(QObject *parent)
    :   QObject(parent)
    ,   m_bufferLength(AUDIOPLAYER_DEFAULT_BUFFER_LEN)
    ,   m_pullTimer(new QTimer(this))
    ,   m_modeButton(0)
    ,   m_suspendResumeButton(0)
    ,   m_deviceBox(0)
    ,   m_device(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_audioOutput(0)
    ,   m_output_IODev(0)
{
    this->initClass();
    initializeWidget();
}

AudioPlayer::AudioPlayer(QWidget *parentWidget, QObject *parent )
    :   QObject(parent)
    ,   m_bufferLength(AUDIOPLAYER_DEFAULT_BUFFER_LEN)
    ,   m_pullTimer(new QTimer(this))
    ,   m_modeButton(0)
    ,   m_suspendResumeButton(0)
    ,   m_deviceBox(0)
    ,   m_device(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_audioOutput(0)
    ,   m_output_IODev(0)
{
    this->initClass();
    initializeWidget(parentWidget);
}

void AudioPlayer::initClass() {
    QAudioFormat format;
    format.setFrequency(AUDIOUTILS_DEFAULT_SR);
    format.setChannels(AUDIOUTILS_DEFAULT_NCHANNELS);
    format.setSampleSize(AUDIOUTILS_DEFAULT_SAMPLESIZE);
    format.setSampleType(AUDIOUTILS_DEFAULT_SAMPLETYPE);
    format.setCodec(AUDIOUTILS_DEFAULT_CODEC);
    format.setByteOrder(AUDIOUTILS_DEFAULT_BYTEORDER);

    m_buffer=QByteArray(m_bufferLength,0);
    m_pullMode = true;
    m_source=AudioPlayer::STREAM;
    connect(m_pullTimer, SIGNAL(timeout()), SLOT(pullTimerExpired()));

    initializeAudio(format);
    m_inputStream=NULL;
    m_previousState=QAudio::IdleState;
    m_audioOutput->stop();
}

void AudioPlayer::initializeWidget(QWidget * parentWidget)
{
    //init all different widget
    m_deviceBox = new QComboBox();
    foreach (const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
        m_deviceBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
    connect(m_deviceBox,SIGNAL(activated(int)),SLOT(deviceChanged(int)));

    m_modeButton = new QPushButton(parentWidget);
    m_modeButton->setText("Enable push mode");
    connect(m_modeButton, SIGNAL(clicked()), SLOT(toggleMode()));

    m_startStopButton = new QPushButton(parentWidget);
    m_startStopButton->setText("Start audio");
    connect(m_startStopButton, SIGNAL(clicked()), SLOT(toggleStartStop()));

    m_suspendResumeButton = new QPushButton(parentWidget);
    m_suspendResumeButton->setText("Suspend playback");
    connect(m_suspendResumeButton, SIGNAL(clicked()), SLOT(toggleSuspendResume()));

    m_sourceButton = new QPushButton(parentWidget);
    m_sourceButton->setText("Play file");
    connect(m_sourceButton, SIGNAL(clicked()), SLOT(toggleSource()));

    m_streamPositionSlider = new QSlider(Qt::Horizontal,parentWidget);
    m_streamPositionSlider->setTickPosition(QSlider::TicksBothSides);
    //connect(m_streamPositionSlider,SIGNAL(valueChanged(int)),this,SLOT(convertAndSetSliderPositionToStreamSample(int)));
    connect(m_streamPositionSlider,SIGNAL(sliderReleased()),this,SLOT(sliderToggled()));
    connect(m_streamPositionSlider,SIGNAL(sliderPressed()),this,SLOT(sliderToggled()));
    connect(this,SIGNAL(streamSamplePositionChanged(qint64)),this,SLOT(convertAndSetStreamSampleToSliderPosition(qint64)));
    m_streamPositionSlider->setMaximum(200);

    m_qvumeter=new QVUMeter(parentWidget);

    //Layouyt different control widget
    m_audioControlWidget=new QWidget(parentWidget);
    QHBoxLayout * audioControlLayout=new QHBoxLayout();

    //Layout control first row
    QWidget * _control=new QWidget(parentWidget);

    QVBoxLayout * _controlLayout=new QVBoxLayout();
    _controlLayout->addWidget(m_startStopButton);
    _controlLayout->addWidget(m_suspendResumeButton);
    _controlLayout->addWidget(m_streamPositionSlider);
    _control->setLayout(_controlLayout);
    audioControlLayout->addWidget(_control);
    audioControlLayout->addWidget(m_qvumeter);

    audioControlLayout->setSizeConstraint(QLayout::SetMinimumSize);
    m_audioControlWidget->setLayout(audioControlLayout);

    m_audioOptionWidget=new QWidget(parentWidget);
    QVBoxLayout * audioOptionLayout=new QVBoxLayout();
    audioOptionLayout->addWidget(m_deviceBox);
    audioOptionLayout->addWidget(m_modeButton);
    m_audioOptionWidget->setLayout(audioOptionLayout);

    m_testControlWidget=new QWidget(parentWidget);
    QVBoxLayout * testControlLayout=new QVBoxLayout();
    testControlLayout->addWidget(m_audioControlWidget);
    testControlLayout->addWidget(m_audioOptionWidget);
    testControlLayout->addWidget(m_sourceButton);
    m_testControlWidget->setLayout(testControlLayout);

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
            qDebug() << "Setting push mode";
            m_modeButton->setText("Enable pull mode");
            m_pullMode = false;
            break;
        case AudioPlayer::PUSH:
            qDebug() << "Setting pull mode";
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
        qDebug() << "AudioPlayer::setAudioFormat changing format format";
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
    //qDebug() << "AudioPlayer::setFileName set file name " << m_inputFile.fileName();
}

void AudioPlayer::startPlayPush(qint64 samplePosition) {
    m_pullMode=false;
    switch (m_source) {
        case AudioPlayer::STREAM:
            //qDebug() << "AudioPlayer::startPlayPush starting  with STREAM@samplePosition " << samplePosition;
            if (m_inputStream && m_inputStream->open(QIODevice::ReadOnly)) {
                this->setAudioFormat(m_inputStream->getAudioFormat());//Change format only if needed
                m_audioOutput->start(m_inputStream);
                m_output_IODev=m_inputStream;
                if (!this->setStreamSamplePosition(samplePosition))
                    qWarning() << "AudioPlayer::startPlayPull CAN'T' SET STREAM@samplePosition=" <<samplePosition;
                //m_inputStream->seek(this->convertSliderPositionToStreamSample(samplePosition));
                qDebug() << "AudioPlayer::startPlayPush started  with SR=" << m_audioOutput->format().sampleRate() <<
                            " @samplePos=" << this->convertSliderPositionToStreamSample(samplePosition);
            } else {
                qWarning() << "AudioPlayer::startPlayPush nothing to play";
              //  this->setPause(true);
            }
            break;
        case AudioPlayer::FILE:
            //qDebug() << "AudioPlayer::startPlayPush starting  with FILE@samplePosition " << samplePosition;
            if (m_inputFile.open(QIODevice::ReadOnly)){
                this->setAudioFormat(AudioUtils::readFileHeader(m_inputFile.fileName()));
                m_audioOutput->start(&m_inputFile);
                m_output_IODev=&m_inputFile;
                samplePosition=samplePosition<AUDIOPLAYER_HEADER_WAV_SAMPLES+4 ? AUDIOPLAYER_HEADER_WAV_SAMPLES+4 : samplePosition;
                if (!this->setStreamSamplePosition(samplePosition))
                    qWarning() << "AudioPlayer::startPlayPull CAN'T' SET FILE@samplePosition=" <<samplePosition;
                //m_inputFile.seek(AUDIOPLAYER_HEADER_WAV_SAMPLES+ (samplePosition<4 ? 4:samplePosition)) ;
                qDebug() << "AudioPlayer::startPlayPush started  with SR=" << m_audioOutput->format().sampleRate() <<
                            " @samplePos=" << samplePosition;
            } else {
                qWarning() << "AudioPlayer::startPlayPush can't open "<< m_inputFile.fileName();
            }
            break;
    }
}

void AudioPlayer::startPlayPull(qint64 samplePosition) {
    m_pullMode=true;
    switch (m_source) {
        case AudioPlayer::STREAM:
            if (m_inputStream && m_inputStream->open(QIODevice::ReadOnly)) {
                this->setAudioFormat(m_inputStream->getAudioFormat());//Change format only if needed
               // qDebug() << "AudioPlayer::startPlayPull starting  with STREAM@position " << samplePosition;
                m_output_IODev = m_audioOutput->start();
                if (!this->setStreamSamplePosition(samplePosition))
                    qWarning() << "AudioPlayer::startPlayPull CAN'T' SET STREAM@samplePosition=" <<samplePosition;
                //m_output_IODev->seek(samplePosition);

                m_pullTimer->start(AUDIOPLAYER_PULL_INTERVAL);
                //qDebug() << "AudioPlayer::startPlayPush started  with SR=" << m_audioOutput->format().sampleRate() <<
                //            " @samplePos=" << samplePosition;
            } else {
                qDebug() << "AudioPlayer::startPlayPull nothing to play, can't open stream";
               // this->setPause(true);
            }
            break;
        case AudioPlayer::FILE:
            //qDebug() << "AudioPlayer::startPlayPull starting  with FILE@samplePosition " << samplePosition;
            if (m_inputFile.open(QIODevice::ReadOnly)){
                this->setAudioFormat(AudioUtils::readFileHeader(m_inputFile.fileName()));
                m_output_IODev = m_audioOutput->start();
                samplePosition=samplePosition<AUDIOPLAYER_HEADER_WAV_SAMPLES+4 ? AUDIOPLAYER_HEADER_WAV_SAMPLES+4 : samplePosition;
                if (!this->setStreamSamplePosition(samplePosition))
                    qWarning() << "AudioPlayer::startPlayPull CAN'T' SET FILE@samplePosition=" <<samplePosition;
                //m_inputFile.seek(AUDIOPLAYER_HEADER_WAV_SAMPLES+ (position<4 ? 4:samplePosition;
                m_pullTimer->start(AUDIOPLAYER_PULL_INTERVAL);
                //qDebug() << "AudioPlayer::startPlayPush started  with SR=" << m_audioOutput->format().sampleRate() <<
                //            " @samplePos=" << samplePosition;
            } else {
                qWarning() << "AudioPlayer::startPlayPull can't open "<< m_inputFile.fileName();
            }
            break;
    }
}

void AudioPlayer::startPlay() {
   this->startPlay(0);
}

void AudioPlayer::startPlay(qint64 samplePosition) {
    stopPlay();
    m_previousPosition=samplePosition;
   // qDebug() << "AudioPlayer::startPlay starting@position " << samplePosition;
    if (m_pullMode) {
        startPlayPull(samplePosition);
    } else {
        startPlayPush(samplePosition);
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
                //qDebug() << "AudioPlayer::stopPlay called close for Internal Generator";
            }
            break;
        case AudioPlayer::FILE:
            if (m_inputFile.isOpen()) {
                m_inputFile.close();
               // qDebug() << "AudioPlayer::stopPlay called close for "<< m_inputFile.fileName();
            }
            break;
    }
}

bool AudioPlayer::setStreamSamplePosition(qint64 samplePosition) {
    bool retval=false;
    qint64 bytesPosition=0;
  //  if (m_audioOutput && (m_audioOutput->state() == QAudio::ActiveState || m_audioOutput->state() == QAudio::SuspendedState)) {
    if (m_audioOutput ) {
        switch (m_source) {
            case AudioPlayer::STREAM:
                if (m_inputStream && m_inputStream->isOpen()) {
                    bytesPosition=qMin(m_audioOutput->format().channels()*(m_audioOutput->format().sampleSize()/8)*samplePosition,m_inputStream->size() );
                    retval=m_inputStream->seek(bytesPosition);
                    if (!retval)
                        qWarning() << "AudioPlayer::setStreamSamplePosition CAN'T' set STREAM position@" << bytesPosition << "/"<< m_inputStream->size() << " bytes";
                }
                break;
            case AudioPlayer::FILE:
                if (m_inputFile.isOpen()) {
                    bytesPosition=qMin(m_audioOutput->format().channels()*(m_audioOutput->format().sampleSize()/8)*samplePosition,m_inputFile.size() );
                    retval=m_inputFile.seek(bytesPosition+AUDIOPLAYER_HEADER_WAV_SAMPLES); //TODO: + header!
                    if (!retval)
                            qWarning() << "AudioPlayer::setStreamSamplePosition CAN'T' set FILE position@" << bytesPosition << "/"<< m_inputFile.size() << " bytes";
                }
                break;
        }
    } else {
         qWarning() << "AudioPlayer::setStreamSamplePosition: DO NOTHING";
    }
    return retval;
}

qreal AudioPlayer::actualStreamTimePosition() {
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
    return ((qreal)retval)/(qreal)(m_audioOutput->format().sampleRate()*m_audioOutput->format().channels()*m_audioOutput->format().sampleSize()/8);
}

qreal AudioPlayer::actualStreamTotalTime() {
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
    return ((qreal)retval)/(qreal)(m_audioOutput->format().sampleRate()*m_audioOutput->format().channels()*m_audioOutput->format().sampleSize()/8);
}

qreal AudioPlayer::actualStreamRemainingTime() {
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
    return ((qreal)retval)/(qreal)(m_audioOutput->format().sampleRate()*m_audioOutput->format().channels()*m_audioOutput->format().sampleSize()/8);
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
    return retval/(m_audioOutput->format().channels()*m_audioOutput->format().sampleSize()/8);
}

qint64 AudioPlayer::actualStreamTotalSample() {
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
    return retval/(m_audioOutput->format().channels()*m_audioOutput->format().sampleSize()/8);
}

qint64 AudioPlayer::actualStreamRemainingSample() {
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
    return retval/(m_audioOutput->format().channels()*m_audioOutput->format().sampleSize()/8);
}

void AudioPlayer::sliderToggled() {
    this->convertAndSetSliderPositionToStreamSample( m_streamPositionSlider->value() );
}

void AudioPlayer::convertAndSetStreamSampleToSliderPosition(qint64 samplePosition) {
    if (!m_streamPositionSlider->isSliderDown()) {
        int sliderPosition=this->convertStreamSampleToSliderPosition(samplePosition);
        //qDebug() << "AudioPlayer::convertStreamSampleToSliderPosition sliderPosition=" << sliderPosition;
        bool sigStatus=m_streamPositionSlider->blockSignals(true);
        m_streamPositionSlider->setValue(sliderPosition);
        m_streamPositionSlider->blockSignals(sigStatus);
    }
}

int AudioPlayer::convertStreamSampleToSliderPosition(qint64 samplePosition) {
    return (int) (m_streamPositionSlider->maximum()* ((qreal)samplePosition/(qreal)this->actualStreamTotalSample()));
}

void AudioPlayer::convertAndSetSliderPositionToStreamSample(int sliderPosition) {
    qint64 streamPosition=this->convertSliderPositionToStreamSample(sliderPosition);
    //qDebug() << "AudioPlayer::convertSliderPositionToStreamSample setPosition@" << streamPosition << "/"<<this->actualStreamTotalSample();
    this->setStreamSamplePosition(streamPosition);

}

qint64 AudioPlayer::convertSliderPositionToStreamSample(int sliderPosition) {          
    return (qint64) ( ((qreal)sliderPosition)/((qreal)m_streamPositionSlider->maximum())*this->actualStreamTotalSample());
}

AudioPlayer::~AudioPlayer()
{
    stopPlay();
    m_audioOutput->disconnect();
}

void AudioPlayer::deviceChanged(int index) {
    stopPlay();
    stopUI();
    m_audioOutput->reset();
    m_audioOutput->disconnect(this);
    m_device = m_deviceBox->itemData(index).value<QAudioDeviceInfo>();
    this->initializeAudio(m_audioOutput->format());
}

void AudioPlayer::notified()
{    
    qint64 _actualPos;
    qint64 _bufLen;
    Q_ASSERT(_bufLen>=0);
    AudioUtils_structMeter _meter;

    if (m_source== AudioPlayer::STREAM) {
        _actualPos=m_inputStream->pos();
        _bufLen=_actualPos-m_previousPosition;
        Q_ASSERT(_bufLen>=0);
        QByteArray& _buffer= m_inputStream->buffer();
        //qDebug() << "AudioPlayer::notified: buffer@  "<< &_buffer << " len=" << m_buffer.length()<< " pos=" <<;
        const char * _data=_buffer.constData();
        _meter=AudioUtils::getAudioPeak(&_data[m_inputStream->pos()],_bufLen,m_audioOutput->format());
        //qDebug() << "AudioPlayer::notified: buffer@  "<< &_data << " len=" << m_buffer.length();
    }
    else if (m_source== AudioPlayer::FILE) {
        _actualPos=m_inputFile.pos();
        _bufLen=_actualPos-m_previousPosition;
        Q_ASSERT(_bufLen>=0);
        QByteArray _buffer= m_inputFile.peek(m_buffer.length());
        //qDebug() << "AudioPlayer::notified: buffer@  "<< &_buffer << " len=" << m_buffer.length()<< " pos=" <<m_inputFile.pos();
        const char * _data=_buffer.constData();
        _meter=AudioUtils::getAudioPeak(_data,_bufLen,m_audioOutput->format());
    }
    m_qvumeter->setLeftValue(_meter.peak);
    m_qvumeter->setRightValue(_meter.rms);

    emit streamSamplePositionChanged(this->actualStreamSamplePosition());
    emit streamTimePositionChanged(this->actualStreamTimePosition());
    m_previousPosition=_actualPos;
}

void AudioPlayer::pullTimerExpired()
{
    if (m_audioOutput && m_audioOutput->state() != QAudio::StoppedState) {
        int chunks = m_audioOutput->bytesFree()/m_audioOutput->periodSize();
        if (chunks <=0 &&  m_audioOutput->state()!=QAudio::SuspendedState) {
            qWarning() << "AudioPlayer::pullTimerExpired: data not ready, device has bytesFree=" << m_audioOutput->bytesFree() << " and a periodSize=" << m_audioOutput->periodSize();
        }

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

           if (len) {
               m_output_IODev->write(m_buffer.data(), len);
           }
           if (len != m_audioOutput->periodSize()) //If the last read is shorter of the period size stream is finished and then exit.
               break;
           --chunks;
        }
    } else qWarning() << "AudioPlayer::pullTimerExpired: m_audioOutput is "<< m_audioOutput << " && m_audioOutput->state() != QAudio::StoppedState" << AudioUtils::audioStateToString(  m_audioOutput->state());
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
    //qDebug() << "AudioPlayer::toggleStartStop sliderValue=" << this->convertSliderPositionToStreamSample(m_streamPositionSlider->value());
    if (m_audioOutput) {
        if (m_audioOutput->state() == QAudio::StoppedState) {
            this->setStart(true,this->convertSliderPositionToStreamSample(m_streamPositionSlider->value()) );
        } else if (m_audioOutput->state() == QAudio::ActiveState) {
            this->setStart(false);
        } else if (m_audioOutput->state() == QAudio::IdleState) {
          //  qDebug() << "AudioPlayer::toggleStartStop status: IdleState, going to start.";
            this->setStart(true,this->convertSliderPositionToStreamSample(m_streamPositionSlider->value()) );
        }  else if (m_audioOutput->state() == QAudio::SuspendedState) {
          //  qDebug() << "AudioPlayer::toggleStartStop status: IdleState, going to start.";
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
    //setStart(true);
}

void AudioPlayer::setPause(bool pause) {
    if (m_audioOutput && (m_audioOutput->state()==QAudio::ActiveState || m_audioOutput->state()==QAudio::SuspendedState) )
        if (pause) {
            //qDebug() << "AudioPlayer::setPause status: Active, suspend()";
            this->pauseUI();
            m_audioOutput->suspend();
        } else {
            //qDebug() << "AudioPlayer::setPause status: Suspended, resume()";
            this->startUI();
            m_audioOutput->resume();
        }
    else {
        qWarning() << "AudioPlayer::setPause do nothing";
    }
}

void AudioPlayer::setStart(bool start, int samplePosition) {
    if (m_audioOutput) {
        if (start) {
           // qDebug() << "AudioPlayer::setStart status: start() @ " << samplePosition;
            startPlay(samplePosition);
        } else {            
            //qDebug() << "AudioPlayer::setStart stop()";
            stopPlay();
        }
    } else {
        qWarning() << "AudioPlayer::setStart do nothing";
    }
}

void AudioPlayer::stopUI() {
    //qDebug() << "AudioPlayer::stopUI stop UI called";
    m_startStopButton->setText("Start audio");
    m_suspendResumeButton->setText("Pause playback");

    //Need to block signals before going on...
    bool signalStatus=m_streamPositionSlider->blockSignals(true);
    m_streamPositionSlider->setValue(0);
    m_streamPositionSlider->blockSignals(signalStatus);
    m_qvumeter->setLeftValue(0);
    m_qvumeter->setRightValue(0);
}

void AudioPlayer::startUI() {
    //qDebug() << "AudioPlayer::startUI start UI called";
    m_startStopButton->setText("Stop Audio");
    m_suspendResumeButton->setText("Pause playback");
}

void AudioPlayer::pauseUI() {
    m_suspendResumeButton->setText("Resume playback");
}

void AudioPlayer::stateChanged(QAudio::State state) {
    //qDebug() << "AudioPlayer::stateChanged previous state is " <<AudioUtils::audioStateToString(m_previousState)<< " next state " << AudioUtils::audioStateToString(state);
    if (m_previousState!=state) {
        qDebug() << "AudioPlayer::stateChanged previous state is " <<AudioUtils::audioStateToString(m_previousState)<< " going to " << AudioUtils::audioStateToString(state);
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
    } else {
        qWarning() << "AudioPlayer::stateChanged nothing to do.";
    }

    //Q_ASSERT(m_audioOutput->state()==m_previousState);
    //qDebug() << "AudioPlayer::stateChanged state is " <<AudioUtils::audioStateToString(m_previousState);

}


