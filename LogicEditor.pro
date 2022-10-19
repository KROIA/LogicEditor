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


INCLUDEPATH += $$inc \
               $$inc/logicGate \
               $$inc/logicGate/gates \
               $$inc/connection \
               $$inc/ui \
               $$inc/serialization \
               $$inc/utilities

HEADERS += \
    $$inc/ui/mainwindow.h \
    $$inc/ui/EditorRibbon.h \
    $$inc/logicGate/Pin.h \
    $$inc/logicGate/Gate.h \
    $$inc/logicGate/gates/AndGate.h \
    $$inc/connection/Connection.h \
    $$inc/LogicSignal.h \
    $$inc/EditingTool.h \
    $$inc/logicGate/LogicGates.h \
    $$inc/logicGate/gates/Clock.h \
    $$inc/logicGate/gates/InputGate.h \
    $$inc/logicGate/gates/NotGate.h \
    $$inc/logicGate/gates/OrGate.h \
    $$inc/logicGate/gates/XorGate.h \
    $$inc/serialization/ISerializable.h \
    $$inc/serialization/database.h \
    $$inc/serialization/databaseDeclaration.h \
    $$inc/serialization/databaseID.h \
    $$inc/serialization/databaseObject.h \
    $$inc/utilities/sfSerializer.h \
    $$inc/utilities/debug.h \
    inc/utilities/forwardDeclaration.h


SOURCES += \
    $$src/main.cpp \
    $$src/ui/mainwindow.cpp \
    $$src/ui/EditorRibbon.cpp \
    $$src/logicGate/Pin.cpp \
    $$src/logicGate/Gate.cpp \
    $$src/logicGate/gates/AndGate.cpp \
    $$src/connection/Connection.cpp \
    $$src/LogicSignal.cpp \
    $$src/EditingTool.cpp \
    $$src/logicGate/gates/Clock.cpp \
    $$src/logicGate/gates/InputGate.cpp \
    $$src/logicGate/gates/NotGate.cpp \
    $$src/logicGate/gates/OrGate.cpp \
    $$src/logicGate/gates/XorGate.cpp \
    $$src/serialization/ISerializable.cpp \
    $$src/serialization/database.cpp \
    $$src/serialization/databaseID.cpp \
    $$src/serialization/databaseObject.cpp \
    $$src/utilities/debug.cpp \
    src/utilities/sfSerializer.cpp


FORMS += \
    $$ui/mainwindow.ui

RESOURCES += \
    $$PWD/resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
