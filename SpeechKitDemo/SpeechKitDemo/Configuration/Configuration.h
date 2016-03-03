#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QList>

#include "Dashboard.h"

class Logger;
class QJsonObject;
class ConfigurationProperty;

class Configuration : public QObject
{
    Q_OBJECT
public:
    explicit Configuration(Logger *logger, QObject *parent = 0);
    ~Configuration();

    // Dashboards configuration
    QList<Dashboard> dashboards() const;
    void setDashboards(const QList<Dashboard> &dashboards);

    // Key to access yandex services
    QString yandexApiKey() const;

    // ISO 639 language code
    QString language() const;

    // Config version
    QString version() const;

signals:

public slots:

private:
    void registerProperty(ConfigurationProperty *property);
    ConfigurationProperty *findProperty(const QString &name) const;
    void load();
    void save();
    bool loadJson(const QString &path, QJsonObject *result, QString *error) const;
    bool saveJson(const QString &path, const QJsonObject &root, QString *error) const;

private:
    Logger *m_logger;

    QString m_path;
    QString m_defaultPath;
    QList<ConfigurationProperty *> m_properties;
};

#endif // CONFIGURATION_H
