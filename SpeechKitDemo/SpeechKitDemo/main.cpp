#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QFile>
#include <QDebug>

#include "ServiceLocator.h"
#include "Configuration/Configuration.h"
#include "MainViewModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    ServiceLocator locator;

    // Load translations
    QTranslator translator;
    if (translator.load(QString(":/translations/%1").arg(locator.configuration()->language())))
        app.installTranslator(&translator);

    // Load main view
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("MainViewModel", locator.mainViewModel());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
