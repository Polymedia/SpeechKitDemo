#include "AudioRecorder.h"

#include "Logger.h"

AudioRecorder::AudioRecorder(Logger* logger, QObject *parent) :
    QObject(parent),
    m_logger(logger)
{
    m_format.setSampleRate(16000);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setSampleType(QAudioFormat::SignedInt);
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setCodec("audio/pcm");

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(m_format)) {
        m_logger->warning("Default format not supported - trying to use nearest");
        m_format = info.nearestFormat(m_format);
    }

    m_audioInput = new QAudioInput(m_format, this);
}

void AudioRecorder::start()
{
    m_buffer.setData(QByteArray());
    m_buffer.open(QIODevice::WriteOnly);
    m_audioInput->start(&m_buffer);
}

void AudioRecorder::stop()
{
    m_audioInput->stop();
    m_buffer.close();
}

const QByteArray &AudioRecorder::data() const
{
    return m_buffer.data();
}
