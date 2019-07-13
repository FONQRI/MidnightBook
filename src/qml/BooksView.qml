import QtQuick 2.12
import QtQuick.Controls 2.12
import fonqri.book.models 1.0
import QtGraphicalEffects 1.12

Item {
    property string currentSummery
    property url currentCoverPath
    property string currentBookName
    signal showPreview
    onShowPreview: bookViewLoader.sourceComponent = previewComp
    BookDelegate {
        id: bookDelegate
    }

    Component {
        id: appHighlight
        Rectangle {
            visible: PathView.onPath

            width: 40
            height: 40
            color: "red"
        }
    }
    Loader {
        id: bookViewLoader
        anchors.fill: parent
    }
    Component {
        id: booksViewComp
        PathView {
            id: view
            anchors.fill: parent

            //        highlight: appHighlight
            preferredHighlightBegin: 0.5
            preferredHighlightEnd: 0.5
            //_________________________________________
            //        property int previousCurrentIndex: 0
            //        currentIndex: 0

            //        onCurrentIndexChanged: {
            //            var lastIndex = contactModel.count - 1
            //            if (currentIndex == 0 && previousCurrentIndex == lastIndex) {
            //                pathViewId.positionViewAtIndex(lastIndex, PathView.Beginning)
            //            } else if (currentIndex == lastIndex && previousCurrentIndex == 0) {
            //                pathViewId.positionViewAtIndex(0, PathView.Beginning)
            //            }
            //            previousCurrentIndex = currentIndex
            //        }
            //__________________________________________
            //        property real previousOffsetVal: 0.

            //        onOffsetChanged: {
            //            /* 10 is a threshhold, should be smaller than the total elements count*/
            //            if (Math.abs(previousOffsetVal - offset) > 10) {
            //                offset = previousOffsetVal
            //            }

            //            previousOffsetVal = offset
            //        }
            onCurrentIndexChanged: {

                currentBookName = view.currentItem.myData.name
                currentSummery = view.currentItem.myData.summary
                currentCoverPath = Qt.resolvedUrl(
                            "file:///" + view.currentItem.myData.coverImagePath)
            }
            focus: true
            //        model: 10
            model: BooksModel {
                id: contactModel
            }
            delegate: bookDelegate
            path: Path {
                startX: 0
                startY: parent.height / 2
                PathAttribute {
                    name: "iconScale"
                    value: 0.5
                }
                PathLine {
                    x: parent.width / 2
                    y: parent.height / 2
                }
                PathAttribute {
                    name: "iconScale"
                    value: 1.0
                }
                PathLine {
                    x: parent.width
                    y: parent.height / 2
                }
                PathAttribute {
                    name: "iconScale"
                    value: 0.5
                }
            }

            Component.onCompleted: {
                console.log("book title " + view.currentItem.myData.name)
                currentBookName = view.currentItem.myData.name
                currentSummery = view.currentItem.myData.summary
                currentCoverPath = Qt.resolvedUrl(
                            "file:///" + view.currentItem.myData.coverImagePath)
            }
        }
    }
    Component {
        id: previewComp

        BookPreview {
            summery: currentSummery
            coverPath: currentCoverPath
            bookName: currentBookName
        }
    }

    Component.onCompleted: {

        bookViewLoader.sourceComponent = booksViewComp
    }
}
