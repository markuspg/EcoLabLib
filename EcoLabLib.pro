#-------------------------------------------------
#
# Project created by QtCreator 2015-07-07T16:27:55
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = EcoLabLib
TEMPLATE = lib

DEFINES += ECOLABLIB_LIBRARY

SOURCES += src/ecolablib.cpp \
    src/builder.cpp \
    src/settingsstorage.cpp \
    src/client/client.cpp \
    src/client/clientmanager.cpp \
    src/session/sessionsmodel.cpp \
    src/session/session.cpp

HEADERS += src/ecolablib.h\
        src/ecolablib_global.h \
    src/builder.h \
    src/settingsstorage.h \
    src/client/client.h \
    src/client/clientmanager.h \
    src/session/sessionsmodel.h \
    src/session/session.h

QMAKE_CXXFLAGS += -std=c++11

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    data/EcoLabLib.conf
