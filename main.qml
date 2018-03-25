import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import Data 1.0
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
            color: "#000"
            id: contentsArea
            text: notesModel.textPane;

            renderType: Text.NativeRendering
            textFormat: Text.RichText
            font.family: "Arial"

            height: parent.height

            selectByMouse: true
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.pointSize: 14

            onTextChanged: {
                if(notesView.currentIndex >= 0)
                    notesModel.get(notesView.currentIndex).note = text
            }
        }
    }

    Rectangle {
        id: rectangle
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
            anchors.bottomMargin: 25
            model: notesModel
            anchors.fill: parent
            highlight: Rectangle { color: "#e6e6e6" }

            delegate: ItemDelegate {
                width: parent.width
                text: model.textDescription

                onClicked: {
                    notesView.currentIndex = index
                }
            }
        }

        // Нижний бар с кнопками
        Rectangle {
            id: rectangle1
            height: 25
            color: "#aaa"
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0

            // Кнопка добавления заметки
            ItemDelegate {
                id: addButton
                width: 40
                height: 25
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                highlighted: false
                padding: 0

                Text {
                    anchors.centerIn: parent
                    text: "+"
                    font.pointSize: 12
                }

                // Добавление новой заметки
                onClicked: {
                    dataList.appendItem()
//                    notesView.currentIndex = notesModel.rowCount()-1;
                }
            }

            // Кнопка удления заметки
            ItemDelegate {
                id: removeButton
                width: 40
                height: 25
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.left: addButton.right
                anchors.leftMargin: 0
                padding: 0
                highlighted: false

                Text {
                    anchors.centerIn: parent
                    text: "−"
                    font.pointSize: 12
                }

                // Удаление текущей выбранной заметки
                onClicked: {
                    notesModel.remove(notesView.currentIndex, 1);
                    notesView.currentIndex = -1;
                }
            }

            // Кнопка Меню
            ItemDelegate {
                id: optionsButton
                x: -1
                y: 6
                width: 40
                height: 25
                anchors.right: parent.right
                anchors.rightMargin: 0
                padding: 0
                highlighted: false
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0

                Text {
                    anchors.centerIn: parent
                    text: "⋯"
                    font.pointSize: 18
                }

                onClicked: {
                    optionsMenu.open()
                }
            }

            // Само меню
            Menu {
                id: optionsMenu

                MenuItem {
                    text: "About"
                }
            }
        }
    }

    DataModel {
        id: notesModel
        list: dataList

//        ListElement { name: "note 1"; type: "note"; note: "this is note 1" }
//        ListElement { name: "note 2"; type: "note"; note: "this is note 2" }
//        ListElement { name: "note 3"; type: "note"; note: "this is note 3" }
    }

}
