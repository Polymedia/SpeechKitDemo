#include <QString>
#include <QtTest>

//#include "../../SpeechKitDemo/DashboardService.h"

class DashboardRecognizerTest : public QObject
{
    Q_OBJECT

public:
    DashboardRecognizerTest();

private Q_SLOTS:
//    void recognize_data();
//    void recognize();
};

DashboardRecognizerTest::DashboardRecognizerTest()
{
}

//void DashboardRecognizerTest::recognize_data()
//{
//    QTest::addColumn<QList<Dashboard> >("dashboards");
//    QTest::addColumn<QString>("speech");
//    QTest::addColumn<QUrl>("expected");

//    QList<Dashboard> dashboards;
//    QStringList testAliases;
//    QString testUrl;

//    // exact match
//    testAliases.append("обращения граждан");
//    testAliases.append("обращение граждан");
//    testAliases.append("общественная приемная");
//    testUrl = "http://192.168.46.140/society/obrashcheniya-grazhdan/obrashcheniya-grazhdan";
//    dashboards.append(Dashboard("1", testUrl, testAliases));

//    testAliases.clear();
//    testAliases.append("опрос");
//    testAliases.append("социология");
//    testUrl = "http://192.168.46.140/society/oprosy-obshchestvennogo-mneniya";
//    dashboards.append(Dashboard("2", testUrl, testAliases));

//    QTest::newRow("exact match") << dashboards << "социология" << testUrl;
//}

//void DashboardRecognizerTest::recognize()
//{
//    // Arrange
//    QFETCH(QList<Dashboard>, dashboards);
//    QFETCH(QString, speech);
//    QFETCH(QUrl, expected);

//    // Act
//    DashboardService recognizer(nullptr);
//    QUrl actual = recognizer.recognize(speech);

//    // Assert
//    QCOMPARE(actual, expected);
//}

QTEST_APPLESS_MAIN(DashboardRecognizerTest)

#include "tst_DashboardRecognizerTest.moc"
