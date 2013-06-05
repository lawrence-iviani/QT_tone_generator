#-------------------------------------------------
#
# Project created by QtCreator 2013-01-12T12:15:31
#
#-------------------------------------------------

QT       -= gui

TARGET = ErrorMessage
TEMPLATE = lib

DEFINES += ERRORMESSAGE_LIBRARY

SOURCES += errormessage.cpp

HEADERS += errormessage.h\
        ErrorMessage_global.h


# For macosx! Now the link is copyed as file (this is bad for the app dimension and need to be fixed)
# MYAPPDEST = $$PWD/../ComplexToneGenerator/debug/ComplexToneGenerator.app/Contents/MacOS/
CONFIG(debug, debug|release) {
    DESTDIR = debug
   # QMAKE_POST_LINK += $$quote(cp $$PWD/debug/libErrorMessage* $$MYAPPDEST/)
    OBJECTS_DIR = debug/obj
    MOC_DIR = debug/moc
} else {
    DESTDIR = release
   # QMAKE_POST_LINK += $$quote(cp $$PWD/release/libErrorMessage* $$MYAPPDEST/)
    OBJECTS_DIR = release/obj
    MOC_DIR = release/moc
}


QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib
symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE14F28CD
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ErrorMessage.dll
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
