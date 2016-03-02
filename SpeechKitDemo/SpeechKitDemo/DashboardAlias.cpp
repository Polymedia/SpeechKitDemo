#include "DashboardAlias.h"

DashboardAlias::DashboardAlias()
{

}

DashboardAlias::DashboardAlias(const QString &newOriginal) :
    original(newOriginal)
{

}

bool DashboardAlias::operator ==(const DashboardAlias &other)
{
    return (original == other.original) && (morpholized == other.morpholized);
}
