#include "DashboardService.h"

#include <QStringList>

#include "Configuration/Configuration.h"
#include "SpeechKitService.h"

DashboardService::DashboardService(Configuration *config, SpeechKitService *speechKitService, QObject *parent) :
    QObject(parent),
    m_config(config),
    m_speechKitService(speechKitService),
    m_state(Ready)
{
    connect(m_speechKitService, SIGNAL(success(QString)), SLOT(onSpeechKitSuccess(QString)));
    connect(m_speechKitService, SIGNAL(error(QString)), SLOT(onSpeechKitError(QString)));

    m_dashboards = m_config->dashboards();
}

int DashboardService::count() const
{
    return m_dashboards.count();
}

Dashboard DashboardService::at(int index) const
{
    return m_dashboards[index];
}

void DashboardService::append(const Dashboard &dashboard)
{
    m_dashboards.append(dashboard);
    m_config->setDashboards(m_dashboards);
}

void DashboardService::setAt(int index, const Dashboard &dashboard)
{
    m_dashboards[index] = dashboard;
    m_config->setDashboards(m_dashboards);
}

void DashboardService::remove(int first, int last)
{
    for (int i = first; i < last; i++)
        m_dashboards.removeAt(i);
    m_config->setDashboards(m_dashboards);
}

void DashboardService::recognize(const QByteArray &data)
{
    m_state = SpeechRecognition;
    m_preparingIndex = -1;
    m_aliasIndex = -1;
    m_speech.clear();
    m_speechKitService->recognizeSpeech(data);
}

void DashboardService::cancel()
{
    if (m_state == Ready)
        return;

    m_speechKitService->cancel();

    m_state = Ready;
}

int DashboardService::getWordCount(int index, QList<int> aliasIndexes) const
{
    auto dashboard = m_dashboards[index];
    auto aliases = dashboard.aliases();
    int count = 0;
    foreach (int i, aliasIndexes) {
        count += aliases[i].morpholized.split(" ", QString::SkipEmptyParts).count();
    }

    return count;
}

void DashboardService::onSpeechKitSuccess(QString result)
{
    if (m_state == SpeechRecognition) {
        m_speech = result.toLower().simplified();
        m_state = Preparation;
    }

    if (m_state == Preparation) {
        // Preparation
        if (m_preparingIndex >= 0 && m_preparingIndex < m_dashboards.count()) {
            auto dashboard = m_dashboards[m_preparingIndex];
            auto aliases = dashboard.aliases();
            if (m_aliasIndex >= 0 && m_aliasIndex < aliases.count()) {
                aliases[m_aliasIndex].morpholized = result;
                dashboard.setAliases(aliases);

                m_dashboards[m_preparingIndex] = dashboard;
                m_config->setDashboards(m_dashboards);
            }
        }

        for (int i = 0; i < m_dashboards.count(); i++) {
            auto dashboard = m_dashboards[i];
            auto aliases = dashboard.aliases();
            for (int j = 0; j < aliases.count(); j++) {
                auto alias = aliases[j];
                if (!alias.original.isEmpty() && alias.morpholized.isEmpty()) {
                    m_preparingIndex = i;
                    m_aliasIndex = j;
                    m_speechKitService->recognizeMorphology(alias.original);
                    return;
                }
            }
        }

        // Find dashboard
        QMap<int, QList<int>> correlations;

        for (int i = 0; i < m_dashboards.count(); i++) {
            auto dashboard = m_dashboards[i];
            auto aliases = dashboard.aliases();
            for (int j = 0; j < aliases.count(); j++) {
                auto alias = aliases[j];
                auto substring = alias.morpholized;
                if (substring.isEmpty())
                    continue;

                auto index = m_speech.indexOf(substring);
                if (index < 0)
                    continue;

                correlations[i].append(j);
            }
        }

        int max = -1;
        int maxIndex = -1;
        foreach (auto i, correlations.keys()) {
            auto count = correlations[i].count();
            auto newMaxFound = false;
            if (count > max) {
                newMaxFound = true;
            } else if (count == max) {
                if (getWordCount(maxIndex, correlations[maxIndex]) < getWordCount(i, correlations[i]))
                {
                    newMaxFound = true;
                }
            }

            if (newMaxFound) {
                max = count;
                maxIndex = i;
            }
        }

        m_state = Ready;
        if (maxIndex >= 0)
            emit recognitionSuccess(m_dashboards[maxIndex].url());
        else {
            emit recognitionNotFound();
        }
    }
}

void DashboardService::onSpeechKitError(QString error)
{
    m_state = Ready;
    m_preparingIndex = -1;
    m_aliasIndex = -1;
    m_speech.clear();
    emit recognitionFailed(error);
}
