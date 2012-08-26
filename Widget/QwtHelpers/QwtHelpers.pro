#-------------------------------------------------
#
# Project created by QtCreator 2012-07-09T20:52:21
#
#-------------------------------------------------

TARGET = QwtHelpers
TEMPLATE = lib

DEFINES += QWTHELPERS_LIBRARY

INCLUDEPATH += /usr/local/qwt-6.0.1/lib/qwt.framework/Headers/
LIBS += -F/usr/local/qwt-6.0.1/lib/ -framework qwt

SOURCES += qwthelpers.cpp \
    zmp_handler.cpp

HEADERS += qwthelpers.h\
        QwtHelpers_global.h \
    zmp_handler.h

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7379BC8
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = QwtHelpers.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
