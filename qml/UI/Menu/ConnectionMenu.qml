import QtQuick 2.15
import QtQuick.Controls 2.15
import Database

//Connection info
Rectangle {
    id: root
    required property string hostAddress;
    required property string username;
    required property string password;
    required property string databaseName;
    required property string tableName;

    Column {
        width: parent.width - 6
        height: parent.height - 6
        anchors.centerIn: parent
        spacing: 3

        Row {
            width: parent.width
            spacing: 3

            Text {
                text: "Connection status:"
                font.pointSize: 10
            }

            Rectangle {
                x: parent.width - width
                width: 15
                height: 15
                color: {
                    if (Database.connected === false){
                        "gray"
                    } else if (Database.pingStatus) {
                        "green"
                    } else {
                        "yellow"
                    }
                }

            }
        }

        TextField {
            id: address_field
            width: parent.width
            height:25
            placeholderText: "Database address"
            Component.onCompleted: {
                text = hostAddress
                hostAddress = Qt.binding(function() {return text})
            }
        }

        TextField {
            id: database_field
            width: parent.width
            height:25
            placeholderText: "Database name"
            Component.onCompleted: {
                text = databaseName
                databaseName = Qt.binding(function() {return text})
            }
        }

        TextField {
            id: table_field
            width: parent.width
            height:25
            placeholderText: "Table name"
            Component.onCompleted: {
                text = tableName
                tableName = Qt.binding(function() {return text})
            }
        }

        Row {
            width: parent.width
            spacing: 3
            TextField {
                id: user_field
                width: (parent.width-3)/2
                height:25
                placeholderText: "Username"
                Component.onCompleted: {
                    text = username
                    username = Qt.binding(function() {return text})
                }
            }

            TextField {
                id: password_field
                width: (parent.width-3)/2
                height:25
                echoMode: TextInput.Password
                placeholderText: "Password"
                Component.onCompleted: {
                    text = password
                    password = Qt.binding(function() {return text})
                }
            }
        }

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 15
            height: 1
            color: "#AAAAAA"
        }

        Button {
            width: parent.width
            text: Database.connected ? "Disconnect" : "Connect"
            onClicked: {
                if (Database.connected) {
                    Database.disconnect()
                } else {
                    Database.connect(hostAddress, databaseName, username, password);
                }
            }
        }
    }
}
