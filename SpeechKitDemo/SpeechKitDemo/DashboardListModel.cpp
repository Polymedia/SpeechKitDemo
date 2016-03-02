#include "DashboardListModel.h"

#include "DashboardService.h"

DashboardListModel::DashboardListModel(DashboardService *dashboardService, QObject *parent) :
    QAbstractListModel(parent),
    m_dashboardService(dashboardService)
{
    m_roles[NameRole] = "name";
    m_roles[UrlRole] = "url";
    m_roles[AliasesRole] = "aliases";
}

QHash<int, QByteArray> DashboardListModel::roleNames() const
{
    return m_roles;
}

int DashboardListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_dashboardService->count();
}

QVariant DashboardListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_dashboardService->count())
        return QVariant();

    const Dashboard &dashboard = m_dashboardService->at(index.row());
    if (role == NameRole)
        return dashboard.name();
    else if (role == UrlRole)
        return dashboard.url();
    else if (role == AliasesRole) {
        QStringList aliases;
        foreach (auto alias, dashboard.aliases()) {
            aliases.append(alias.original);
        }
        return aliases.join(", ");
    }
    return QVariant();
}

bool DashboardListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row + count > m_dashboardService->count())
        return false;

    int first = row;
    int last = row + count;

    beginRemoveRows(parent, first, last - 1);

    m_dashboardService->remove(first, last);

    endRemoveRows();

    return true;
}

void DashboardListModel::append(const Dashboard &dashboard)
{
    int first = m_dashboardService->count();
    beginInsertRows(QModelIndex(), first, first);

    m_dashboardService->append(dashboard);

    endInsertRows();
}

Dashboard DashboardListModel::toDashboard(const QVariantMap &object) const
{
    Dashboard dashboard;
    dashboard.setName(object["name"].toString());
    dashboard.setUrl(object["url"].toString());

    // Split by comma and trim parts from user input
    QString aliasesString = object["aliases"].toString();
    QStringList aliasesList = aliasesString.split(",", QString::SkipEmptyParts);
    QList<DashboardAlias> aliases;
    for (int i = 0; i < aliasesList.count(); i++) {
        aliases.append(aliasesList[i].trimmed());
    }
    dashboard.setAliases(aliases);

    return dashboard;
}

void DashboardListModel::append(const QVariantMap &object)
{
    append(toDashboard(object));
}

void DashboardListModel::setAt(int index, const QVariantMap &object)
{
    if (index < 0 || index >= m_dashboardService->count())
        return;

    m_dashboardService->setAt(index, toDashboard(object));

    emit dataChanged(createIndex(index, 0), createIndex(index, 0), m_roles.keys().toVector());
}

void DashboardListModel::removeAt(int index)
{
    removeRow(index);
}
