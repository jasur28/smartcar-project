// // // Main.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

ApplicationWindow {
    id: window
    width: 400
    height: 640
    visible: true
    title: "BLE SmartCar"

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainPage
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: 10

            ToolButton {
                text: "\u2699"
                onClicked: stackView.push(scanPage)
            }

            Label {
                text: "Умная Машина"
                font.pixelSize: 20
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                width: 12
                height: 12
                radius: 6
                color: btManager.connected ? "green" : "red"
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 8
            }
        }
    }

    // --------------------------------------
    // Главная страница управления
    // --------------------------------------
    Component {
        id: mainPage

        Rectangle {
            color: "#f0f4f8"
            anchors.fill: parent

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 20

                Item { Layout.fillHeight: true } // Верхний отступ

                Text {
                    text: "Управление"
                    font.pixelSize: 24
                    font.bold: true
                    color: "#333"
                    Layout.alignment: Qt.AlignHCenter
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 40

                    ColumnLayout {
                        spacing: 20
                        Button {
                            text: "▲"
                            font.pixelSize: 24
                            width: 70; height: 70
                            onPressed: {
                                forwardPressed = true
                                sendCombinedCommand()
                            }
                            onReleased: {
                                forwardPressed = false
                                sendCombinedCommand()
                            }
                        }

                        Button {
                            text: "▼"
                            font.pixelSize: 24
                            width: 70; height: 70
                            onPressed: {
                                backwardPressed = true
                                sendCombinedCommand()
                            }
                            onReleased: {
                                backwardPressed = false
                                sendCombinedCommand()
                            }
                        }
                    }

                    ColumnLayout {
                        spacing: 20
                        Button {
                            text: "◀"
                            font.pixelSize: 24
                            width: 70; height: 70
                            onPressed: {
                                leftPressed = true
                                sendCombinedCommand()
                            }
                            onReleased: {
                                leftPressed = false
                                sendCombinedCommand()
                            }
                        }

                        Button {
                            text: "▶"
                            font.pixelSize: 24
                            width: 70; height: 70
                            onPressed: {
                                rightPressed = true
                                sendCombinedCommand()
                            }
                            onReleased: {
                                rightPressed = false
                                sendCombinedCommand()
                            }
                        }
                    }
                }

                Item { Layout.fillHeight: true } // Заполнить оставшееся пространство

                Text {
                    id: statusText
                    text: ""
                    font.pixelSize: 16
                    color: "#555"
                    Layout.alignment: Qt.AlignHCenter
                }

                Connections {
                    target: btManager
                    onStatusMessage: (msg) => statusText.text = msg
                    onConnectedChanged: {
                        if (btManager.connected) {
                            stackView.pop();
                        }
                    }
                }
            }

            property bool forwardPressed: false
            property bool backwardPressed: false
            property bool leftPressed: false
            property bool rightPressed: false

            function sendCombinedCommand() {
                if (forwardPressed && rightPressed) {
                    btManager.sendCommand("FR")
                } else if (forwardPressed && leftPressed) {
                    btManager.sendCommand("FL")
                } else if (backwardPressed && leftPressed) {
                    btManager.sendCommand("BL")
                } else if (backwardPressed && rightPressed) {
                    btManager.sendCommand("BR")
                } else if (forwardPressed) {
                    btManager.sendCommand("F")
                } else if (backwardPressed) {
                    btManager.sendCommand("B")
                } else if (leftPressed) {
                    btManager.sendCommand("L")
                } else if (rightPressed) {
                    btManager.sendCommand("R")
                } else {
                    btManager.sendCommand("S")
                }
            }
        }
    }

    // --------------------------------------
    // Страница сканирования
    // --------------------------------------
    Component {
        id: scanPage

        Rectangle {
            color: "#ffffff"
            anchors.fill: parent

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12

                Label {
                    text: "BLE Устройства"
                    font.pixelSize: 22
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                Button {
                    text: "🔍 Сканировать"
                    font.pixelSize: 18
                    onClicked: btManager.startScan()
                }

                ListView {
                    width: parent.width
                    height: 280
                    model: btManager.deviceNames
                    spacing: 6
                    delegate: Rectangle {
                        width: parent.width
                        height: 40
                        color: "#e0f0ff"
                        radius: 6

                        Text {
                            anchors.centerIn: parent
                            text: modelData
                            font.pixelSize: 16
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: btManager.connectToDevice(index)
                        }
                    }
                }

                Button {
                    text: "↩ Назад"
                    font.pixelSize: 18
                    onClicked: stackView.pop()
                }
            }
        }
    }
}

// import QtQuick 2.15
// import QtQuick.Controls 2.15
// import QtQuick.Layouts 1.15
// import QtQuick.Window 2.15

// ApplicationWindow {
//     id: window
//     width: 400
//     height: 640
//     visible: true
//     title: "BLE SmartCar"

//     StackView {
//         id: stackView
//         anchors.fill: parent
//         initialItem: mainPage
//     }

//     header: ToolBar {
//         RowLayout {
//             anchors.fill: parent
//             spacing: 10

