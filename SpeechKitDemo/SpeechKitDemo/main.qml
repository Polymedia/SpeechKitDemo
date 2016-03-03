import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import Visiology 1.0

Window {
    width: 640
    height: 480
    visible: true

    Rectangle {
        id: mainView
        anchors.fill: parent
        state: "SpeechView"

        SpeechView {
            id: speechView
            anchors.fill: parent
            anchors.margins: 5
            model: MainViewModel

            onSettingsRequested: {
                mainView.state = "SettingsView"
            }
        }

        SettingsView {
            id: settingsView
            anchors.fill: parent
            anchors.margins: 5
            model: MainViewModel

            onClosed: {
                mainView.state = "SpeechView"
            }
        }

        states: [
            State {
               name: "SpeechView"

               PropertyChanges {
                   target: speechView
                   visible: true
               }
               PropertyChanges {
                   target: settingsView
                   visible: false
               }
            },
            State {
               name: "SettingsView"

               PropertyChanges {
                   target: speechView
                   visible: false
               }
               PropertyChanges {
                   target: settingsView
                   visible: true
               }
            }
        ]
    }
}
