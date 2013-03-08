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

LIBS += -L../Widget/WidgetLib/ -lwidget

CONFIG(debug, debug|release) {
    DESTDIR = debug
    OBJECTS_DIR = debug/obj
    MOC_DIR = debug/moc
    LIBS += -L$$PWD/../AudioEngine/debug/ -lAudioEngine
    LIBS += -L$$PWD/../ErrorMessage/debug/ -lErrorMessage
    LIBS += -L$$PWD/../DataUiHandler/debug/ -lDataUiHandler
} else {
    DESTDIR = release
    OBJECTS_DIR = release/obj
    MOC_DIR = release/moc
    LIBS += -L$$PWD/../AudioEngine/release/ -lAudioEngine
    LIBS += -L$$PWD/../ErrorMessage/release/ -lErrorMessage
    LIBS += -L$$PWD/../DataUiHandler/release/ -lDataUiHandler
}

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

# ------ Files
SOURCES +=    timedata/generictimedataparams.cpp \
    timedata/generictimedataui.cpp \
    timedata/generictimedata.cpp \
    envelope/dataenvelopeparameters.cpp \
    envelope/dataenvelopeui.cpp \
    envelope/dataenvelope.cpp \
    timedata/digesttimedata.cpp \
    timedata/partialtimedataparams.cpp \
    timedata/partialtimedataui.cpp \
    timedata/partialtimedata.cpp \
    timedata/repeatedtimedataparams.cpp \
    timedata/repeatedtimedataui.cpp \
    timedata/repeatedtimedata.cpp \
    plotwidget/plotwidget.cpp \
    plotwidget/timeplotwidgetparams.cpp \
    plotwidget/timeplotwidgetui.cpp \
    plotwidget/timeplotwidget.cpp \
    plotwidget/freqplotwidget.cpp \
    CustomCurves/customcurvefactory.cpp \
    CustomCurves/genericsinusdata.cpp \
    CustomCurves/partialsinusdata.cpp \
    CustomCurves/repeatedsinusdata.cpp \
    mainwindow.cpp \
    main.cpp \
    ctg_app.cpp \
    CustomCurves/sinusdata.cpp




HEADERS  += CTG_constants.h \
    timedata/generictimedataparams.h \
    timedata/generictimedataui.h \
    timedata/generictimedata.h \
    timedata/digesttimedata.h \
    envelope/dataenvelopeparameters.h \
    envelope/dataenvelopeui.h \
    envelope/dataenvelope.h \
    timedata/partialtimedataparams.h \
    timedata/partialtimedataui.h \
    timedata/partialtimedata.h \
    timedata/repeatedtimedataparams.h \
    timedata/repeatedtimedata.h \
    timedata/repeatedtimedataui.h \
    plotwidget/plotwidget.h \
    plotwidget/timeplotwidgetparams.h \
    plotwidget/timeplotwidgetui.h \
    plotwidget/timeplotwidget.h \
    plotwidget/freqplotwidget.h \
    plotwidget/scrollrubberband.h \
    CustomCurves/abstractcurvesfactory.h \
    CustomCurves/customcurvefactory.h \
    CustomCurves/genericsinusdata.h \
    CustomCurves/repeatedsinusdata.h \
    CustomCurves/partialsinusdata.h \
    mainwindow.h \
    ctg_app.h \
    CustomCurves/customcurvesheader.h \
    CustomCurves/sinusdata.h


FORMS    += mainwindow.ui

RESOURCES += \
    Resources/resource.qrc
