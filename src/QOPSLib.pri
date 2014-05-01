
INCLUDEPATH += $$PWD

contains(QT, gui) {
    DEFINES += USE_GUI
    message("GUI support enabled.")
    QMAKE_POST_LINK += printf \""$$LITERAL_HASH"define USE_GUI\n\" >> src/qopslib.h
}

!contains(QT, gui) {
    message("GUI support disabled.")
}

contains(DEFINES, NO_SMART_POINTERS) {
    message("Smart pointers disabled.")
    QMAKE_POST_LINK += printf \""$$LITERAL_HASH"define NO_SMART_POINTERS\n\" >> src/qopslib.h
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
