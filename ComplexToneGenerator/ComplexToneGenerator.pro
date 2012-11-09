#-------------------------------------------------
#
# Project created by QtCreator 2012-06-02T13:19:15
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = ComplexToneGenerator
TEMPLATE = app

OBJECTS_DIR = obj



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
INCLUDEPATH += ../Widget/SelectRemoveCurveWindowDialog
INCLUDEPATH += ../Widget/ScaledSliderWidget
INCLUDEPATH += ../Widget/ComboBoxWidgetColor
INCLUDEPATH += ../Widget/ComboBoxLabelWidget
INCLUDEPATH += ../Widget/QwtHelpers
INCLUDEPATH += ../Widget/ExportAudioFileWidget
INCLUDEPATH += ../AudioEngine

LIBS += -L../Widget/WidgetLib/ -lwidget -L../AudioEngine -lAudioEngine

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

# ------ Files
SOURCES += envelope/dataenvelope.cpp \
    envelope/dataenvelopeparameters.cpp \
    envelope/dataenvelopeui.cpp \
    timedata/generictimedata.cpp \
    timedata/generictimedataui.cpp \
    timedata/digesttimedata.cpp \
    timedata/partialtimedata.cpp \
    timedata/partialtimedataui.cpp \
    timedata/timedatacontrolui.cpp \
    plotwidget/plotwidget.cpp \
    plotwidget/timeplotwidget.cpp \
    plotwidget/freqplotwidget.cpp \
    CustomCurves/sinusdata.cpp \
    CustomCurves/constanttimedata.cpp \
    CustomCurves/sinusdataui.cpp \
    CustomCurves/partialsinusdata.cpp \
    mainwindow.cpp \
    main.cpp



HEADERS  += CTG_constants.h \
    envelope/dataenvelope.h \
    envelope/dataenvelopeparameters.h \
    envelope/dataenvelopeui.h \
    timedata/generictimedata.h \
    timedata/digesttimedata.h \
    timedata/partialtimedata.h \
    timedata/generictimedataui.h \
    timedata/partialtimedataui.h \
    timedata/timedatacontrolui.h \
    plotwidget/plotwidget.h \
    plotwidget/timeplotwidget.h \
    plotwidget/freqplotwidget.h \
    plotwidget/scrollrubberband.h \
    CustomCurves/sinusdata.h \
    CustomCurves/constanttimedata.h \
    CustomCurves/customcurveui.h \
    CustomCurves/sinusdataui.h \
    CustomCurves/partialsinusdata.h \
    mainwindow.h


FORMS    += mainwindow.ui \
    envelope/dataenvelopeui.ui
