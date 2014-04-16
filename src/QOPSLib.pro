#-------------------------------------------------
#
# Project created by QtCreator 2014-04-04T16:33:39
#
#-------------------------------------------------


TARGET = QOPSLib
TEMPLATE = lib

DEFINES += QOPSLIB_LIBRARY

include(QOPSLib.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}
