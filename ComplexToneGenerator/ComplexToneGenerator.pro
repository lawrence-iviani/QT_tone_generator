#-------------------------------------------------
#
# Project created by QtCreator 2012-06-02T13:19:15
#
#-------------------------------------------------

QT       += core gui

TARGET = ComplexToneGenerator
TEMPLATE = app

LIBS += -F/usr/local/qwt-6.0.1/lib/ -framework qwt
LIBS += -L../Widget/ -lwidget


INCLUDEPATH += /usr/local/qwt-6.0.1/lib/qwt.framework/Headers/
INCLUDEPATH += ../Widget/SelectCurveWindowDialog
INCLUDEPATH += ../Widget/ScaledSliderWidget
INCLUDEPATH += ../Widget/ComboBoxWidgetColor
INCLUDEPATH += ../Widget/ComboBoxLabelWidget
INCLUDEPATH += ../Widget/QwtHelpers

SOURCES += main.cpp\
        mainwindow.cpp \
    plotwidget.cpp \
    generictimedata.cpp \
    sinusdata.cpp \
    timeplotwidget.cpp \
    freqplotwidget.cpp

HEADERS  += mainwindow.h \
    plotwidget.h \
    generictimedata.h \
    sinusdata.h \
    timeplotwidget.h \
    freqplotwidget.h \
    CTG_constants.h

FORMS    += mainwindow.ui
