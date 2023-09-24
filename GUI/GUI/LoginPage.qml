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

        Text {
            text: "Inicio de Sesión"
            font.bold: true
            font.pixelSize: 24
        }

        TextField {
            placeholderText: "Usuario"
            width: 200
        }

        TextField {
            placeholderText: "Contraseña"
            width: 200
            echoMode: TextInput.Password
        }

        Button {
            text: "Entrar"
            onClicked: {
                // Simulación de inicio de sesión exitoso
                loginSuccess()
            }
        }
    }

    // Esta señal se emite cuando se inicia sesión correctamente
    signal loginSuccess()
}
