#ifndef CONFIGURATIONPROPERTY_H
#define CONFIGURATIONPROPERTY_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>

class QJsonValue;

class ConfigurationProperty : public QObject
{
    Q_OBJECT
public:
    explicit ConfigurationProperty(const QString &name, const QVariant &defaultValue, QObject *parent = 0);

    QString name() const;

    virtual void load(const QJsonObject &config, const QJsonObject &defaultConfig);
    virtual void save(QJsonObject *config) const;

    virtual QVariant value() const;
    virtual void setValue(const QVariant &value);

protected:
    bool findValue(const QJsonObject &config, const QJsonObject &defaultConfig, const QString &name, QJsonValue *result) const;

private:
    QString m_name;
    QVariant m_defaultValue;
    QVariant m_value;
};

#endif // CONFIGURATIONPROPERTY_H
