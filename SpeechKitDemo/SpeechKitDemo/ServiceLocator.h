#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include <QObject>

class Logger;
class Configuration;
class DashboardService;
class AudioRecorder;
class SpeechKitService;
class MainViewModel;

class ServiceLocator : public QObject
{
    Q_OBJECT
public:
    explicit ServiceLocator();
    static ServiceLocator *instance();

    Logger *logger();
    Configuration *configuration();
    DashboardService *dashboardService();
    AudioRecorder *audioRecorder();
    SpeechKitService *speechKitService();

    MainViewModel *mainViewModel();
signals:

public slots:

private:
    static ServiceLocator *m_instance;

    Logger *m_logger;
    Configuration *m_configuration;
    DashboardService *m_dashboardService;
    AudioRecorder *m_audioRecorder;
    SpeechKitService *m_speechKitService;
    MainViewModel *m_mainViewModel;
};

#endif // SERVICELOCATOR_H
