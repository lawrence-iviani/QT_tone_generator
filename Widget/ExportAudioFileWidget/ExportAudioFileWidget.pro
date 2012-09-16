#-------------------------------------------------
#
# Project created by QtCreator 2012-09-16T18:32:36
#
#-------------------------------------------------

QT       += core gui

TARGET = ExportAudioFileWidget
TEMPLATE = lib


SOURCES += main.cpp\
        mainwindow.cpp \
   	 	exportaudiofiledialog.cpp

HEADERS  += mainwindow.h \
    	exportaudiofiledialog.h

FORMS    += mainwindow.ui

QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7379BC8
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ExportAudioFileWidget.dll
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

