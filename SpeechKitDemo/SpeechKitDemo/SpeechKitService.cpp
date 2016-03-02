#include "SpeechKitService.h"

#include <QXmlStreamReader>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "Logger.h"
#include "Configuration/Configuration.h"

SpeechKitService::SpeechKitService(Logger *logger,
                                   Configuration *config,
                                   QObject *parent) :
    QObject(parent),
    m_logger(logger),
    m_config(config),
    m_state(Ready)
{
    m_manager= new QNetworkAccessManager(this);
}

void SpeechKitService::recognizeSpeech(const QByteArray &data)
{
    QUrl url(QString("https://asr.yandex.net/asr_xml?key=%1&uuid=12345678123456781234567812345678&topic=queries&lang=ru-RU").arg(m_config->yandexApiKey()));
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "audio/x-pcm;bit=16;rate=16000");

    m_state = SpeechRequest;
    setReply(m_manager->post(request, data));
}

void SpeechKitService::recognizeMorphology(const QString &text)
{
    if (m_state != Ready)
        return;

    sendMorphologyRequest(text);
}

void SpeechKitService::cancel()
{
    if (m_state == Ready)
        return;

    m_reply->abort();
}

void SpeechKitService::sendMorphologyRequest(const QString &text)
{
    m_state = MorphologyRequest;

    QUrl url(QString("https://vins-markup.voicetech.yandex.net/markup/0.x/?text=%1&layers=Morph&key=%2")
             .arg(text)
             .arg(m_config->yandexApiKey()));
    QNetworkRequest request(url);

    setReply(m_manager->get(request));
}

void SpeechKitService::setReply(QNetworkReply *reply)
{
    m_reply = reply;
    connect(m_reply, SIGNAL(finished()), SLOT(onReplyFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(onReplyError(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(onReplySslError(QList<QSslError>)));
}

void SpeechKitService::deleteReply()
{
    if (m_reply != nullptr) {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    m_state = Ready;
}

bool SpeechKitService::parseSpeechRecognitionResult(const QString &response, QString *result)
{
    QXmlStreamReader xml(response);
    QString recognitionResult;

    // Confidence can be less than zero, so use the smallest value of Int32
    double maxConfidence = INT32_MIN;

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == "recognitionResults") {
                QXmlStreamAttributes attributes = xml.attributes();
                if (!attributes.hasAttribute("success")) {
                    return false;
                }

                bool ok = false;
                int success = attributes.value("success").toInt(&ok);
                if (!ok || !success) {
                    return false;
                }
            }

            if (xml.name() == "variant") {
                QXmlStreamAttributes attributes = xml.attributes();
                if (!attributes.hasAttribute("confidence"))
                    continue;

                bool ok = false;
                double confidence = attributes.value("confidence").toDouble(&ok);
                if (!ok)
                    continue;

                if (confidence > maxConfidence) {
                    maxConfidence = confidence;
                    recognitionResult = xml.readElementText();
                }
            }
        }
    }

    *result = recognitionResult;
    return true;
}

bool SpeechKitService::parseMorphologyRecognitionResult(const QByteArray &response, QString *result)
{
    QStringList words;

    auto json = QJsonDocument::fromJson(response);
    auto root = json.object();
    auto morphsArray = root["Morph"].toArray();

    foreach (QJsonValue morphValue, morphsArray) {
        auto morphObject = morphValue.toObject();
        auto lemmasArray = morphObject["Lemmas"].toArray();
        if (lemmasArray.count() > 0) {
            // Chose first lemma
            auto lemmaObject = lemmasArray[0].toObject();
            words.append(lemmaObject["Text"].toString());
        }
    }

    *result = words.join(" ");
    return true;
}

void SpeechKitService::onReplyFinished()
{
    if (m_reply == nullptr)
        return; // It seems there was a error, the reply was handled in onReplyError

    if (m_state == SpeechRequest) {
        QString response = m_reply->readAll();

        QString recognitionResult;
        if (!parseSpeechRecognitionResult(response, &recognitionResult)) {
            m_logger->warning("Yandex speech kit recognition failed");

            deleteReply();

            emit error(tr("Speech recognition has failed"));
            return;
        }

        deleteReply();

        // Make request to morphology service
        sendMorphologyRequest(recognitionResult);
    } else if (m_state == MorphologyRequest) {
        QByteArray response = m_reply->readAll();

        QString recognitionResult;
        if (!parseMorphologyRecognitionResult(response, &recognitionResult)) {
            m_logger->warning("Yandex morphology recognition failed");

            deleteReply();

            emit error(tr("Morphology recognition has failed"));
            return;
        }

        deleteReply();

        emit success(recognitionResult);
    }
}

void SpeechKitService::onReplyError(QNetworkReply::NetworkError networkError)
{
    if (networkError == QNetworkReply::OperationCanceledError) {
        // Operation has been cancelled by user
        deleteReply();
        return;
    }

    m_logger->error(QString("Network error: %1 - %2")
                    .arg(networkError)
                    .arg(m_reply->errorString())
                    );

    deleteReply();

    emit error(tr("Network error"));
}

void SpeechKitService::onReplySslError(QList<QSslError> sslErrors)
{
    foreach (auto error, sslErrors) {
        m_logger->error(QString("Ssl error: %1 - %2")
                        .arg(error.error())
                        .arg(error.errorString())
                        );
    }

    deleteReply();

    emit error(tr("Ssl error"));
}
