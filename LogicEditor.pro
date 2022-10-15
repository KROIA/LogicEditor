QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(../SFML_EditorWidget/SFML_EditorWidget.pri)
include(extern/QT-Ribbon-Widget/QT-Ribbon-Widget.pri)


inc = inc
src = src
ui  = ui

INCLUDEPATH += $$inc

HEADERS += \
    $$inc/mainwindow.h

SOURCES += \
    $$src/main.cpp \
    $$src/mainwindow.cpp

FORMS += \
    $$ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