//             ToolButton {
//                 text: "\u2699"
//                 onClicked: stackView.push(scanPage)
//             }

//             Label {
//                 text: "Умная Машина"
//                 font.pixelSize: 20
//                 Layout.alignment: Qt.AlignHCenter
//             }

//             Rectangle {
//                 width: 12
//                 height: 12
//                 radius: 6
//                 color: btManager.connected ? "green" : "red"
//                 anchors.verticalCenter: parent.verticalCenter
//                 anchors.rightMargin: 8
//             }
//         }
//     }

//     // --------------------------------------
//     // Главная страница управления
//     // --------------------------------------
//     Component {
//         id: mainPage

//         Rectangle {
//             color: "#f0f4f8"
//             anchors.fill: parent

//             RowLayout {
//                 anchors.centerIn: parent
//                 spacing: 30

//                 ColumnLayout {
//                     spacing: 16
//                     Button {
//                         text: "▲"
//                         font.pixelSize: 24
//                         width: 60; height: 60
//                         onPressed: btManager.sendCommand("F")
//                         onReleased: btManager.sendCommand("S")
//                     }

//                     Button {
//                         text: "▼"
//                         font.pixelSize: 24
//                         width: 60; height: 60
//                         onPressed: btManager.sendCommand("B")
//                         onReleased: btManager.sendCommand("S")
//                     }
//                 }

//                 ColumnLayout {
//                     spacing: 16
//                     Button {
//                         text: "◀"
//                         font.pixelSize: 24
//                         width: 60; height: 60
//                         onPressed: btManager.sendCommand("L")
//                         onReleased: btManager.sendCommand("S")
//                     }

//                     Button {
//                         text: "▶"
//                         font.pixelSize: 24
//                         width: 60; height: 60
//                         onPressed: btManager.sendCommand("R")
//                         onReleased: btManager.sendCommand("S")
//                     }
//                 }
//             }

//             Text {
//                 id: statusText
//                 anchors.horizontalCenter: parent.horizontalCenter
//                 anchors.bottom: parent.bottom
//                 anchors.bottomMargin: 16
//                 text: ""
//                 font.pixelSize: 16
//                 color: "#555"
//             }

//             Connections {
//                 target: btManager
//                 onStatusMessage: (msg) => statusText.text = msg
//                 onConnectedChanged: {
//                     if (btManager.connected) {
//                         stackView.pop(); // Вернуться на главную при подключении
//                     }
//                 }
//             }
//         }
//     }

//     // --------------------------------------
//     // Страница сканирования
//     // --------------------------------------
//     Component {
//         id: scanPage

//         Rectangle {
//             color: "#ffffff"
//             anchors.fill: parent

//             ColumnLayout {
//                 anchors.fill: parent
//                 anchors.margins: 16
//                 spacing: 12

//                 Label {
//                     text: "BLE Устройства"
//                     font.pixelSize: 22
//                     font.bold: true
//                     Layout.alignment: Qt.AlignHCenter
//                 }

//                 Button {
//                     text: "🔍 Сканировать"
//                     font.pixelSize: 18
//                     onClicked: btManager.startScan()
//                 }

//                 ListView {
//                     width: parent.width
//                     height: 280
//                     model: btManager.deviceNames
//                     spacing: 6
//                     delegate: Rectangle {
//                         width: parent.width
//                         height: 40
//                         color: "#e0f0ff"
//                         radius: 6

//                         Text {
//                             anchors.centerIn: parent
//                             text: modelData
//                             font.pixelSize: 16
//                         }

//                         MouseArea {
//                             anchors.fill: parent
//                             onClicked: btManager.connectToDevice(index)
//                         }
//                     }
//                 }

//                 Button {
//                     text: "↩ Назад"
//                     font.pixelSize: 18
//                     onClicked: stackView.pop()
//                 }
//             }
//         }
//     }
// }




// import QtQuick 2.15
// import QtQuick.Controls 2.15

// ApplicationWindow {
//     width: 360
//     height: 640
//     visible: true
//     title: "BLE SmartCar"

//     Column {
//         anchors.centerIn: parent
//         spacing: 10

//         Button { text: "Сканировать"; onClicked: btManager.startScan() }

//         ListView {
//             width: parent.width
//             height: 150
//             model: btManager.deviceNames
//             delegate: Button {
//                 text: modelData
//                 onClicked: btManager.connectToDevice(index)
//             }
//         }

//         Row {
//             spacing: 10
//             Button { text: "F"; onClicked: btManager.sendCommand("F") }
//             Button { text: "B"; onClicked: btManager.sendCommand("B") }
//             Button { text: "L"; onClicked: btManager.sendCommand("L") }
//             Button { text: "R"; onClicked: btManager.sendCommand("R") }
//             Button { text: "S"; onClicked: btManager.sendCommand("S") }
//         }

//         Text {
//             id: statusText
//             text: ""
//         }

//         Connections {
//             target: btManager
//             onStatusMessage: (msg) => statusText.text = msg
//         }
//     }
// }
