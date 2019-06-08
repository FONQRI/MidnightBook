QT += quick network
CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += -std=c++17 -O3
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        src/cpp/models/Book.cpp \
        src/cpp/models/LibraryManager.cpp \
        src/cpp/models/Season.cpp \
        src/cpp/network/BookDownloader.cpp \
        src/cpp/network/DownloadManager.cpp \
        src/cpp/network/FileDownloader.cpp \
        src/cpp/storage/FileManager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
  android/AndroidManifest.xml \
  android/build.gradle \
  android/gradle/wrapper/gradle-wrapper.jar \
  android/gradle/wrapper/gradle-wrapper.properties \
  android/gradlew \
  android/gradlew.bat \
  android/res/values/libs.xml \
  qtquickcontrols2.conf \
  resources/icon/midnight_128.png \
  resources/icon/midnight_256.png \
  resources/icon/midnight_512.png

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
  ANDROID_PACKAGE_SOURCE_DIR = \
    $$PWD/android
}

HEADERS += \ \
  src/cpp/models/Author.h \
  src/cpp/models/Book.h \
  src/cpp/models/LibraryInfo.h \
  src/cpp/models/LibraryManager.h \
  src/cpp/models/Season.h \
  src/cpp/network/BookDownloader.h \
  src/cpp/network/DownloadManager.h \
  src/cpp/network/FileDownloader.h \
  src/cpp/storage/FileManager.h

