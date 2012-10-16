#-------------------------------------------------
#
# Project created by QtCreator 2012-10-15T20:05:45
#
#-------------------------------------------------

QT       += core gui

TARGET = SelectRemoveCurveWindowDialog
TEMPLATE = lib


SOURCES += main.cpp\
        selectremovecurvewindowdialog.cpp

HEADERS  += selectremovecurvewindowdialog.h

FORMS    += selectremovecurvewindowdialog.ui

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7379BC8
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = SelectRemoveCurveWindowDialog.dll
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
