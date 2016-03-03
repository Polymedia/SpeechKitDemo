TEMPLATE = app

QT += qml quick multimedia
CONFIG += c++11

CONFIG(debug, debug | release) {
    DEFINES += DEBUG
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    MainViewModel.h \
    Dashboard.h \
    DashboardListModel.h \
    ServiceLocator.h \
    Logger.h \
    DashboardService.h \
    AudioRecorder.h \
    SpeechKitService.h \
    DashboardAlias.h \
    Configuration/Configuration.h \
    Configuration/ConfigurationProperty.h \
    Configuration/DashboardsProperty.h

SOURCES += main.cpp \
    MainViewModel.cpp \
    Dashboard.cpp \
    DashboardListModel.cpp \
    ServiceLocator.cpp \
    Logger.cpp \
    DashboardService.cpp \
    AudioRecorder.cpp \
    SpeechKitService.cpp \
    DashboardAlias.cpp \
    Configuration/Configuration.cpp \
    Configuration/ConfigurationProperty.cpp \
    Configuration/DashboardsProperty.cpp

RESOURCES += \
    Resources.qrc

DISTFILES +=

TRANSLATIONS = \
    SpeechKitDemo_ru.ts

lupdate_only {
    SOURCES += *.qml
}

mac {
    DEFINES += MACOSX
}

ios {
    DEFINES += IOS
}
