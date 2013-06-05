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

# fftw3
INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib -lfftw3

# ------ End Dependecies libs:

# ------ Internal deps
INCLUDEPATH += ../Widget/SelectCurveWindowDialog ../Widget/SelectMultipleCurvesWindowDialog ../Widget/ScaledSliderWidget ../Widget/ComboBoxWidgetColor ../Widget/ComboBoxLabelWidget ../Widget/QwtHelpers ../Widget/ExportAudioFileWidget ../Widget/WidgetStyle
INCLUDEPATH += ../AudioEngine
INCLUDEPATH += ../ErrorMessage
INCLUDEPATH += ../DataUiHandler
INCLUDEPATH += ../CustomCurves



CONFIG(debug, debug|release) {
    DESTDIR = debug
    OBJECTS_DIR = debug/obj
    MOC_DIR = debug/moc
    LIBS += -L$$PWD/../Widget/WidgetLib/debug/ -lwidget
    LIBS += -L$$PWD/../AudioEngine/debug/ -lAudioEngine
    LIBS += -L$$PWD/../ErrorMessage/debug/ -lErrorMessage
    LIBS += -L$$PWD/../DataUiHandler/debug/ -lDataUiHandler
} else {
    DESTDIR = release
    OBJECTS_DIR = release/obj
    MOC_DIR = release/moc
    LIBS += -L$$PWD/../Widget/WidgetLib/release/ -lwidget
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
    CustomCurves/customcurvefactory.cpp \
    CustomCurves/SinusData/sinusdata.cpp \
    CustomCurves/SinusData/genericsinusdata.cpp \
    CustomCurves/SinusData/partialsinusdata.cpp \
    CustomCurves/SinusData/repeatedsinusdata.cpp \
    CustomCurves/SquareData/squaredata.cpp \
    CustomCurves/SquareData/genericsquaredata.cpp \
    CustomCurves/SquareData/partialsquaredata.cpp \
    CustomCurves/SquareData/repeatedsquaredata.cpp \
    plotwidget/plotwidget.cpp \
    plotwidget/timeplotwidgetparams.cpp \
    plotwidget/timeplotwidgetui.cpp \
    plotwidget/timeplotwidget.cpp \
    plotwidget/freqplotwigetparams.cpp \
    plotwidget/freqplotwidgetui.cpp \
    plotwidget/freqplotwidget.cpp \
    mainwindow.cpp \
    main.cpp \
    ctg_app.cpp \

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
    CustomCurves/abstractcurvesfactory.h \
    CustomCurves/customcurvefactory.h \
    CustomCurves/customcurvesheader.h \
    CustomCurves/SinusData/sinusdata.h \
    CustomCurves/SinusData/genericsinusdata.h \
    CustomCurves/SinusData/partialsinusdata.h \
    CustomCurves/SinusData/repeatedsinusdata.h \
    CustomCurves/SquareData/squaredata.h \
    CustomCurves/SquareData/genericsquaredata.h \
    CustomCurves/SquareData/partialsquaredata.h \
    CustomCurves/SquareData/repeatedsquaredata.h \
    plotwidget/plotwidget.h \
    plotwidget/timeplotwidgetparams.h \
    plotwidget/timeplotwidgetui.h \
    plotwidget/timeplotwidget.h \
    plotwidget/freqplotwigetparams.h \
    plotwidget/freqplotwidgetui.h \
    plotwidget/freqplotwidget.h \
    plotwidget/scrollrubberband.h \
    mainwindow.h \
    ctg_app.h \


FORMS    += mainwindow.ui

RESOURCES += \
    Resources/resource.qrc
