import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import Visiology 1.0

Item {
    id: view

    property var model

    signal closed()

    onModelChanged: {
        editFormName.text = model.dashboard.name
        editFormUrl.text = model.dashboard.url
        editFormAliases.text = model.dashboard.aliases
    }

    Item {
        id: menuRow
        height: 22
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 5

        Button {
            id: backButton
            width: 50
            height: parent.height
            anchors.left: parent.left
            text: qsTr("Back")
            onClicked: {
                model.dashboard.name = editFormName.text
                model.dashboard.url = editFormUrl.text
                model.dashboard.aliases = editFormAliases.text

                closed()
            }
        }

        Text {
            id: headerText
            height: parent.height
            anchors.top: parent.top
            anchors.left: backButton.right
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            clip: true

            text: qsTr("Dashboard edit")
        }
    }

    Rectangle {
        id: form
        anchors.top: menuRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 15
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.bottomMargin: 5

        Column {
            anchors.fill: parent
            spacing: 15

            EditFieldForm {
                id: editFormName
                width: parent.width
                labelText: qsTr("Name") + ": "
            }

            EditFieldForm {
                id: editFormUrl
                width: parent.width
                labelText: qsTr("Url") + ": "
            }

            EditFieldForm {
                id: editFormAliases
                width: parent.width
                labelText: qsTr("Aliases") + ": "
            }
        }
    }
}
