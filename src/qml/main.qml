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

    LinearGradient {
        anchors.fill: parent
        end: Qt.point(parent.width * 0.266, 0)
        start: Qt.point(parent.width * 0.776, parent.height)
        gradient: Gradient {
            GradientStop {
                position: 1
                color: "#191970"
            }

            GradientStop {
                position: 0
                color: "#800080"
            }
        }
    }
    Component {
        id: bookView
        BooksView {
            anchors.fill: parent
        }
    }

    Loader {
        anchors.fill: parent
        id: loader
    }
    Component.onCompleted: loader.sourceComponent = bookView
}
