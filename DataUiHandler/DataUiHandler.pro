#-------------------------------------------------
#
# Project created by QtCreator 2013-01-06T08:29:35
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataUiHandler
TEMPLATE = lib

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

SOURCES += XML_utils/domhelper.cpp \
    XML_utils/domhelperutility.cpp \
    datauihandlerproperty.cpp \
    datauihandlerui.cpp \
    datauihandlerdelegate.cpp \

HEADERS  += XML_utils/domhelper_constant.h \
    XML_utils/domhelperutility.h \
    XML_utils/domhelper.h \
    datauihandlercommon.h \
    datauihandlerproperty.h \
    datauihandlerui.h \
    datauihandlerdelegate.h \
    DataUiHandler.h

# For macosx! Now the link is copyed as file (this is bad for the app dimension and need to be fixed)
MYAPPDEST = $$PWD/../ComplexToneGenerator/debug/ComplexToneGenerator.app/Contents/MacOS/
CONFIG(debug, debug|release) {
    DESTDIR = debug
    QMAKE_POST_LINK += $$quote(cp $$PWD/debug/libDataUiHandler* $$MYAPPDEST/)
    OBJECTS_DIR = debug/obj
    MOC_DIR = debug/moc
    LIBS += -L$$PWD/../ErrorMessage/debug/ -lErrorMessage
} else {
    DESTDIR = release
    QMAKE_POST_LINK += $$quote(cp $$PWD/release/libDataUiHandler* $$MYAPPDEST/)
    OBJECTS_DIR = release/obj
    MOC_DIR = release/moc
    LIBS += -L$$PWD/../ErrorMessage/release/ -lErrorMessage
}

INCLUDEPATH += $$PWD/../ErrorMessage
DEPENDPATH += $$PWD/../ErrorMessage
