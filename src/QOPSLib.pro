#-------------------------------------------------
#
# Project created by QtCreator 2014-04-04T16:33:39
#
#-------------------------------------------------

QT += gui

TARGET = QOPS
TEMPLATE = lib

DEFINES += QOPSLIB_LIBRARY

include(QOPSLib.pri)

unix {
    headers.path    = /usr/local/include/QOPSLib

    target.path = /usr/local/lib
    INSTALLS += target \
                headers
}
