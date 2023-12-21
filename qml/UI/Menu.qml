import QtQuick 2.15
import QtQuick.Controls 2.15

import "Menu"

Item {
    id: root
    required property string hostAddress;
    required property string username;
    required property string password;
    required property string databaseName;
    required property string tableName;

    required property bool autosubmit;

    required property string apiKey;
    Column {
        anchors.fill: parent

        ConnectionMenu {
            width: parent.width
            height: 180
            color: "#BBBBBB"

            hostAddress: root.hostAddress
            username: root.username
            password: root.password
            databaseName: root.databaseName
            tableName: root.tableName

            Component.onCompleted: {
                root.tableName = Qt.binding(function() {return tableName});
            }
        }

        RecordMenu {
            width: parent.width
            height: 160
            color: "#BBBBBB"

            autosubmit: root.autosubmit
            Component.onCompleted: {
                root.autosubmit = Qt.binding(function(){return autosubmit})
            }
        }

        IpStackMenu {
            width: parent.width
            height: 160
            color: "#BBBBBB"

            apiKey: root.apiKey
        }
    }
}
