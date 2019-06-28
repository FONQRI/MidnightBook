import QtQuick 2.12
import QtQuick.Controls 2.12
import fonqri.book.models 1.0
import QtGraphicalEffects 1.12

Item {

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
    ListView {
        anchors.fill: parent
        model: BooksModel {
        }
        delegate: Label {
            //            text: name
            width: parent.width
            height: 500

            Image {
                anchors.fill: parent
                source: Qt.resolvedUrl("file:///" + CoverImagePath)
                sourceSize: Qt.size(parent.width, parent.height)
            }

            //            SeasonsMainView {
            //                model: seasons
            //            }
        }
    }

    //    Component.onCompleted: console.log(booksModel.books)
}
