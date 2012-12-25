#-------------------------------------------------
#
# Project created by QtCreator 2012-12-24T23:14:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SelectMultipleCurvesWindowDialog
TEMPLATE = lib

QMAKE_CLEAN += *.o  *.app *.exe *.dll *.a *.so *.dylib

SOURCES += selectmultiplecurveswindowdialog.cpp

HEADERS  += selectmultiplecurveswindowdialog.h

FORMS    += selectmultiplecurveswindowdialog.ui

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
