#-------------------------------------------------
#
# Project created by QtCreator 2012-09-16T18:32:36
#
#-------------------------------------------------

QT       += core gui

TARGET = ExportAudioFileWidget
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    exportaudiofiledialog.cpp

HEADERS  += mainwindow.h \
    exportaudiofiledialog.h

FORMS    += mainwindow.ui

QMAKE_CLEAN += *.o  *.app *.exe 
