#ifndef DASHBOARDLISTMODEL_H
#define DASHBOARDLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "Dashboard.h"

class DashboardService;

class DashboardListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    Q_INVOKABLE void append(const QVariantMap &object);
    Q_INVOKABLE void setAt(int index, const QVariantMap &object);
    Q_INVOKABLE void removeAt(int index);

    enum DashboardRoles {
            NameRole = Qt::UserRole + 1,
            UrlRole,
            AliasesRole
        };

    explicit DashboardListModel(DashboardService *dashboardService, QObject *parent = 0);

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

    void append(const Dashboard &dashboard);

signals:

private:
    Dashboard toDashboard(const QVariantMap &object) const;

private:
    DashboardService *m_dashboardService;

    QHash<int, QByteArray> m_roles;
};

#endif // DASHBOARDLISTMODEL_H
