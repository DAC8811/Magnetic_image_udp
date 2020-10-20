QT       += core gui \
        network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    imgoperator.cpp \
    main.cpp \
    mainwindow.cpp \
    scannerctr.cpp \
    timecount.cpp

HEADERS += \
    imgoperator.h \
    mainwindow.h \
    parameters.h \
    scannerctr.h \
    timecount.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Scanner_UDP_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += D:/opencv/build/include

CONFIG(debug, debug|release): {
    LIBS += -LD:/opencv/build/x64/vc15/lib   \
    -lopencv_world340d

} else{
    LIBS += -LD:/opencv/build/x64/vc15/lib   \
    -lopencv_world340
}

