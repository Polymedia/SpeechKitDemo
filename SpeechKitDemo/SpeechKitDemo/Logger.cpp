#include "Logger.h"

#include <QtDebug>

Logger::Logger(QObject *parent) : QObject(parent)
{

}

void Logger::info(const QString &message)
{
    qWarning() << "[Info] " << message;
}

void Logger::warning(const QString &message)
{
    qWarning() << "[Warning] " << message;
}

void Logger::error(const QString &message)
{
    qCritical() << "[Error] " << message;
}
