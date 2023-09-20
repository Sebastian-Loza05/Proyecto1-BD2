import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "App Window"

    Column {
        anchors.centerIn: parent
        spacing: 10

        Text {
            text: "Select method"
            font.bold: true
            font.pixelSize: 24
        }

        Row {
            spacing: 10
            Text { text: "AVL" }
            Button {
                id: btn1
                text: "1"
                onClicked: bridge.methodChosen("1")
            }
        }

        Row {
            spacing: 10
            Text { text: "HASHING" }
            Button {
                id: btn2
                text: "2"
                onClicked: bridge.methodChosen("2")
            }
        }

        Row {
            spacing: 10
            Text { text: "BPLUS" }
            Button {
                id: btn3
                text: "3"
                onClicked: bridge.methodChosen("3")
            }
        }

        TextArea {
            id: txtArea
            width: 200
            height: 100
        }

        Button {
            text: "RUN"
            onClicked: {
                outputArea.text = "Metodo: " + bridge.method + "\nConsulta: " + txtArea.text;
                bridge.runQuery(txtArea.text);
            }
        }

        Button {
            text: "Clear"
            onClicked: {
                txtArea.text = "";
                outputArea.text = "";
            }
        }

        TextArea {
            id: outputArea
            width: 200
            height: 100
            readOnly: true
            onTextChanged: {
                text = "Metodo: " + bridge.method + "\nConsulta: " + bridge.query;
            }
        }
    }
}
