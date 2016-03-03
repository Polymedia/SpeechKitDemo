#ifndef DASHBOARDALIAS_H
#define DASHBOARDALIAS_H

#include <QString>

class DashboardAlias
{
public:
    DashboardAlias();
    DashboardAlias(const QString &newOriginal);

    bool operator == (const DashboardAlias &other);

    QString original;
    QString morpholized;
};

#endif // DASHBOARDALIAS_H
