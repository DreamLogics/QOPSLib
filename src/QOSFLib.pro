#-------------------------------------------------
#
# Project created by QtCreator 2014-04-04T16:33:39
#
#-------------------------------------------------

QT       -= gui

TARGET = QOSFLib
TEMPLATE = lib

DEFINES += QOSFLIB_LIBRARY

SOURCES += \
    stylesheet.cpp \
    style.cpp \
    property.cpp \
    stylesheetreader.cpp \
    stylesheetwriter.cpp \
    animation.cpp

HEADERS +=\
    stylesheet.h \
    style.h \
    property.h \
    stylesheetreader.h \
    stylesheetwriter.h \
    stylesheetprivate.h \
    styleprivate.h \
    propertyprivate.h \
    animation.h \
    qosflib_global.h \
    osfinformationprovider.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
