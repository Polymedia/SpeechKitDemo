#include "Configuration.h"

#include <QGuiApplication>
#include <QFile>
#include <QMetaObject>
#include <QMetaProperty>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

#include "../Logger.h"
#include "../DashboardAlias.h"
#include "ConfigurationProperty.h"
#include "DashboardsProperty.h"

Configuration::Configuration(Logger *logger, QObject *parent) :
    QObject(parent),
    m_logger(logger)
{
    m_defaultPath = ":/defaults/config.json";

#ifdef IOS
    m_path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/config.json";
#else
    m_path = "config.json";
#endif

    registerProperty(new ConfigurationProperty("version", "1.0.0.0"));
    registerProperty(new ConfigurationProperty("language", "ru"));
    registerProperty(new DashboardsProperty("dashboards", QList<Dashboard>()));

    // Sync configuration
    load();
    m_logger->info("Synchonizing configuration");
    save();
}

Configuration::~Configuration()
{
    foreach (auto property, m_properties) {
        property->deleteLater();
    }
    m_properties.clear();
}

QList<Dashboard> Configuration::dashboards() const
{
    return findProperty("dashboards")->value().value<QList<Dashboard> >();
}

void Configuration::setDashboards(const QList<Dashboard> &dashboards)
{
    findProperty("dashboards")->setValue(QVariant::fromValue(dashboards));
    save();
}

QString Configuration::yandexApiKey() const
{
    return "61caa17f-f079-4d7d-b2b6-3278bc3a21ac";
}

QString Configuration::language() const
{
    return findProperty("language")->value().toString();
}

QString Configuration::version() const
{
    return findProperty("version")->value().toString();
}

void Configuration::registerProperty(ConfigurationProperty *property)
{
    m_properties.append(property);
}

ConfigurationProperty *Configuration::findProperty(const QString &name) const
{
    foreach (auto property, m_properties) {
        if (property->name() == name)
            return property;
    }

    return nullptr;
}

void Configuration::load()
{
    QJsonObject configObject, defaultConfigObject;
    QString error;

    if (!loadJson(m_path, &configObject, &error)) {
        m_logger->warning(QString("Can't load configuration(%1): %2").arg(m_path).arg(error));
    }
    if (!loadJson(m_defaultPath, &defaultConfigObject, &error)) {
        m_logger->error(QString("Can't load DEFAULT configuration(%1): %2").arg(m_defaultPath).arg(error));
        return;
    }

    // Load properties values
    foreach (auto property, m_properties) {
        property->load(configObject, defaultConfigObject);
    }
}

void Configuration::save()
{
    QJsonObject currentConfig;

    foreach (auto property, m_properties) {
        property->save(&currentConfig);
    }

    QString error;
    if (!saveJson(m_path, currentConfig, &error)) {
        m_logger->error(QString("Can't save configuration: %1").arg(error));
    }
}

bool Configuration::loadJson(const QString &path, QJsonObject *result, QString *error) const
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        *error = QString("can't open file for reading - %1").arg(file.errorString());
        return false;
    }

    QByteArray data = file.readAll();

    QJsonParseError jsonError;
    QJsonDocument json = QJsonDocument::fromJson(data, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        *error = QString("configuration file is corrupted - json error: %1").arg(jsonError.errorString());
        return false;
    }

   *result = json.object();

    return true;
}

bool Configuration::saveJson(const QString &path, const QJsonObject &root, QString *error) const
{
    // Open file for writing
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        *error = QString("can't open file for writing - %1").arg(file.errorString());
        return false;
    }

    // Write json to file
    QJsonDocument json(root);
#ifdef DEBUG
    file.write(json.toJson(QJsonDocument::Indented));
#else
    file.write(json.toJson(QJsonDocument::Compact));
#endif

    return true;
}
