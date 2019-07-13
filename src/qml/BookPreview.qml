import QtQuick 2.12
import QtQuick.Controls 2.12
import fonqri.book.models 1.0
import QtGraphicalEffects 1.12

Item {
    property string summery
    property string coverPath
    property string bookName
    Component.onCompleted: console.log(summery)
    anchors.fill: parent
    Image {
        id: backgroundImage
        anchors.fill: parent
        source: coverPath
    }

    Label {
        id: summeryLable
        anchors.fill: fastBlur
        leftPadding: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        text: "<center>" + bookName + "</center><br>" + summery
        textFormat: Text.RichText
        background: Rectangle {
            id: bug
            radius: 5
            width: fastBlur.width
            height: fastBlur.height
        }
    }

    FastBlur {
        id: fastBlur

        radius: 86
        width: parent.width * 0.8
        height: parent.height * 0.8
        anchors.centerIn: parent
        opacity: 0.70

        source: ShaderEffectSource {
            sourceItem: backgroundImage
            sourceRect: Qt.rect(0, 0, fastBlur.width, fastBlur.height)
        }
    }
}
