#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QVariantMap>

#include "DashboardAlias.h"

class Dashboard : public QObject
{
    Q_OBJECT
public:
    Dashboard();
    Dashboard(const QString &name, const QString &url, const QList<DashboardAlias> &aliases);
    Dashboard(const Dashboard &other);

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QList<DashboardAlias> aliases READ aliases WRITE setAliases NOTIFY aliasesChanged)

    QString name() const { return m_name; }
    void setName(const QString &name) {
        if (m_name != name) {
            m_name = name;
            emit nameChanged(name);
        }
    }

    QString url() const { return m_url; }
    void setUrl(const QString &url) {
        if (m_url != url) {
            m_url = url;
            emit urlChanged(url);
        }
    }

    QList<DashboardAlias> aliases() const { return m_aliases; }
    void setAliases(const QList<DashboardAlias> &aliases) {
        if (m_aliases != aliases) {
            m_aliases = aliases;
            emit aliasesChanged(aliases);
        }
    }

    Dashboard &operator = (const Dashboard &other);

signals:
    void nameChanged(QString name);
    void urlChanged(QString url);
    void aliasesChanged(QList<DashboardAlias> aliases);

private:
    void copy(const Dashboard &other);

private:
    QString m_name;
    QString m_url;
    QList<DashboardAlias> m_aliases;
};

Q_DECLARE_METATYPE(Dashboard)

#endif // DASHBOARD_H
