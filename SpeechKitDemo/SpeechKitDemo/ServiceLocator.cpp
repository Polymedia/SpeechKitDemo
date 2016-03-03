#include "ServiceLocator.h"

#include "Logger.h"
#include "Configuration/Configuration.h"
#include "DashboardService.h"
#include "AudioRecorder.h"
#include "SpeechKitService.h"
#include "MainViewModel.h"

ServiceLocator *ServiceLocator::m_instance = nullptr;

ServiceLocator::ServiceLocator()
{
    if (m_instance == nullptr) {
        m_logger = new Logger(this);
        m_configuration = new Configuration(m_logger, this);
        m_audioRecorder = new AudioRecorder(m_logger, this);
        m_speechKitService = new SpeechKitService(m_logger, m_configuration, this);
        m_dashboardService = new DashboardService(m_configuration, m_speechKitService, this);
        m_mainViewModel = new MainViewModel(m_configuration,
                                            m_dashboardService,
                                            m_audioRecorder,
                                            this);

        m_instance = this;
    }
}

ServiceLocator *ServiceLocator::instance()
{
    return m_instance;
}

Logger *ServiceLocator::logger()
{
    return m_instance->m_logger;
}

Configuration *ServiceLocator::configuration()
{
    return m_instance->m_configuration;
}

DashboardService *ServiceLocator::dashboardService()
{
    return m_instance->m_dashboardService;
}

AudioRecorder *ServiceLocator::audioRecorder()
{
    return m_instance->m_audioRecorder;
}

SpeechKitService *ServiceLocator::speechKitService()
{
    return m_instance->m_speechKitService;
}

MainViewModel *ServiceLocator::mainViewModel()
{
    return m_instance->m_mainViewModel;
}
