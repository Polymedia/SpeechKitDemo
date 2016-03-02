#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QObject>

#include "DashboardListModel.h"

class Configuration;
class DashboardService;
class ServiceLocator;
class Dashboard;
class AudioRecorder;
class SpeechKitService;

class MainViewModel : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
public:
    enum State {
        Ready,
        Listening,
        Recognizing,
        Finished,
        FinishedNotFound,
        Error,
        Settings
    };

    Q_INVOKABLE void toggleState();
    Q_INVOKABLE void cancel();
    Q_INVOKABLE void quit();

    explicit MainViewModel(Configuration *config,
                           DashboardService *dashboardService,
                           AudioRecorder *audioRecorder,
                           QObject *parent = 0);
    explicit MainViewModel(QObject *parent = 0);
    ~MainViewModel();

    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(bool isCancellationEnabled READ isCancellationEnabled NOTIFY isCancellationEnabledChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    Q_PROPERTY(DashboardListModel* dashboards READ dashboards NOTIFY dashboardsChanged)

    State state() const { return m_state; }
    bool isCancellationEnabled() const;
    void setState(State newState);
    DashboardListModel* dashboards() const { return m_dashboards; }

    QString error() const { return m_error; }

signals:    
    void stateChanged(State state);
    void isCancellationEnabledChanged(bool value);
    void errorChanged(QString description);
    void dashboardsChanged(QList<QObject *> dashboards);

private:
    void startListening();
    void stopListening(bool accepted);
    void setErrorState(const QString &description);

private slots:
    void onRecognitionSuccess(QString speech);
    void onRecognitionNotFound();
    void onRecognitionError(QString error);

private:
    Configuration *m_config;
    DashboardService *m_dashboardService;
    AudioRecorder *m_audioRecorder;

    State m_state;
    QString m_error;
    DashboardListModel *m_dashboards;
};

Q_DECLARE_METATYPE(MainViewModel::State)

#endif // MAINVIEWMODEL_H
