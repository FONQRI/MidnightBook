import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12

Window {
    visible: true
    property bool isPhone: Qt.platform.os == "android"
                           || Qt.platform.os == "ios"
    //    width: Screen.width
    //    height: Screen.height
    width: isPhone ? Screen.width : 480
    height: isPhone ? Screen.height : 720
    title: qsTr("Midnight Book")

    BooksView {
        anchors.fill: parent
    }

    //    //Component.onCompleted: exitPopup.open()
    //    LinearGradient {
    //        anchors.fill: parent
    //        end: Qt.point(parent.width * 0.266, 0)
    //        start: Qt.point(parent.width * 0.776, parent.height)
    //        gradient: Gradient {
    //            GradientStop {
    //                position: 1
    //                color: "#191970"
    //            }

    //            //            GradientStop {
    //            //                position: 0.2
    //            //                color: "#8D0B93"
    //            //            }
    //            GradientStop {
    //                position: 0
    //                color: "#800080"
    //            }
    //        }
    //    }

    //    Popup {
    //        id: exitPopup
    //        height: parent.height / 4
    //        width: parent.width / 1.5

    //        padding: 0
    //        anchors.centerIn: parent
    //        background: Item {
    //            Rectangle {
    //                anchors.fill: parent
    //                id: rect
    //                visible: false // hide it!!!
    //                LinearGradient {
    //                    anchors.fill: parent
    //                    //                    end: Qt.point(parent.width * 0.266, 0)
    //                    //                    start: Qt.point(parent.width * 0.776, parent.height)
    //                    gradient: Gradient {
    //                        GradientStop {
    //                            position: 0.0
    //                            color: "#800080"
    //                        }
    //                        GradientStop {
    //                            position: 1.0
    //                            color: "#191970"
    //                        }
    //                    }
    //                }
    //            }
    //            OpacityMask {
    //                anchors.fill: rect
    //                source: rect
    //                maskSource: Rectangle {
    //                    width: rect.width
    //                    height: rect.height
    //                    radius: 10
    //                }
    //            }
    //        }
    //    }
    //    Component.onCompleted: console.log(Qt.platform.os)
}
