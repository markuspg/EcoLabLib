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

SOURCES += src/ecolablib.cpp

HEADERS += src/ecolablib.h\
        src/ecolablib_global.h

QMAKE_CXXFLAGS += -std=c++11

unix {
    target.path = /usr/lib
    INSTALLS += target
}
