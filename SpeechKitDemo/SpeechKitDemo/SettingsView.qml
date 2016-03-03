import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import Visiology 1.0

Rectangle {
    id: view
    state: "List"

    property var model

    signal closed()

    DashboardListView {
        id: listView
        anchors.fill: parent
        anchors.margins: 5
        model: view.model

        onShowEditView: {
            editView.model = model
            view.state = "Edit"
        }

        onClosed: {
            view.closed()
        }
    }

    DashboardEditView {
        id: editView
        anchors.fill: parent
        anchors.margins: 5

        onClosed: {
            listView.onEditViewClosed(editView.model)
            view.state = "List"
        }
    }

    states: [
        State {
           name: "List"

           PropertyChanges {
               target: listView
               visible: true
           }
           PropertyChanges {
               target: editView
               visible: false
           }
        },
        State {
           name: "Edit"

           PropertyChanges {
               target: listView
               visible: false
           }
           PropertyChanges {
               target: editView
               visible: true
           }
        }
    ]
}

