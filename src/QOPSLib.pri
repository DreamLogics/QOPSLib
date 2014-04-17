
INCLUDEPATH += $$PWD

contains(QT, gui) {
    DEFINES += USE_GUI
    message("GUI support enabled.")
}

!contains(QT, gui) {
    message("GUI support disabled.")
}

contains(DEFINES, NO_SMART_POINTERS) {
    message("Smart pointers disabled.")
}

!contains(DEFINES, NO_SMART_POINTERS) {
    message("Smart pointers enabled.")
}

SOURCES += \
    $$PWD/propsheet.cpp \
    $$PWD/table.cpp \
    $$PWD/property.cpp \
    $$PWD/propsheetreader.cpp \
    $$PWD/propsheetwriter.cpp \
    $$PWD/sequence.cpp

headers.files += \
    $$PWD/propsheet.h \
    $$PWD/table.h \
    $$PWD/property.h \
    $$PWD/propsheetreader.h \
    $$PWD/propsheetwriter.h \
    $$PWD/qopslib_global.h \
    $$PWD/opsinformationprovider.h \
    $$PWD/qopslib.h \
    $$PWD/sequence.h \
    $$PWD/valuetypes.h

HEADERS +=\
    $$PWD/propsheetprivate.h \
    $$PWD/propertyprivate.h \
    $$PWD/propsheetreaderprivate.h \
    $$PWD/tableprivate.h \
    $$PWD/sequenceprivate.h

HEADERS += $$headers.files
