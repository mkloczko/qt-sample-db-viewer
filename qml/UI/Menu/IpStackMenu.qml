import QtQuick 2.15
import QtQuick.Controls 2.15

import IpStack 0.1

Rectangle {
    id: root
    required property string apiKey;

    Column {
        width: parent.width - 6
        height: parent.height - 6
        anchors.centerIn: parent
        spacing: 3

        Text {
            text: "IpStack API Key:"
            font.pointSize: 10
        }

        TextField {
            width: parent.width
            height:25
            placeholderText: "API Key"
            Component.onCompleted: {
                text = apiKey
                apiKey = Qt.binding(function() {return text})
            }
        }
    }
}
