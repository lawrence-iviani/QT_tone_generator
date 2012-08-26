#-------------------------------------------------
#
# Project created by QtCreator 2012-07-01T13:50:17
#
#-------------------------------------------------

QT       += core gui

TARGET = ComboBoxWidgetColor
TEMPLATE = lib
LIBS += -L../ComboBoxLabelWidget -lcomboboxnamewidget.o -lmoc_comboboxnamewidget.o
INCLUDEPATH += ../ComboBoxLabelWidget

SOURCES += main.cpp\
        comboboxwidgetcolor.cpp

HEADERS  += comboboxwidgetcolor.h

FORMS    += comboboxwidgetcolor.ui

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7379BC8
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ComboBoxWidgetColor.dll
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
