#-------------------------------------------------
#
# Project created by QtCreator 2013-03-09T10:19:25
#
#-------------------------------------------------

QT       += core gui multimedia xml

TARGET = CustomCurves
TEMPLATE = lib

DEFINES += CUSTOMCURVES_LIBRARY

# qwt
INCLUDEPATH += /usr/local/qwt-6.0.1/lib/qwt.framework/Headers/
INCLUDEPATH += ../ComplexToneGenerator ../ErrorMessage ../DataUiHandler
INCLUDEPATH += ../Widget/SelectCurveWindowDialog ../Widget/SelectMultipleCurvesWindowDialog ../Widget/ScaledSliderWidget ../Widget/ComboBoxWidgetColor ../Widget/ComboBoxLabelWidget ../Widget/QwtHelpers ../Widget/ExportAudioFileWidget ../Widget/WidgetStyle


LIBS += -F/usr/local/qwt-6.0.1/lib/ -framework qwt
LIBS += -L../Widget/WidgetLib/ -lwidget

CONFIG(debug, debug|release) {
    DESTDIR = debug
    OBJECTS_DIR = debug/
    MOC_DIR = debug/
    LIBS += -L$$PWD/../AudioEngine/debug/ -lAudioEngine
    LIBS += -L$$PWD/../ErrorMessage/debug/ -lErrorMessage
    LIBS += -L$$PWD/../DataUiHandler/debug/ -lDataUiHandler
    LIBS += -L$$PWD/../ComplexToneGenerator/debug/obj/ -lpartialtimedataui.o
} else {
    DESTDIR = release
    OBJECTS_DIR = release/
    MOC_DIR = release/
    LIBS += -L$$PWD/../AudioEngine/release/ -lAudioEngine
    LIBS += -L$$PWD/../ErrorMessage/release/ -lErrorMessage
    LIBS += -L$$PWD/../DataUiHandler/release/ -lDataUiHandler
}



SOURCES += customcurves.cpp \
        customcurvefactory.cpp \
        SinusData/sinusdata.cpp \
        SinusData/genericsinusdata.cpp \
        SinusData/partialsinusdata.cpp \
        SinusData/repeatedsinusdata.cpp \

HEADERS += customcurves.h\
        CustomCurves_global.h \
        abstractcurvesfactory.h \
        customcurvefactory.h \
        customcurvesheader.h \
        SinusData/sinusdata.h \
        SinusData/genericsinusdata.h \
        SinusData/repeatedsinusdata.h \
        SinusData/partialsinusdata.h \

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

