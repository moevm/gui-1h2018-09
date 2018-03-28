import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: window
    visible: true
    width: 840
    height: 480
    title: qsTr("marxx")

    onClosing: {
        notesModel.saveToDefault()
    }

    ScrollView {
        id: scrollView
        rightPadding: 5
        leftPadding: 5
        anchors.bottomMargin: -12
        anchors.leftMargin: 200
        anchors.fill: parent

        TextArea {
            id: contentsArea
            text: {
                if(notesView.currentIndex >= 0)
                    return notesModel.data(notesModel.index(notesView.currentIndex, 0), 258)
                else
                    return ""
            }
            renderType: Text.NativeRendering
            textFormat: Text.RichText
            font.family: "Arial"

            height: parent.height

            selectByMouse: true
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.pointSize: 14

            onTextChanged: {
                if(notesView.currentIndex >= 0) {
                    notesModel.setData(notesModel.index(notesView.currentIndex, 0), text, 258)

                    var txt = contentsArea.getText(0, contentsArea.length > 30 ? 30 : contentsArea.length)
                    //console.log(contentsArea.text)
                    notesModel.setData(notesModel.index(notesView.currentIndex, 0), txt == "" ? "New note" : txt, 257)
                }


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
                clip: true
                text: name

                onClicked: {
                    notesView.currentIndex = index
                }
            }
        }

        // Нижний бар с кнопками
        Rectangle {
            id: rectangle1
            height: 25
            color: "#e6e6e6"
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
                    notesModel.append("New Note", "");
                    notesView.currentIndex = notesModel.rows()-1;
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
                    notesModel.remove(notesView.currentIndex);
                    notesView.currentIndex = 0;
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
                    text: "Export note..."
                    onTriggered: fileSaveNote.open()
                }

                MenuSeparator { }

                MenuItem {
                    text: "Import library..."
                    onTriggered: fileOpenDialog.open()
                }

                MenuItem {
                    text: "Export library..."
                    onTriggered: fileSaveDialog.open()
                }

                MenuSeparator { }

                MenuItem {
                    text: "About"
                }
            }
        }

    }

    FileDialog {
          id: fileSaveNote
          title: "Choose a path"
          selectExisting: false
          nameFilters: ["Marxx notes (*.md)"]
          onAccepted: saveNote(fileSaveNote.fileUrl,contentsArea.getText(0,contentsArea.length-1))
        }

    FileDialog {
        id: fileOpenDialog
        title: "Choose a library"
        folder: shortcuts.home
        onAccepted: importLibrary(fileOpenDialog.fileUrl)
    }

    FileDialog {
        id: fileSaveDialog
        title: "Choose a path"
        selectExisting: false
        nameFilters: ["Marxx library (*.marxxlib)"]
        onAccepted: exportLibrary(fileSaveDialog.fileUrl)
    }

    function importLibrary(fileUrl) {
        notesModel.unpack(fileUrl, true)

        // Обновляем вид списка после обновления модели
        notesView.model = 0;
        notesView.model = notesModel
    }

    function exportLibrary(fileUrl) {
        console.log(fileUrl);
        notesModel.pack(fileUrl, true)
    }

    function saveNote(fileUrl, text) {
            console.log(fileUrl
                        );
            var request = new XMLHttpRequest();
            request.open("PUT", fileUrl, false);
            request.send(text);
            return request.status;
        }



}
