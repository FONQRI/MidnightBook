import QtQuick 2.12
import QtQuick.Controls 2.12
import fonqri.book.models 1.0
import QtGraphicalEffects 1.12

ListView {
//    model: seasons
    anchors.fill: parent
    delegate: Label {
        background: Rectangle {
            color: "blue"
        }
        text: model.modelData.name
        width: parent.width
        height: 20
    }
}
