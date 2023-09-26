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
                    source: "file:/home/luisd/UTEC/ciclo_6/BDII/proyecto/Proyecto1-BD2/GUI/GUI/images/fondo.jpeg"
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

                            source: "file:/home/luisd/UTEC/ciclo_6/BDII/proyecto/Proyecto1-BD2/GUI/GUI/images/BD.png"
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
                            text: "Log In ➡️"
                            onClicked: {
                                if (bridge.verifyLogin(usernameTextField.text, passwordTextField.text)) {
                                    errorMessage.visible = false;
                                    stackView.push(mainPageComponent.createObject(stackView));
                                } else {
                                    errorMessage.visible = true;
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
                    source: "file:/home/luisd/UTEC/ciclo_6/BDII/proyecto/Proyecto1-BD2/GUI/GUI/images/fondo.jpeg"
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
                        z: -1
                    }

                    Column {
                        anchors.centerIn: parent
                        spacing: 10

                        Rectangle {
                            width: 800
                            height: 200
                            color: "transparent"  // Hacemos transparente este Rectangle que actúa como contenedor

                            Row {
                                anchors.fill: parent
                                spacing: 10

                                // Rectangle que envuelve al TextArea
                                Rectangle {
                                    width: 600  // Aquí definimos el ancho para el TextArea y su contenedor
                                    height: parent.height
                                    color: "white"
                                    border.color: "black"
                                    radius: 10
                                    clip: true

                                    ScrollView {
                                        anchors.fill: parent
                                        clip: true
                                        TextArea {
                                            id: txtArea
                                            anchors.fill: parent
                                            wrapMode: Text.Wrap
                                        }
                                    }
                                }

                                // Imagen a la derecha del TextArea
                                Image {
                                    width: 200
                                    height: parent.height
                                    source: "file:/home/luisd/UTEC/ciclo_6/BDII/proyecto/Proyecto1-BD2/GUI/GUI/images/excel.png"
                                    fillMode: Image.PreserveAspectFit
                                }
                            }
                        }
                        Row {
                            spacing: 10
                            Button {
                                text: "✅"
                                onClicked: {
                                    headersRow.visible = true;
                                    bridge.runQuery(txtArea.text);
                                    if(bridge.getErrorMessage() !== "") {
                                        headersRow.currentHeaders = [bridge.getErrorMessage()];
                                        recordsListModel1.clear(); // Limpia los registros anteriores
                                        recordsListModel2.clear(); // Limpia los registros anteriores

                                    } else {
                                        var M= bridge.getErrorMessage();
                                        M = "Succesfull";
                                        headersRow.currentHeaders = [M];
                                        recordsListModel1.clear();
                                        recordsListModel2.clear();
                                        const records = bridge.readRecords();
                                        if (records.length > 0) {
                                            if (records[0].headerType === "1") {
                                                headersRow.currentHeaders = headersRow.headers1;
                                                for(let i = 0; i < records.length; ++i) {
                                                    recordsListModel1.append(records[i]);
                                                }
                                            } else if (records[0].headerType === "2") {
                                                headersRow.currentHeaders = headersRow.headers2;
                                                for(let i = 0; i < records.length; ++i) {
                                                    recordsListModel2.append(records[i]);
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Button {
                                text: "🛑"
                                onClicked: {
                                    txtArea.text = "";
                                    recordsListModel.clear();
                                    headersRow.visible = false;
                                }
                            }
                        }

                        Rectangle {
                            width: 805
                            height: 295
                            color: "white"
                            border.color: "black"
                            radius: 15
                            clip: true

                            Column {
                                spacing: 5
                                anchors.top: parent.top
                                width: parent.width

                                Row {
                                    id: headersRow
                                    visible: false
                                    spacing: 5

                                    property var headers1: ["Código", "Nombre", "Producto", "Marca", "Precio", "Cantidad"]
                                    property var headers2: ["Código", "Género", "Profesión", "Edad", "Sueldo"]
                                    property var currentHeaders: ["Succesfull!"]

                                    Repeater {
                                        model: headersRow.currentHeaders
                                        delegate: Rectangle {
                                            width: index === 0 ? 90 : 140
                                            height: 30
                                            color: "transparent"
                                            Text {
                                                anchors.centerIn: parent
                                                text: modelData
                                            }
                                        }
                                    }
                                }

                                ScrollView {
                                    width: parent.width
                                    height: 230
                                    clip: true

                                    TableView {
                                        width: parent.width
                                        height: parent.height
                                        model: headersRow.currentHeaders === headersRow.headers1 ? recordsListModel1 : recordsListModel2

                                        delegate: Row {
                                            spacing: 5
                                            Rectangle {
                                                width: 90
                                                height: 30
                                                color: "transparent"
                                                Text {
                                                    anchors.centerIn: parent
                                                    text: model.codigo
                                                }
                                            }
                                            Rectangle {
                                                width: 140
                                                height: 30
                                                color: "transparent"
                                                Text {
                                                    anchors.centerIn: parent
                                                    text: model.nombre || model.genero || ""
                                                }
                                            }
                                            Rectangle {
                                                width: 140
                                                height: 30
                                                color: "transparent"
                                                Text {
                                                    anchors.centerIn: parent
                                                    text: model.producto || model.profesion || ""
                                                }
                                            }
                                            Rectangle {
                                                width: 140
                                                height: 30
                                                color: "transparent"
                                                Text {
                                                    anchors.centerIn: parent
                                                    text: model.marca || model.edad || ""
                                                }
                                            }
                                            Rectangle {
                                                width: 140
                                                height: 30
                                                color: "transparent"
                                                Text {
                                                    anchors.centerIn: parent
                                                    text: model.precio || model.sueldo || ""
                                                }
                                            }
                                            Rectangle {
                                                width: 140
                                                height: 30
                                                color: "transparent"
                                                Text {
                                                    anchors.centerIn: parent
                                                    text: model.cantidad || ""
                                                }
                                            }
                                        }
                                    }
                                }
                            }


                            }
                            Button {
                            text: "↩️"
                            onClicked: {
                                stackView.pop();
                            }
                        }
                    }
                }
            }
        }
        ListModel {
            id: recordsListModel1
        }
        ListModel {
            id: recordsListModel2
        }
        initialItem: Item {
            Component.onCompleted: {
                stackView.push(loginComponent.createObject(stackView));
            }
        }
    }
}
