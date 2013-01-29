#-------------------------------------------------
#
# Project created by QtCreator 2012-06-29T16:42:37
#
#-------------------------------------------------

QT       += core gui

TARGET = ComboBoxLabelWidget
TEMPLATE = lib

SOURCES += main.cpp\
        comboboxnamewidget.cpp

HEADERS  += comboboxnamewidget.h

FORMS    += comboboxnamewidget.ui

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7379BC8
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ComboBoxLabelWidget.dll
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
