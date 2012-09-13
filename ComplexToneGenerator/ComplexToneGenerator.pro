#-------------------------------------------------
#
# Project created by QtCreator 2012-06-02T13:19:15
#
#-------------------------------------------------

QT       += core gui

TARGET = ComplexToneGenerator
TEMPLATE = app





# ------ Dependecies libs:
# qwt
LIBS += -F/usr/local/qwt-6.0.1/lib/ -framework qwt
INCLUDEPATH += /usr/local/qwt-6.0.1/lib/qwt.framework/Headers/

# libsndifle
INCLUDEPATH += /opt/local/include/
LIBS += -L/opt/local/lib -lsndfile

# ------ End Dependecies libs:

# ------ Internal deps
INCLUDEPATH += ../Widget/SelectCurveWindowDialog
INCLUDEPATH += ../Widget/ScaledSliderWidget
INCLUDEPATH += ../Widget/ComboBoxWidgetColor
INCLUDEPATH += ../Widget/ComboBoxLabelWidget
INCLUDEPATH += ../Widget/QwtHelpers

LIBS += -L../Widget/WidgetLib/ -lwidget

# ------ Files
SOURCES += main.cpp\
        mainwindow.cpp \
    plotwidget.cpp \
    generictimedata.cpp \
    sinusdata.cpp \
    timeplotwidget.cpp \
    freqplotwidget.cpp \
    digesttimedata.cpp

HEADERS  += mainwindow.h \
    plotwidget.h \
    generictimedata.h \
    sinusdata.h \
    timeplotwidget.h \
    freqplotwidget.h \
    CTG_constants.h \
    digesttimedata.h

FORMS    += mainwindow.ui
