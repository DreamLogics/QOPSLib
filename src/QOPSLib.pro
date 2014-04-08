#-------------------------------------------------
#
# Project created by QtCreator 2014-04-04T16:33:39
#
#-------------------------------------------------

QT       -= gui

TARGET = QOPSLib
TEMPLATE = lib

DEFINES += QOPSLIB_LIBRARY

SOURCES += \
    propsheet.cpp \
    table.cpp \
    property.cpp \
    propsheetreader.cpp \
    propsheetwriter.cpp \
    animation.cpp

HEADERS +=\
    propsheet.h \
    table.h \
    property.h \
    propsheetreader.h \
    propsheetwriter.h \
    propsheetprivate.h \
    styleprivate.h \
    propertyprivate.h \
    animation.h \
    qopslib_global.h \
    opsinformationprovider.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
