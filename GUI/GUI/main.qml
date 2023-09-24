import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "App Window"
    Rectangle {
        anchors.fill: parent
        Image {
            source: "file:///E:/BD2/PROYECTO1/GUI/GUI/images/fondo.jpeg"
            fillMode: Image.PreserveAspectCrop
            anchors.fill: parent
        }
        Column {
            anchors.centerIn: parent
            spacing: 10
            Rectangle {
                width: 800
                height: 200
                color: "white"
                border.color: "black"
                radius: 10 // Radio de las esquinas redondeadas
                clip: true

                TextArea {
                    id: txtArea
                    width: parent.width
                    height: parent.height
                }
            }

            Button {
                text: "RUN"
                onClicked: {
                    outputArea.text = "Consulta: " + txtArea.text;
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

            Button {
                text: "Read CSV"
                onClicked: {
                    var csvLines = bridge.getThreeLines("E:/BD2/PROYECTO1/GUI/GUI/Libro1.csv");
                    outputArea.text = csvLines.join("\n");
                }
            }
            Rectangle {
                width: 400
                height: 200
                color: "white"
                border.color: "black"
                radius: 10
                clip: true
                TextArea {
                    id: outputArea
                    width: parent.width
                    height: parent.height
                    readOnly: true
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
            }
        }
    }
}
