#ifndef DASHBOARDSPROPERTY_H
#define DASHBOARDSPROPERTY_H

#include "ConfigurationProperty.h"
#include "../Dashboard.h"

class DashboardsProperty : public ConfigurationProperty
{
    Q_OBJECT
public:
    explicit DashboardsProperty(const QString &name, const QList<Dashboard> &defaultDashboards, QObject *parent = 0);

    virtual void load(const QJsonObject &config, const QJsonObject &defaultConfig);
    virtual void save(QJsonObject *config) const;

signals:

public slots:
};

#endif // DASHBOARDSPROPERTY_H
