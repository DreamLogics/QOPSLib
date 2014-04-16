
INCLUDEPATH += $$PWD

QT       -= gui

CONFIG += use_gui

use_gui
{
    DEFINES += USE_GUI
    QT += gui
}

SOURCES += \
    $$PWD/propsheet.cpp \
    $$PWD/table.cpp \
    $$PWD/property.cpp \
    $$PWD/propsheetreader.cpp \
    $$PWD/propsheetwriter.cpp \
    $$PWD/sequence.cpp

HEADERS +=\
    $$PWD/propsheet.h \
    $$PWD/table.h \
    $$PWD/property.h \
    $$PWD/propsheetreader.h \
    $$PWD/propsheetwriter.h \
    $$PWD/propsheetprivate.h \
    $$PWD/propertyprivate.h \
    $$PWD/qopslib_global.h \
    $$PWD/opsinformationprovider.h \
    $$PWD/qopslib.h \
    $$PWD/sequence.h \
    $$PWD/propsheetreaderprivate.h \
    $$PWD/tableprivate.h \
    $$PWD/sequenceprivate.h \
    $$PWD/valuetypes.h
