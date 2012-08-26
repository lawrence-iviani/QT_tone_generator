#-------------------------------------------------
#
# Project created by QtCreator 2012-06-11T01:22:41
#
#-------------------------------------------------

QT       += core gui

TARGET = ScaledSliderWidgetTest
TEMPLATE = app


INCLUDEPATH += /usr/local/qwt-6.0.1/lib/qwt.framework/Headers/
LIBS += -F/usr/local/qwt-6.0.1/lib/ -framework qwt


SOURCES += main.cpp\
    scaledslider.cpp \
    scaledsliderwidget.cpp

HEADERS  += scaledslider.h \
    scaledsliderwidget.h

FORMS    += mainwindow.ui

QMAKE_CLEAN += *.o  *.app *.exe 
