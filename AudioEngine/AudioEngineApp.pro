#-------------------------------------------------
#
# Project created by QtCreator 2012-09-17T06:36:18
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = AudioEngineTest
TEMPLATE = app

INCLUDEPATH += /opt/local/include/
LIBS += -L/opt/local/lib -lsndfile

SOURCES += main.cpp\
    internalstreamdevice.cpp \
    audioplayer.cpp \
    generator.cpp \
    audioutils.cpp

HEADERS  += mainwindow.h \
    internalstreamdevice.h \
    audioplayer.h \
    generator.h \
    audioutils.h

FORMS    +=

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib
