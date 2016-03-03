#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>
#include <QAudioFormat>
#include <QAudioInput>
#include <QBuffer>

class Logger;

class AudioRecorder : public QObject
{
    Q_OBJECT
public:
    explicit AudioRecorder(Logger *logger, QObject *parent = 0);

    void start();
    void stop();

    const QByteArray &data() const;

signals:

public slots:

private:
    Logger *m_logger;

    QAudioFormat m_format;
    QAudioInput *m_audioInput;
    QBuffer m_buffer;
};

#endif // AUDIORECORDER_H
