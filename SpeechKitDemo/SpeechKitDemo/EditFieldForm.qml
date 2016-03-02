import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import Visiology 1.0

Row {
    id: form
    height: 22

    property alias labelWidth: label.width
    property alias labelText: label.text
    property alias text: textField.text

    Label {
        id: label
        width: 75
        height: parent.height
    }

    TextField {
        id: textField
        width: form.width - label.width
        height: parent.height
     }
}
