#-------------------------------------------------
#
# Project created by QtCreator 2015-07-07T16:27:55
#
#-------------------------------------------------

QT       += network websockets

QT       -= gui

TARGET = EcoLabLib
TEMPLATE = lib
VERSION = 2.99.1

DEFINES += ECOLABLIB_LIBRARY

SOURCES += src/ecolablib.cpp \
    src/builder.cpp \
    src/settingsstorage.cpp \
    src/client/client.cpp \
    src/client/clientmanager.cpp \
    src/session/sessionsmodel.cpp \
    src/session/session.cpp \
    src/session/ztree.cpp \
    src/session/receiptscreator.cpp \
    src/session/receiptsprinter.cpp

HEADERS += src/ecolablib.h\
        src/ecolablib_global.h \
    src/builder.h \
    src/settingsstorage.h \
    src/client/client.h \
    src/client/clientmanager.h \
    src/session/sessionsmodel.h \
    src/session/session.h \
    src/session/ztree.h \
    src/session/receiptscreator.h \
    src/session/receiptsprinter.h

QMAKE_CXXFLAGS += -std=c++11

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    CHANGELOG.md \
    Doxyfile \
    LICENSE \
    README.md \
    data/EcoLabLib.conf \
    data/example_header.tex \
    doc/doc.md

DISTFILES += \
    LICENSE \
    doc/doc.md
