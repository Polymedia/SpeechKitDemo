#ifndef SPEECHKITSERVICE_H
#define SPEECHKITSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Logger;
class Configuration;

class SpeechKitService : public QObject
{
    Q_OBJECT
public:
    explicit SpeechKitService(Logger *logger, Configuration *config, QObject *parent = 0);

    void recognizeSpeech(const QByteArray &data);
    void recognizeMorphology(const QString &text);
    void cancel();

signals:
    void success(QString speech);
    void error(QString error);

private:
    void sendMorphologyRequest(const QString &text);
    void setReply(QNetworkReply *reply);
    void deleteReply();
    bool parseSpeechRecognitionResult(const QString &response, QString *result);
    bool parseMorphologyRecognitionResult(const QByteArray &response, QString *result);

private slots:
    void onReplyFinished();
    void onReplyError(QNetworkReply::NetworkError error);
    void onReplySslError(QList<QSslError> errors);

private:
    Logger *m_logger;
    Configuration *m_config;

    enum State {
        Ready,
        SpeechRequest,
        MorphologyRequest
    };
    State m_state;

    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
};

#endif // SPEECHKITSERVICE_H
