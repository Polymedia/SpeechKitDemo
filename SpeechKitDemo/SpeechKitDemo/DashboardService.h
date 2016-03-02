#ifndef DASHBOARDSERVICE_H
#define DASHBOARDSERVICE_H

#include <QObject>
#include <QUrl>

#include "Dashboard.h"

class Configuration;
class SpeechKitService;

class DashboardService : public QObject
{
    Q_OBJECT
public:
    explicit DashboardService(Configuration *config, SpeechKitService *speechKitService, QObject *parent = 0);

    int count() const;
    Dashboard at(int index) const;
    void append(const Dashboard &dashboard);
    void setAt(int index, const Dashboard &dashboard);
    void remove(int first, int last);

    void recognize(const QByteArray &data);
    void cancel();

signals:
    void recognitionSuccess(QString url);
    void recognitionNotFound();
    void recognitionFailed(QString error);

public slots:

private:
    int getWordCount(int index, QList<int> aliasIndexes) const;

private slots:
    void onSpeechKitSuccess(QString speech);
    void onSpeechKitError(QString error);

private:
    Configuration *m_config;
    SpeechKitService *m_speechKitService;

    enum State {
        Ready,
        SpeechRecognition,
        Preparation
    };
    State m_state;
    int m_preparingIndex;
    int m_aliasIndex;
    QString m_speech;

    QList<Dashboard> m_dashboards;

};

#endif // DASHBOARDSERVICE_H
