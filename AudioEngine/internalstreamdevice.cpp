#include <QDebug>
#include <QVBoxLayout>

#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QtCore/qmath.h>
#include <QtCore/qendian.h>
#include "internalstreamdevice.h"





InternalStreamDevice::InternalStreamDevice(QAudioFormat format, QObject *parent)
    :   QIODevice(parent)
    ,   m_pos(0)
{
    m_format=QAudioFormat(format);
}

InternalStreamDevice::~InternalStreamDevice()
{

}



bool InternalStreamDevice::open()
{
    if (QIODevice::open(QIODevice::ReadOnly)) {
        m_pos = 0;
        return true;
    } else {
        return false;
    }

}

void InternalStreamDevice::close()
{
    m_pos = 0;
    QIODevice::close();
}

void InternalStreamDevice::setData(qreal * data, qint64 len) {

    //Verify if data are present
    if (!data) return;
    //CHANGING DATA, STOPPING AUDIO STREAM
    if (this->isOpen()) {
        this->close();
    }

    const int channelBytes = m_format.sampleSize() / 8;
    const int sampleBytes = m_format.channels() * channelBytes;
   // qint64 length = (m_format.frequency() * m_format.channels() * (m_format.sampleSize() / 8))
   //                     * durationUs / 100000;
    qint64 length = ( m_format.channels() * (m_format.sampleSize() / 8)) * len;
    Q_ASSERT(length % sampleBytes == 0);
    Q_UNUSED(sampleBytes) // suppress warning in release builds
    qWarning() << "InternalStreamDevice::setData channels="<< m_format.channels() << " channelBytes=" << channelBytes << " sampleBytes="<<sampleBytes << " length=" << length;
    m_buffer.resize(length);
    unsigned char *ptr = reinterpret_cast<unsigned char *>(m_buffer.data());
    int sampleIndex = 0;

    while (length) {
        for (int i=0; i<m_format.channels(); ++i) {
            if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                const quint8 value = static_cast<quint8>((1.0 + data[sampleIndex]) / 2 * 255);
                *reinterpret_cast<quint8*>(ptr) = value;
            } else if (m_format.sampleSize() == 8 && m_format.sampleType() == QAudioFormat::SignedInt) {
                const qint8 value = static_cast<qint8>(data[sampleIndex] * 127);
                *reinterpret_cast<quint8*>(ptr) = value;
            } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::UnSignedInt) {
                quint16 value = static_cast<quint16>((1.0 + data[sampleIndex]) / 2 * 65535);
                if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                    qToLittleEndian<quint16>(value, ptr);
                else
                    qToBigEndian<quint16>(value, ptr);
            } else if (m_format.sampleSize() == 16 && m_format.sampleType() == QAudioFormat::SignedInt) {
                qint16 value = static_cast<qint16>(data[sampleIndex] * 32767);
                if (m_format.byteOrder() == QAudioFormat::LittleEndian)
                    qToLittleEndian<qint16>(value, ptr);
                else
                    qToBigEndian<qint16>(value, ptr);
            } //else if float
            ptr += channelBytes;
            length -= channelBytes;
        }
        ++sampleIndex;
    }
    emit(dataChanged());
}

qint64 InternalStreamDevice::readData(char *data, qint64 len)
{
    if (m_pos==m_buffer.size()) return -1;
    qWarning() << "InternalStreamDevice::readData called with len="<<len ;
    const qint64 chunk = qMin((m_buffer.size() - m_pos), len );
    memcpy(data , m_buffer.constData() + m_pos, chunk);
    m_pos = (m_pos + chunk);
    qWarning() << "InternalStreamDevice::readData returning with chunk="<<chunk ;
    return chunk;
}

qint64 InternalStreamDevice::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

qint64 InternalStreamDevice::bytesAvailable() const
{
    return /*m_buffer.size() +*/ QIODevice::bytesAvailable();
}

qint64 InternalStreamDevice::pos() const
{
    return m_pos;
}

qint64 InternalStreamDevice::size() const
{
    return m_buffer.size();
}

bool InternalStreamDevice::seek(qint64 pos) {
    qWarning() << "InternalStreamDevice::seek called with pos="<<pos <<"/" <<m_buffer.size();
    if (pos > m_buffer.size()) return false;

    m_pos=pos;
    return true;
}

