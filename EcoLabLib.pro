#-------------------------------------------------
#
# Project created by QtCreator 2015-07-07T16:27:55
#
#-------------------------------------------------

QT       += network websockets

QT       -= gui

TARGET = EcoLabLib
TEMPLATE = lib
VERSION = 3.0.0

DEFINES += ECOLABLIB_LIBRARY

SOURCES += src/ecolablib.cpp \
    src/builder.cpp \
    src/settingsstorage.cpp \
    src/client/client.cpp \
    src/client/clientmanager.cpp \
    src/client/clientpinger.cpp \
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
    src/client/clientenums.h \
    src/client/clientmanager.h \
    src/client/clientpinger.h \
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

DISTFILES += CHANGELOG.md \
    Doxyfile \
    LICENSE \
    README.md \
    doc/doc.md \
    data/EcoLabLib.conf \
    data/EcoLabLibDependencies.conf \
    data/example_header.tex
