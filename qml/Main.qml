import QtQuick
import QtQuick.Window

import Database 0.1
import IpStack 0.1

import "UI"

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Table {
        id: table
        width: parent.width * 3/4
        anchors.top: parent.top
        anchors.bottom:parent.bottom
        anchors.left: parent.left
        model: Database.tableModel
    }

    Rectangle {
        id: separator
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: table.right
        width: 1
        color: "#AAAAAA"
    }

    Menu {
        id: menu
        width: parent.width /4
        anchors.left: separator.right
        anchors.top: parent.top
        anchors.bottom:parent.bottom

        hostAddress: "127.0.0.1"
        username: "foo"
        password: "bar"
        databaseName: "task"
        tableName: "locations"
        autosubmit: false
        apiKey: ""
    }

    Component.onCompleted: {
        Database.connectedChanged.connect(function(){
            if (Database.connected){
                Database.prepareTableModel(menu.tableName)
            }
        })

        //Connect the ExtendedTableModel with other objects.
        Database.tableModelChanged.connect(function() {
            if(Database.tableModel !== null){
                Database.tableModel.ipStack = IpStack;
                Database.tableModel.editStrategy = Qt.binding(function () {
                    //QSqlTableModel::EditStrategy values:
                    //  0 - OnFieldChange
                    //  1 - OnRowChange
                    //  2 - OnManualSubmit
                    return menu.autosubmit ? 0 : 2
                })
            }
        })

        IpStack.accessKey = Qt.binding(function () {return menu.apiKey;})
    }
}
