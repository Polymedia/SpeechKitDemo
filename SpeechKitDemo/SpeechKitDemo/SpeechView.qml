import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import Visiology 1.0

Item {
    id: speechView

    property var model

    signal settingsRequested()

    Row {
        id: menuRow
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 5
        layoutDirection: Qt.RightToLeft
        spacing: 5

        Button {
            id: quitButton
            width: 75
            height: 22
            text: qsTr("Quit")
            onClicked: {
                model.quit()
            }
        }

        Button {
            id: settingsButton
            width: 100
            height: 22
            text: qsTr("Settings")
            onClicked: {
                settingsRequested()
            }
        }
    }

    Rectangle {
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: menuRow.bottom
        anchors.margins: 5

        MouseArea {
            anchors.fill: parent
            onClicked: {
                model.toggleState();
            }
        }

        Rectangle {
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.verticalCenter: parent.verticalCenter

            Image {
                id: icon
                x: 0
                y: -95
                width: 128
                height: 128
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                sourceSize.height: 0
                sourceSize.width: 0
                fillMode: Image.Pad
                source: "qrc:/images/mic_ready"
            }

            Text {
                id: message
                y: 39
                height: 16
                elide: Text.ElideMiddle
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Ready to listen")
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
            }

            Button {
                id: cancelButton
                x: 0
                y: 61
                width: 100
                height: 22
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                visible: MainViewModel.isCancellationEnabled
                text: qsTr("Cancel")
                onClicked: MainViewModel.cancel()
            }
        }

        Connections {
            target: model
            onStateChanged: {
                if (state == MainViewModelType.Ready) {
                    message.text = qsTr("Ready to listen");
                } else if (state == MainViewModelType.Listening) {
                    message.text = qsTr("Speak") + "...";
                } else if (state == MainViewModelType.Recognizing) {
                    message.text = qsTr("Recognizing")  + "...";
                } else if (state == MainViewModelType.Finished) {
                    message.text = qsTr("Finished");
                } else if (state == MainViewModelType.FinishedNotFound) {
                    message.text = qsTr("You query has been sent to analytics. When it is finished, you will get a notification");
                } else if (state == MainViewModelType.Error) {
                    message.text = qsTr("Error") + ": " + MainViewModel.error;
                }
            }
        }
    }
}
