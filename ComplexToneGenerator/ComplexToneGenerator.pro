#-------------------------------------------------
#
# Project created by QtCreator 2012-06-02T13:19:15
#
#-------------------------------------------------

QT       += core gui multimedia xml

TARGET = ComplexToneGenerator
TEMPLATE = app

OBJECTS_DIR = obj
MOC_DIR = moc


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
INCLUDEPATH += ../Widget/SelectMultipleCurvesWindowDialog
INCLUDEPATH += ../Widget/ScaledSliderWidget
INCLUDEPATH += ../Widget/ComboBoxWidgetColor
INCLUDEPATH += ../Widget/ComboBoxLabelWidget
INCLUDEPATH += ../Widget/QwtHelpers
INCLUDEPATH += ../Widget/ExportAudioFileWidget
INCLUDEPATH += ../Widget/WidgetStyle
INCLUDEPATH += ../AudioEngine
INCLUDEPATH += ../ErrorMessage
INCLUDEPATH += ../DataUiHandler

LIBS += -L../Widget/WidgetLib/ -lwidget -L../AudioEngine -lAudioEngine

CONFIG(debug, debug|release) {
    DESTDIR = debug
    OBJECTS_DIR = debug/obj
    MOC_DIR = debug/moc
    LIBS += -L$$PWD/../ErrorMessage/debug/ -lErrorMessage
    LIBS += -L$$PWD/../DataUiHandler/debug/ -lDataUiHandler
} else {
    DESTDIR = release
    OBJECTS_DIR = release/obj
    MOC_DIR = release/moc
    LIBS += -L$$PWD/../ErrorMessage/release/ -lErrorMessage
    LIBS += -L$$PWD/../DataUiHandler/release/ -lDataUiHandler
}

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

# ------ Files
SOURCES += envelope/dataenvelope.cpp \
    envelope/dataenvelopeparameters.cpp \
    envelope/dataenvelopeui.cpp \
    XML_utils/domhelper.cpp \
    timedata/generictimedata.cpp \
    timedata/generictimedataui.cpp \
    timedata/digesttimedata.cpp \
    timedata/partialtimedata.cpp \
    timedata/partialtimedataui.cpp \
    timedata/timedatacontrolui.cpp \
    timedata/repeatedtimedata.cpp \
    plotwidget/plotwidget.cpp \
    plotwidget/timeplotwidget.cpp \
    plotwidget/freqplotwidget.cpp \
    CustomCurves/customcurvefactory.cpp \
    CustomCurves/sinusdataparams.cpp \
    CustomCurves/genericsinusdata.cpp \
    CustomCurves/constanttimedata.cpp \
    CustomCurves/sinusdataui.cpp \
    CustomCurves/partialsinusdata.cpp \
    CustomCurves/repeatedsinusdata.cpp \
    timedata/repeatedtimedataui.cpp \
    mainwindow.cpp \
    main.cpp \
    plotwidget/timeplotwidgetparams.cpp \
    ctg_app.cpp





HEADERS  += CTG_constants.h \
    envelope/dataenvelope.h \
    envelope/dataenvelopeparameters.h \
    envelope/dataenvelopeui.h \
    XML_utils/domhelper.h \
    timedata/generictimedata.h \
    timedata/digesttimedata.h \
    timedata/partialtimedata.h \
    timedata/generictimedataui.h \
    timedata/partialtimedataui.h \
    timedata/timedatacontrolui.h \
    timedata/repeatedtimedata.h \
    timedata/repeatedtimedataui.h \
    plotwidget/plotwidget.h \
    plotwidget/timeplotwidget.h \
    plotwidget/freqplotwidget.h \
    plotwidget/scrollrubberband.h \
    CustomCurves/abstractcurvesfactory.h \
    CustomCurves/customcurvefactory.h \
    CustomCurves/sinusdataparams.h \
    CustomCurves/repeatedsinusdata.h \
    CustomCurves/genericsinusdata.h \
    CustomCurves/constanttimedata.h \
    CustomCurves/customcurveui.h \
    CustomCurves/sinusdataui.h \
    CustomCurves/partialsinusdata.h \
    mainwindow.h \
    plotwidget/timeplotwidgetparams.h \
    ctg_app.h \
    CustomCurves/customcurvesheader.h


FORMS    += mainwindow.ui \
    envelope/dataenvelopeui.ui

RESOURCES += \
    Resources/resource.qrc
