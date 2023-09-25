import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 1200
    height: 720
    title: "App Window"

    StackView {
        id: stackView
        anchors.fill: parent

        Component {
            id: loginComponent

            Rectangle {
                width: 1200
                height: 720

                Image {
                    source: "file:///E:/BD2/PROYECTO1/GUI/GUI/images/fondo.jpeg"
                    fillMode: Image.PreserveAspectCrop
                    anchors.fill: parent
                }

                Item {
                    anchors.fill: parent
                    Rectangle {
                        width: parent.width
                        height: parent.height
                        color: "black"
                        opacity: 0.5
                        z: -1 // Establece un valor de z menor para colocarlo detrás
                    }

                    Column {
                        anchors.centerIn: parent
                        spacing: 10

                        Image {

                            source: "file:///E:/BD2/PROYECTO1/GUI/GUI/images/BD.png"
                            width: 200
                            height: 200
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        Text {
                            text: "Welcome"
                            font.bold: true
                            font.pixelSize: 30
                            color: "white"
                            horizontalAlignment: Qt.AlignHCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        Rectangle {
                            width: 200
                            height: 40
                            color: "transparent" // Color transparente
                            border.color: "white" // Borde blanco
                            radius: 5 // Radio para bordes redondeados

                            TextField {
                                id: usernameTextField // Identificador añadido
                                anchors.fill: parent
                                color: "white"
                                placeholderText: "Usuario"
                                horizontalAlignment: Qt.AlignHCenter
                            }
                        }

                        Rectangle {
                            width: 200
                            height: 40
                            color: "transparent" // Color transparente
                            border.color: "white" // Borde blanco
                            radius: 5 // Radio para bordes redondeados

                            TextField {
                                id: passwordTextField // Identificador añadido
                                anchors.fill: parent
                                color: "white"
                                placeholderText: "Contraseña"
                                echoMode: TextInput.Password
                                horizontalAlignment: Qt.AlignHCenter
                            }
                        }

                        Button {
                            text: "Log In"
                            onClicked: {
                                if (bridge.verifyLogin(usernameTextField.text, passwordTextField.text)) {
                                    errorMessage.visible = false;
                                    stackView.push(mainPageComponent.createObject(stackView));
                                } else {
                                    errorMessage.visible = true; // Mostrar el mensaje de error
                                }
                            }
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Text {
                            id: errorMessage
                            color: "red"
                            text: "Credenciales inválidas"
                            anchors.horizontalCenter: parent.horizontalCenter
                            visible: false // Ocultar por defecto
                        }
                    }
                }
            }
        }

        Component {
            id: mainPageComponent

            Rectangle {
                width: 1200
                height: 720

                Image {
                    source: "file:///E:/BD2/PROYECTO1/GUI/GUI/images/fondo.jpeg"
                    fillMode: Image.PreserveAspectCrop
                    anchors.fill: parent
                }
                anchors.fill: parent

                Item {
                    anchors.fill: parent

                    Rectangle {
                        width: parent.width
                        height: parent.height
                        color: "black"
                        opacity: 0.5
                        z: -1 // Establece un valor de z menor para colocarlo detrás
                    }

                    Column {
                        anchors.centerIn: parent
                        spacing: 10

                        Rectangle {
                            width: 800
                            height: 200
                            color: "white"
                            border.color: "black"
                            radius: 10
                            clip: true

                            ScrollView {
                                anchors.fill: parent
                                clip: true
                                TextArea {
                                    id: txtArea
                                    width: parent.width
                                    height: parent.height
                                    wrapMode: Text.Wrap
                                }
                            }
                        }

                        Row {
                            spacing: 10
                            Button {
                                text: "Run"
                                onClicked: {
                                    bridge.runQuery(txtArea.text);
                                    if(bridge.getErrorMessage() !== "") {
                                        outputArea.text = "Error: " + bridge.getErrorMessage();
                                    } else {
                                        outputArea.text = bridge.readRecords();
                                    }
                                }

                            }


                            Button {
                                text: "Clear"
                                onClicked: {
                                    txtArea.text = "";
                                    // Limpia otros elementos si es necesario
                                }

                            }



                        }

                        Rectangle {
                            width: 800
                            height: 240
                            color: "white"
                            border.color: "black"
                            radius: 10
                            clip: true

                            ScrollView {
                                anchors.fill: parent
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

                        Button {
                            text: "Log Out"
                            onClicked: {
                                stackView.pop();
                            }
                        }
                    }
                }
            }
        }
        initialItem: Item {
            Component.onCompleted: {
                stackView.push(loginComponent.createObject(stackView));
            }
        }
    }
}
