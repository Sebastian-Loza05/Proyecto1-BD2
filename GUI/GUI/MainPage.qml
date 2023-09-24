import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    width: 640
    height: 480

    Image {
        source: "file:///E:/BD2/PROYECTO1/GUI/GUI/images/fondo.jpeg"
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
    }

    Column {
        anchors.centerIn: parent
        spacing: 10

        Rectangle {
            width: 400
            height: 200
            color: "white"
            border.color: "black"
            radius: 10
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
                // Simular la llamada a la función bridge.runQuery(txtArea.text);
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
                // Simular la obtención de datos del archivo CSV
                var csvLines = ["Linea 1", "Linea 2", "Linea 3"];
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

        Button {
            text: "Cerrar Sesión"
            onClicked: {
                // Simular el cierre de sesión
                stackView.pop(); // Volver a la página de inicio de sesión
            }
        }
    }
}
