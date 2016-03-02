#include "ConfigurationProperty.h"

#include <QJsonValue>

ConfigurationProperty::ConfigurationProperty(const QString &name, const QVariant &defaultValue, QObject *parent) :
    QObject(parent),
    m_name(name),
    m_defaultValue(defaultValue),
    m_value(defaultValue)
{

}

QString ConfigurationProperty::name() const
{
    return m_name;
}

void ConfigurationProperty::load(const QJsonObject &config, const QJsonObject &defaultConfig)
{
    QJsonValue jsonValue;
    if (findValue(config, defaultConfig, m_name, &jsonValue)) {
        m_value = jsonValue.toVariant();
    } else {
        m_value = m_defaultValue;
    }
}

void ConfigurationProperty::save(QJsonObject *config) const
{
    config->operator [](m_name) = QJsonValue::fromVariant(m_value);
}

QVariant ConfigurationProperty::value() const
{
    return m_value;
}

void ConfigurationProperty::setValue(const QVariant &value)
{
    m_value = value;
}

bool ConfigurationProperty::findValue(const QJsonObject &config, const QJsonObject &defaultConfig, const QString &name, QJsonValue *result) const
{
    if (config.contains(name)) {
        *result = config[name];
        return true;
    } else if (defaultConfig.contains(name)) {
        *result = defaultConfig[name];
        return true;
    }

    return false;
}
