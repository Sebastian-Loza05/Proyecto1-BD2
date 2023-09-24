import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "App Window"

    Rectangle {  // Fondo de la ventana para ocupar todo el espacio disponible
        color: "transparent"
        anchors.fill: parent

        Column {
            anchors.centerIn: parent  // Esto centra la Column en el Rectangle
            spacing: 10

            Text {
                text: "Run Your Query"
                font.bold: true
                font.pixelSize: 24
            }

            TextArea {
                id: txtArea
                width: 400
                height: 100
            }

            Button {
                text: "RUN"
                onClicked: {
                    outputArea.text = "\nConsulta: " + txtArea.text;
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

            TextArea {
                id: outputArea
                width: 400
                height: 100
                readOnly: true
            }
        }
    }
}
