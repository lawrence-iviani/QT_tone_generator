#-------------------------------------------------
#
# Project created by QtCreator 2012-07-19T19:26:48
#
#-------------------------------------------------

QT       += core gui

TARGET = SelectCurveWindowDialog
TEMPLATE = lib


SOURCES += main.cpp\
        selectcurvewindowdialog.cpp \


HEADERS  +=\
        selectcurvewindowdialog.h \


FORMS   += selectcurvewindowdialog.ui

INCLUDEPATH += ../../ComplexToneGenerator/

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7379BC8
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = SelectCurveWindowDialog.dll
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
