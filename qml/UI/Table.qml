import QtQuick
import QtQuick.Controls
import Qt.labs.qmlmodels 1.0
import Database 0.1
import Utils 0.1

Item {
    id: tableContainer

    required property var model;


    HorizontalHeaderView {
        id: horizontalHeader
        anchors.left: verticalHeaderSpacer.right
        anchors.right: parent.right
        anchors.top: parent.top
        height: 30
        syncView: tableView
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        delegate: Rectangle {
            color:"#AAAAAA"
            implicitHeight: 30
            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                text: display
                leftPadding: 3
            }
        }
    }

    Rectangle {
        id: horizontalHeaderSpacer
        color: "transparent"
        anchors.top: corner.bottom
        anchors.left: corner.left
        anchors.right: corner.right
        height: 1
    }

    Rectangle {
        id: corner
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: verticalHeader.right
        anchors.bottom: horizontalHeader.bottom
        visible: tableView.model !== null
        color: hover ? "gray" : "#AAAAAA"
        property bool hover: false
        Text {
            anchors.centerIn: parent
            text: "Sel."
        }

        MouseArea{
            anchors.fill: parent
            onClicked: Database.tableModel.clearSelected()
            onEntered: parent.hover = true
            onExited: parent.hover = false
            hoverEnabled: true
        }
    }

    Rectangle {
        id: verticalHeaderSpacer
        color: "transparent"
        anchors.top: parent.top
        anchors.bottom: corner.bottom
        anchors.left: verticalHeader.right
        width: 1
    }

    VerticalHeaderView {
        id: verticalHeader
        anchors.left: parent.left
        anchors.top: horizontalHeaderSpacer.bottom
        anchors.bottom: parent.bottom
        width: 30
        syncView: tableView
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        delegate: Rectangle {
            color:"#AAAAAA"
            implicitWidth: 30
            Rectangle {
                anchors.centerIn: parent
                width: 15
                height: 15
                border.width: 1
                border.color: "#656565"
                color: display ? "gray" : "white"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var inserted = tableView.model.setHeaderData(row, Qt.Vertical, !display, Qt.DisplayRole)
                    }
                }
            }
        }
    }

    TableView {
        id: tableView
        anchors.top: horizontalHeaderSpacer.bottom
        anchors.left: verticalHeaderSpacer.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        columnSpacing: 1
        rowSpacing: 1
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        model: parent.model

        selectionModel: ItemSelectionModel {}

        delegate: Rectangle {
            implicitWidth: (column === 0 ? 7 : 2) * Math.max((tableView.width-2) / 11,20)
            implicitHeight: 30
            color: selected | editing | current ? "#EEFFEE" : "#DDDDDD"

            required property bool selected
            required property bool current
            required property bool editing

            Text {
                anchors.verticalCenter: parent.verticalCenter
                leftPadding: 3
                rightPadding: 10
                text: display ? display : "NULL"
            }

            TableView.editDelegate: TextField {
                anchors.fill: parent
                text: display
                leftPadding: 3
                rightPadding: 3
                verticalAlignment: TextInput.AlignVCenter
                Component.onCompleted: {
                    selectAll()
                }

                TableView.onCommit: {
                    //Validation
                    var valid = false;
                    valid = valid || (column === 0 && (Utils.isValidHostname(text) || Utils.isValidAddress(text)));
                    valid = valid || (column === 1 && (Utils.isValidLatitude(text)));
                    valid = valid || (column === 2 && (Utils.isValidLongitude(text)));

                    if (valid === false){
                        console.log("Invalid data")
                        return;
                    }

                    //Push things
                    var index = TableView.view.index(row,column)
                    var inserted = TableView.view.model.setData(index, text, Qt.EditRole)
                    if (inserted === false){
                        return;
                    }
                    var ok = tableView.model.submit()
                    if (ok === false) {
                        console.log("Failed to submit data:", TableView.view.model.lastError())
                        TableView.view.model.revertAll();
                    }
                }
            }
        }
    }
}
