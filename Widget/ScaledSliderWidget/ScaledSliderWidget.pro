#-------------------------------------------------
#
# Project created by QtCreator 2012-06-11T01:22:41
#
#-------------------------------------------------

QT       += core gui

TARGET = ScaledSliderWidget
TEMPLATE = lib


INCLUDEPATH += /usr/local/qwt-6.0.1/lib/qwt.framework/Headers/
LIBS += -F/usr/local/qwt-6.0.1/lib/ -framework qwt


SOURCES += main.cpp\
    scaledslider.cpp \
    scaledsliderwidget.cpp

HEADERS  += scaledslider.h \
    scaledsliderwidget.h

FORMS    += mainwindow.ui

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7379BC8
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ScaledSliderWidget.dll
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