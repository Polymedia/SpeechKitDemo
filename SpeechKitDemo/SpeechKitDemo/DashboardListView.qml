import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import Visiology 1.0

Item {
    id: view

    property var model

    signal showEditView(var model)
    signal closed()

    function onEditViewClosed(model)
    {
        if (model.index < 0)
            view.model.dashboards.append(model.dashboard)
        else
            view.model.dashboards.setAt(model.index, model.dashboard)
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

            text: qsTr("Dashboards settings")
        }
    }

    Rectangle {
        id: table
        anchors.top: menuRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 15
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.bottomMargin: 5

        Row {
            id: columns
            height: 22
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 5

            // 200 = 100 (Edit button width) + 100 (Remove button width)

            Text {
                width: (table.width - 200) / 3
                height: columns.height
                clip: true
                wrapMode: Text.WordWrap
                text: qsTr("Name")
            }

            Text {
                width: (table.width - 200) / 3
                height: columns.height
                clip: true
                wrapMode: Text.WrapAnywhere
                text: qsTr("Url")
            }

            Text {
                width: (table.width - 200) / 3
                height: columns.height
                clip: true
                wrapMode: Text.WordWrap
                text: qsTr("Aliases")
            }
        }

        ScrollView {
            id: listScrollView
            anchors.top: columns.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: addButton.top
            anchors.topMargin: 5

            ListView {
                id: dashboardsList
                spacing: 5
                model: view.model.dashboards
                delegate:
                    Rectangle {
                        id: item
                        width: listScrollView.width
                        height: 75

                        // 240 = 100 (Edit button width) + 100 (Remove button width) + 20 (spacing) + 20 (scrollbar)

                        Row {
                            anchors.fill: parent
                            spacing: 5
                            Text {
                                width: (listScrollView.width - 240) / 3
                                height: item.height
                                clip: true
                                wrapMode: Text.WordWrap
                                text: name
                            }

                            Text {
                                width: (listScrollView.width - 240) / 3
                                height: item.height
                                clip: true
                                wrapMode: Text.WrapAnywhere
                                text: url
                            }

                            Text {
                                width: (listScrollView.width - 240) / 3
                                height: item.height
                                clip: true
                                wrapMode: Text.WordWrap
                                text: aliases
                            }

                            Button {
                                width: 100
                                height: 22
                                text: qsTr("Edit")
                                onClicked: {
                                    var obj = {
                                        index: index,
                                        dashboard: {
                                            name: name,
                                            url: url,
                                            aliases: aliases,
                                        }
                                    }

                                    view.showEditView(obj)
                                }
                            }

                            Button {
                                width: 100
                                height: 22
                                text: qsTr("Remove")
                                onClicked: {
                                    view.model.dashboards.removeAt(index)
                                }
                            }
                        }
                    }
            }
        }

        Button {
            id: addButton
            width: 100
            height: 22
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.topMargin: 5
            anchors.leftMargin: 10
            text: qsTr("Add")
            onClicked: {
                var obj = {
                    index: -1,
                    dashboard: {
                        name: qsTr("New dashboard"),
                        url: qsTr("http://visiology.com/"),
                        aliases: qsTr("visiology, polymedia"),
                    }
                }

                view.showEditView(obj)
            }
        }
    }
}
