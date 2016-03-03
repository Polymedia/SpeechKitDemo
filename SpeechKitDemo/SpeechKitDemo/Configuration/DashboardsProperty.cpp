#include "DashboardsProperty.h"

#include <QJsonArray>

#include "../Dashboard.h"

DashboardsProperty::DashboardsProperty(const QString &name, const QList<Dashboard> &defaultDashboards, QObject *parent) :
    ConfigurationProperty(name, QVariant::fromValue(defaultDashboards), parent)
{

}

void DashboardsProperty::load(const QJsonObject &config, const QJsonObject &defaultConfig)
{
    // Find value in configs
    QJsonValue dashboardsValue;
    if (!findValue(config, defaultConfig, name(), &dashboardsValue))
        return;

    // Load dashboards
    QList<Dashboard> dashboards;
    auto dashboardsArray = dashboardsValue.toArray();
    foreach (QJsonValue dashboardValue, dashboardsArray) {
        auto dashboardObject = dashboardValue.toObject();

        Dashboard dashboard;
        dashboard.setName(dashboardObject["name"].toString());
        dashboard.setUrl(dashboardObject["url"].toString());

        auto aliasesArray = dashboardObject["aliases"].toArray();
        QList<DashboardAlias> aliasesList;
        foreach (QJsonValue aliasValue, aliasesArray) {
            DashboardAlias alias;
            auto aliasObject = aliasValue.toObject();
            alias.original = aliasObject["original"].toString();
            alias.morpholized = aliasObject["morpholized"].toString();
            aliasesList.append(alias);
        }
        dashboard.setAliases(aliasesList);

        dashboards.append(dashboard);
    }

    setValue(QVariant::fromValue(dashboards));
}

void DashboardsProperty::save(QJsonObject *config) const
{
    auto dashboards = value().value<QList<Dashboard> >();

    // Save dashboards
    QJsonArray dashboardsArray;
    foreach (const Dashboard &dashboard, dashboards) {
        QJsonObject dashboardObject;
        dashboardObject["name"] = dashboard.name();
        dashboardObject["url"] = dashboard.url();

        QJsonArray aliasesArray;
        foreach (DashboardAlias alias, dashboard.aliases()) {
            QJsonObject aliasObject;
            aliasObject["original"] = alias.original;
            aliasObject["morpholized"] = alias.morpholized;
            aliasesArray.append(aliasObject);
        }
        dashboardObject["aliases"] = aliasesArray;

        dashboardsArray.append(dashboardObject);
    }

    config->operator [](name()) = dashboardsArray;
}
