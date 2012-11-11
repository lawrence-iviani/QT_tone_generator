#-------------------------------------------------
#
# Project created by QtCreator 2012-09-17T06:36:18
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = AudioEngine
TEMPLATE = lib

INCLUDEPATH += /opt/local/include/
LIBS += -L/opt/local/lib -lsndfile

SOURCES += main.cpp\
    internalstreamdevice.cpp \
    audioplayer.cpp \
    generator.cpp \
    audioutils.cpp \
    qvumeter.cpp

HEADERS  += mainwindow.h \
    internalstreamdevice.h \
    audioplayer.h \
    generator.h \
    audioutils.h \
    qvumeter.h

FORMS    +=

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7379BC8
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = AudioEngine.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
