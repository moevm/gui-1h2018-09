import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Window {
    id: window
    visible: true
    width: 840
    height: 480
    title: qsTr("marxx")

    property var nm: notesModel

    ScrollView {
        id: scrollView
        rightPadding: 5
        leftPadding: 5
        anchors.bottomMargin: -12
        anchors.leftMargin: 200
        anchors.fill: parent

        TextArea {
            id: contentsArea
            text: notesModel.get(notesView.currentIndex).note
            renderType: Text.NativeRendering
            textFormat: Text.RichText
            font.family: "Arial"

            height: parent.height

            selectByMouse: true
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.pointSize: 14

            onTextChanged: nm.get(notesView.currentIndex).note = text
        }
    }

    Rectangle {
        color: "#f6f6f6"

        width: 200
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        ListView {
            id: notesView
            anchors.bottomMargin: 0
            model: notesModel
            anchors.fill: parent
            highlight: Rectangle { color: "#e6e6e6" }

            delegate: ItemDelegate {
                width: parent.width
                text: name

                onClicked: {
                    notesView.currentIndex = index
                }
            }
        }
    }



    RoundButton {
        id: addNoteButton
        x: 10
        y: 402
        text: "+"
        highlighted: true
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10

        onClicked: {
            notesModel.append( { "name": "New note", "type": "note", "note": "" } );
            notesView.currentIndex = notesModel.rowCount()-1;
        }
    }

    ListModel {
        id: notesModel

        ListElement { name: "note 1"; type: "note"; note: "this is note 1" }
        ListElement { name: "note 2"; type: "note"; note: "this is note 2" }
        ListElement { name: "note 3"; type: "note"; note: "this is note 3" }
    }

}
