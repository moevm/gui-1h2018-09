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

    function updateContentText() {
        var updated = ""
        if(notesView.currentIndex >= 0)
            updated = notesModel.data(notesModel.index(notesView.currentIndex, 0), 258)

        contentsArea.text = updated
        return updated
    }

    function highlightSyntax(str) {
        var mu = str;
        const codeBlockRegex = /```[a-z]*[\s\S]*?```/g;
        const inlineCodeRegex = /(`)(.*?)\1/g;
        const linkRegex = /\[([^\[]+)\]\(([^\)]+)\)/g;
        const headingRegex = /\n(#+\s*)(.*)/g;
        const strikethroughRegex = /(\~\~)(.*?)\1/g;
        const horizontalRuleRegex = /((\-{3,})|(={3,}))/g;
        const unorderedListRegex = /(\s*(\-|\+)\s.*)+/g;
        const orderedListRegex = /(\s*([0-9]+\.)\s.{5,})+/g;
        const paragraphRegex = /\n+(?!<pre>)(?!<h)(?!<ul>)(?!<blockquote)(?!<hr)(?!\t)([^\n]+)\n/g;
        const boldRegex = /(\*([^*\n])+\*)/g;
        const italicRegex = /(\_([^*\n])+\_)/g;
        const header_1 = /^(# .+)$/gm;
        const header_2 = /^(## .+)$/gm;
        const header_3 = /^(### .+)$/gm;
        const header_4 = /^(#### .+)$/gm;
        const header_5 = /^(##### .+)$/gm;
        const header_6 = /^(###### .+)$/gm;

        mu = mu.replace(boldRegex, function(str) {
            return "<b>" + str + "</b>"
        })

        mu = mu.replace(italicRegex, function(str) {
            return "<i>" + str + "</i>"
        })

        mu = mu.replace(header_1, function(str) {
            return "<span style='color: #333333; font-size: 30px;'>" + str + "</span>"
        })
        mu = mu.replace(header_2, function(str) {
            return "<span style='color: #333333; font-size: 26px;'>" + str + "</span>"
        })
        mu = mu.replace(header_3, function(str) {
            return "<span style='color: #333333; font-size: 22px;'>" + str + "</span>"
        })

        mu = mu.replace(header_4, function(str) {
            return "<span style='color: #333333; font-size: 20px;'>" + str + "</span>"
        })

        mu = mu.replace(header_5, function(str) {
            return "<span style='color: #333333; font-size: 18px;'>" + str + "</span>"
        })

        mu = mu.replace(header_6, function(str) {
            return "<span style='color: #333333; font-size: 16px;'>" + str + "</span>"
        })

        mu = mu.replace(linkRegex, function(str) {
            return "<span style='color:#a485ad;'>" + str + "</span>"
        })

        mu = mu.replace(strikethroughRegex , function(str) {
            return '<span style="color: black; font-size: 22px; text-decoration:line-through" >' + str + '</span>';
        })

        mu = mu.replace(codeBlockRegex, function(str) {
            return '<span style="color: black; background-color:#c1e0b8;  font-family: Courier New, sans-serif;">' + str + '</span>';
        })

        mu = mu.replace(horizontalRuleRegex , function(str) {
            return "<span style='color: blue;'>" + str + "</span>";
        })

        mu = mu.replace(orderedListRegex , function(str) {
                return '<span style="color: black; font-family:Arial ; font-weight:500; ">' + str + '</span>';
        })

        mu = mu.replace(unorderedListRegex , function(str) {
            return '<span style="color: black; font-family:Arial ; font-weight:500;">' + str + '</span>';
         })

        return mu;
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
            }
            renderType: Text.NativeRendering
            textFormat: Text.RichText
            font.family: "Verdana"

            height: parent.height

            selectByMouse: true
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.pointSize: 14

            property bool processing: false
            onTextChanged: {
                if(notesView.currentIndex >= 0) {
                    notesModel.setData(notesModel.index(notesView.currentIndex, 0), text, 258)
                    var plain = getText(0, length)
                    var lines = plain.match(/^(.*)$/gm)
                    notesModel.setData(notesModel.index(notesView.currentIndex, 0), plain == "" ? "New note" : lines[0], 257)
                }

                if (!processing) {
                    processing = true;
                    var p = cursorPosition;

                    text = highlightSyntax(getText(0, length))

                    cursorPosition = p;
                    processing = false;
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
            y: 30
            anchors.topMargin: 35
            anchors.bottomMargin: 25
            model: notesModel
            anchors.fill: parent
            highlight: Rectangle { color: "#e6e6e6" }
            clip: true

            delegate: ItemDelegate {
                width: parent.width
                clip: true
                text: model.name

                onClicked: {
                    console.log('index changed')

                    notesView.currentIndex = index
                    updateContentText()
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
                    updateContentText()
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
                    updateContentText()
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
                    text: "EXPORT AS HTML"
                    onTriggered: fileExportAsHtml.open()
                }
            }
        }


        TextField {
            id: searchField
            height: 30
            text: qsTr("")
            rightPadding: 5
            leftPadding: 5
            bottomPadding: 0
            topPadding: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0

            placeholderText: "Search..."

            onTextChanged: {
                        if(text !== "") {
                               notesModel.restore();
                               notesModel.search(text);
                               notesView.model = 0;
                               notesView.model = notesModel
                        } else {
                             notesModel.restore();
                             notesView.model = 0;
                             notesView.model = notesModel
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

    FileDialog {
              id: fileExportAsHtml
              title: "Choose a path"
              selectExisting: false
              nameFilters: ["Html document (*.html)"]
              onAccepted: exportToHtml(fileExportAsHtml.fileUrl, contentsArea.getText(0,contentsArea.length-1))
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

    function getFilteredModel(text) {
        var storage = notesModel.pureData();
        var filtered = [];
        for(var i = 0; i < storage.length; i++) {
           if(storage[i].indexOf(text) !== -1 && !(filtered.indexOf(storage[i]) > -1)) {
               filtered.push({name : "Note_name", note: storage[i], index: i});
           }
//           if(filtered.indexOf(storage[i]) > -1 && storage[i].indexOf(text) === -1) {
//               for (var i = filtered.length - 1; i >=0; i--) {
//                   if (filtered[i] === strorage[i]) {
//                       array.splice(i, 1);
//                   }
//               }
//           }

        }
        console.log(filtered);
        return filtered;
    }

    function exportToHtml(fileUrl, txt) {
        var notes = notesModel.pureData().reverse();
        var curr = notes[notesView.currentIndex];
        console.log(curr);
        function mmd(str) {
            //parse line
            str = str.replace(/^(.*)$/gm, " <p>$1</p> ");
            //lists
            str = str.replace(/(-\s[\s\S]*-\s.*)\n/g, " <ul>$1</ul> ");
            str = str.replace(/(\d\.\s[\s\S]*\d\.\s.*)\n/g, " <ol>$1</ol> ");
            str = str.replace(/-\s(.*)/gm, " <li>$1</li> ");
            str = str.replace(/\d\.\s(.*)/gm, " <li>$1</li> ");

            str = str.replace(/\*(.*)\*/g, " <b>$1</b> ");
            str = str.replace(/\_(.*)\_/g, " <i>$1</i>  ");
            str = str.replace(/```([a-z]*[\s\S]*?)```/g, " <code> $1 </code>");
            str = str.replace(/~~(.*)~~/g, "  <strike> $1 </strike>  ");
            str = str.replace(/\[([^\[]+)\]\(([^\)]+)\)/g, "<a href='$2'>$1</a>");

            str = str.replace(/######\s(.*)/g, "<h6> $1 </h6> ");
            str = str.replace(/#####\s(.*)/g, "<h5>$1</h5> ");
            str = str.replace(/####\s(.*)/g, "<h4>$1</h4> ");
            str = str.replace(/###\s(.*)/g, " <h3>$1</h3>");
            str = str.replace(/##\s(.*)/g, " <h2>$1</h2>");
            str = str.replace(/#\s(.*)/g, " <h1>$1</h1> ");


            return str
        };
        var request = new XMLHttpRequest();
        request.open("PUT", fileUrl, false);
        request.send(mmd(curr));
        return request.status;

        }

}
