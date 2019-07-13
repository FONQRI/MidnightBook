import QtQuick 2.12
import QtQuick.Controls 2.12
import fonqri.book.models 1.0
import QtGraphicalEffects 1.12

Component {
    Item {
        property variant myData: model
        width: parent.width * (5 / 9)
        height: parent.height * (6 / 9)
        scale: PathView.iconScale
        Column {
            anchors.fill: parent
            Label {
                width: parent.width
                height: parent.height

                Image {
                    id: img
                    property bool rounded: true
                    property bool adapt: true

                    layer.enabled: rounded
                    layer.effect: OpacityMask {
                        maskSource: Item {
                            width: img.width
                            height: img.height
                            Rectangle {
                                anchors.centerIn: parent
                                width: img.adapt ? img.width : Math.min(
                                                       img.width, img.height)
                                height: img.adapt ? img.height : width
                                radius: 10
                            }
                        }
                    }
                    anchors.fill: parent
                    source: Qt.resolvedUrl("file:///" + coverImagePath)
                    sourceSize: Qt.size(parent.width, parent.height)
                }
            }

            Text {
                topPadding: 5
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                font.pixelSize: 18
                text: qsTr(name)
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: showPreview()
        }
    }
}
