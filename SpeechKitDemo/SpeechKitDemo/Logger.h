#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);

    void info(const QString &info);
    void warning(const QString &info);
    void error(const QString &info);

signals:

public slots:
};

#endif // LOGGER_H
