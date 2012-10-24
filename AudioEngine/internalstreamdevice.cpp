#include <QDebug>
#include <QVBoxLayout>

#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QtCore/qmath.h>
#include <QtCore/qendian.h>
#include "internalstreamdevice.h"
#include  "audioutils.h"



InternalStreamDevice::InternalStreamDevice(QObject *parent)
    :   QBuffer(parent)
{
    m_format=QAudioFormat();
    m_buffer=&this->buffer();
}

InternalStreamDevice::InternalStreamDevice(QAudioFormat format, QObject *parent)
    :   QBuffer(parent)
{
    m_format=QAudioFormat(format);
    m_buffer=&this->buffer();
}

InternalStreamDevice::~InternalStreamDevice()
{

}

const int InternalStreamDevice::getSampleLength() {
    const int channelBytes = m_format.sampleSize() / 8;
    const int sampleBytes = m_format.channels() * channelBytes;
    return sampleBytes;
}

const qint64 InternalStreamDevice::getStreamLength() {
    const qint64 retval=this->size()/this->getSampleLength();
    return retval;
}

bool InternalStreamDevice::setAudioData(qreal * data, qint64 len) {

    //Verify if data are present
    if (!data) {
        qWarning() << "InternalStreamDevice::setData no data to set";
        return false;
    }

    //Check validity
    if (!m_format.isValid()) {
        qWarning() << "InternalStreamDevice::setData invalid format" << AudioUtils::audioFormatToString(&m_format) ;
        return false;
    }

    //CHANGING DATA, STOPPING AUDIO STREAM
    if (this->isOpen()) {
        this->close();
    }

    //delete buffer
    if (m_buffer->size() > 0 ) {
        m_buffer->clear();
        qDebug() << "InternalStreamDevice::setData buffer cleared, now size is: "<< m_buffer->size();
    }

    const int channelBytes = m_format.sampleSize() / 8;
    const int sampleBytes = m_format.channels() * channelBytes;
    qint64 length = ( m_format.channels() * (m_format.sampleSize() / 8)) * len;
    Q_ASSERT(length % sampleBytes == 0);
    Q_UNUSED(sampleBytes) // suppress warning in release builds
    qDebug() << "InternalStreamDevice::setData channels="<< m_format.channels() << " channelBytes=" << channelBytes << " sampleBytes="<<sampleBytes << " length=" << length;
    //m_buffer->resize(length); //TODO: resize  in one step! Usando insert (con indice la cosa sembra esplodere!!)
    int sampleIndex = 0;

    while (length) {
        for (int i=0; i<m_format.channels(); ++i) {
            if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                const quint8 value = static_cast<quint8>((1.0 + data[sampleIndex]) / 2 * 255);
                m_buffer->append((char*)&value,channelBytes);
            } else if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::SignedInt) {
                const qint8 value = static_cast<qint8>(data[sampleIndex] * 127);
                m_buffer->append((char*)&value,channelBytes);
            } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                quint16 value = static_cast<quint16>((1.0 + data[sampleIndex]) / 2 * 65535);
                if (m_format.byteOrder() == QAudioFormat::LittleEndian) {
                    value=(const quint16) qToLittleEndian<quint16>(value);
                    m_buffer->append((char*) &value,channelBytes);
                } else {
                    value=(const quint16) qToBigEndian<quint16>(value);
                    m_buffer->append((char*) &value,channelBytes);
                }
            } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::SignedInt) {
                qint16 value = static_cast<qint16>(data[sampleIndex] * 32767);
                if (m_format.byteOrder() == QAudioFormat::LittleEndian) {
                    value=(const qint16) qToLittleEndian<qint16>(value);
                    m_buffer->append((char*) &value,channelBytes);
                } else {
                    value=(const qint16) qToBigEndian<qint16>(value);
                    m_buffer->append((char*) &value,channelBytes);
                }
            } else if (m_format.sampleType() == QAudioFormat::Float) { //TODO: support this format!!
                qWarning() << "InternalStreamDevice::setData, float data not supported." ;
                return false;
            }
            length -= channelBytes;
        }
        ++sampleIndex;
    }
    emit(dataChanged());
    qDebug() << "InternalStreamDevice::setData create m_buffer m_buffer.size="<<m_buffer->size() << " m_buffer.length="<<  m_buffer->length() <<" this size=" << this->size();

    return true;
}
