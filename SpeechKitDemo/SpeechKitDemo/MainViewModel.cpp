#include "MainViewModel.h"

#include <QDesktopServices>
#include <QGuiApplication>
#include <QUrl>
#include <QUrlQuery>
#include <QXmlStreamReader>
#include <QFile>
#include <QtQml>

#include "DashboardService.h"
#include "ServiceLocator.h"
#include "Configuration/Configuration.h"
#include "AudioRecorder.h"
#include "SpeechKitService.h"

MainViewModel::MainViewModel(Configuration *config,
                             DashboardService *dashboardService,
                             AudioRecorder *audioRecorder,
                             QObject *parent) :
    QObject(parent),
    m_config(config),
    m_dashboardService(dashboardService),
    m_audioRecorder(audioRecorder),
    m_state(State::Ready),
    m_dashboards(new DashboardListModel(m_dashboardService, this))
{
    connect(m_dashboardService, SIGNAL(recognitionSuccess(QString)), SLOT(onRecognitionSuccess(QString)));
    connect(m_dashboardService, SIGNAL(recognitionNotFound()), SLOT(onRecognitionNotFound()));
    connect(m_dashboardService, SIGNAL(recognitionFailed(QString)), SLOT(onRecognitionError(QString)));

    qRegisterMetaType<DashboardListModel *>();
    qmlRegisterType<MainViewModel>("Visiology", 1, 0, "MainViewModelType");
}

MainViewModel::MainViewModel(QObject *parent)  :
    MainViewModel(ServiceLocator::instance()->configuration(),
                  ServiceLocator::instance()->dashboardService(),
                  ServiceLocator::instance()->audioRecorder(),
                  parent)
{

}

MainViewModel::~MainViewModel()
{
}

bool MainViewModel::isCancellationEnabled() const
{
    return m_state == Listening || m_state == Recognizing;
}

void MainViewModel::setState(MainViewModel::State newState)
{
    if (m_state != newState) {
        m_state = newState;
        emit stateChanged(newState);
        emit isCancellationEnabledChanged(isCancellationEnabled());
    }
}

void MainViewModel::toggleState()
{
    if (m_state == Ready) {
        startListening();
    } else if (m_state == Listening) {
        stopListening(true);        
    } else if (m_state == Finished || m_state == FinishedNotFound || m_state == Error) {
        setState(Ready);
    }
}

void MainViewModel::cancel()
{
    if (!isCancellationEnabled())
        return;

    if (m_state == Listening)
        stopListening(false);

    if (m_state == Recognizing)
        m_dashboardService->cancel();

    setState(Ready);
}

void MainViewModel::quit()
{
    QGuiApplication::quit();
}

void MainViewModel::startListening()
{
    m_audioRecorder->start();

    setState(Listening);
}

void MainViewModel::stopListening(bool accept)
{
    m_audioRecorder->stop();

    if (!accept)
        return;

    m_dashboardService->recognize(m_audioRecorder->data());

    setState(Recognizing);
}

void MainViewModel::setErrorState(const QString &description)
{
    m_error = description;
    emit errorChanged(description);

    setState(Error);
}

void MainViewModel::onRecognitionSuccess(QString url)
{
    QDesktopServices::openUrl(url);
    setState(Finished);
}

void MainViewModel::onRecognitionNotFound()
{
    setState(FinishedNotFound);
}

void MainViewModel::onRecognitionError(QString error)
{
    setErrorState(error);
}
