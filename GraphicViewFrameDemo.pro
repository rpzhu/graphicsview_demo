QT       += core gui printsupport opengl sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    anchorlayout_widget.cpp \
    flowlayout.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    anchorlayout_widget.h \
    flowlayout.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    Resource.qrc
RC_ICONS = img/theico.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DESTDIR = $$PWD/bin
MOC_DIR = $$PWD/temp/moc
RCC_DIR = $$PWD/temp/rcc
UI_DIR = $$PWD/temp/ui
OBJECTS_DIR = $$PWD/temp/obj

CONFIG += precompile_header

include($$PWD/chip/chip.pri)
include($$PWD/basicgraphicslayouts/basicgraphicslayouts.pri)
include($$PWD/collidingmice/collidingmice.pri)
include($$PWD/diagramscene/diagramscene.pri)
include($$PWD/dragdroprobot/dragdroprobot.pri)
include($$PWD/drilldown/drilldown.pri)
include($$PWD/elasticnodes/elasticnodes.pri)
include($$PWD/embeddeddialogs/embeddeddialogs.pri)
include($$PWD/fademessage/fademessage.pri)
include($$PWD/gvfflowlayout/gvfflowlayout.pri)
include($$PWD/lighting/lighting.pri)
include($$PWD/padnavigator/padnavigator.pri)
include($$PWD/weatheranchorlayout/weatheranchorlayout.pri)
