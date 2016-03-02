#include "Dashboard.h"

Dashboard::Dashboard()
{

}

Dashboard::Dashboard(const QString &name, const QString &url, const QList<DashboardAlias> &aliases) :
    m_name(name),
    m_url(url),
    m_aliases(aliases)
{

}

Dashboard::Dashboard(const Dashboard &other)
{
    copy(other);
}

Dashboard &Dashboard::operator =(const Dashboard &other)
{
    copy(other);
    return *this;
}

void Dashboard::copy(const Dashboard &other)
{
    m_name = other.m_name;
    m_url = other.m_url;
    m_aliases = other.m_aliases;
}
