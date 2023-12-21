import QtQuick 2.15
import QtQuick.Controls 2.15
import Database 0.1
import IpStack 0.1

Rectangle {
    //Basic operations:
    //   Add, Remove, Get Locations, Submit All (selected? maybe later)
    id: root
    required property bool autosubmit;

    Column {
        width: parent.width - 6
        height: parent.height - 6
        anchors.centerIn: parent
        spacing: 3

        Button {
            width: parent.width
            text: enabled ? "Add" : "<font color='gray'>Add</font>"
            enabled: Database.connected && Database.tableModel && Database.tableModel.editStrategy === 2
            onClicked: {
                Database.tableModel.insertEmptyRecord(0);
            }
        }


        Button {
            width: parent.width
            text: enabled ? "Remove" : "<font color='gray'>Remove</font>"
            enabled: Database.connected && Database.tableModel && Database.tableModel.editStrategy === 2
            onClicked: {
                Database.tableModel.removeSelected();
            }
        }

        Button {
            width: parent.width
            text: enabled ? "Get Locations" : "<font color='gray'>Get Locations</font>"
            property bool toggle: false
            enabled: Database.connected
            onClicked: {
                Database.tableModel.updateCoordinatesSelected()
            }
        }

        Rectangle {
            width: parent.width-15
            height: 1
            color: "#AAAAAA"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            width: parent.width
            spacing: 3

            Button {
                width: (parent.width - 3)/2
                text: enabled ? "Revert" : "<font color='gray'>Revert<font>"
                enabled: Database.connected
                onClicked: {
                    Database.tableModel.revertAll()
                    console.log("Reverted all changes")
                }
            }

            Button {
                width: (parent.width - 3)/2
                text: enabled ? "Submit" : "<font color='gray'>Submit<font>"
                enabled: Database.connected
                onClicked: {
                    var submitted = Database.tableModel.submitAll()
                    if (submitted) {
                        console.log("Successfully submitted all data.")
                    } else {
                        console.log("Error:", Database.tableModel.lastError())
                    }
                }
            }
        }

        CheckBox {
            id: autosubmit_box
            text: "Autosubmit"
            Component.onCompleted: {
                checked = root.autosubmit
                root.autosubmit = Qt.binding(function() {return checked})
            }
        }

    }
}
