################################################################
# QT tone generator
# Copyright (C) 2012   Lawrence Iviani
#
# This App is free software; you can redistribute it and/or
# modify it under the terms of GPL, Version 2
################################################################

TARGET = widget
TEMPLATE = lib

DEFINES += WIDGET_LIBRARY

INCLUDEPATH += /usr/local/qwt-6.0.1/lib/qwt.framework/Headers/
INCLUDEPATH += ../ScaledSliderWidget
INCLUDEPATH += ../ComboBoxWidgetColor
INCLUDEPATH += ../ComboBoxLabelWidget
INCLUDEPATH += ../QwtHelpers
INCLUDEPATH += ../SelectCurveWindowDialog
INCLUDEPATH += ../SelectMultipleCurvesWindowDialog
INCLUDEPATH += ../ExportAudioFileWidget
INCLUDEPATH += ../WidgetStyle

LIBS += -F/usr/local/qwt-6.0.1/lib/ -framework qwt
LIBS += -L../ScaledSliderWidget -lscaledsliderwidget.o -lscaledslider.o -lmoc_scaledslider.o -lmoc_scaledsliderwidget.o
LIBS += -L../ComboBoxWidgetColor -lmoc_comboboxwidgetcolor.o -lcomboboxwidgetcolor.o
LIBS += -L../ComboBoxLabelWidget -lmoc_comboboxnamewidget.o -lcomboboxnamewidget.o
LIBS += -L../QwtHelpers -lzmp_handler.o -lqwthelpers.o -lmoc_zmp_handler.o
LIBS += -L../SelectCurveWindowDialog -lselectcurvewindowdialog.o -lmoc_selectcurvewindowdialog.o
LIBS += -L../SelectMultipleCurvesWindowDialog -lselectmultiplecurveswindowdialog.o -lmoc_selectmultiplecurveswindowdialog.o
LIBS += -L../ExportAudioFileWidget -lexportaudiofiledialog.o  -lmoc_exportaudiofiledialog.o
LIBS += -L../WidgetStyle -lwidgetstyle.o

# For macosx! Now the link is copyed as file (this is bad for the app dimension and need to be fixed)
#MYAPPDEST = $$PWD/../../ComplexToneGenerator/debug/ComplexToneGenerator.app/Contents/MacOS/
CONFIG(debug, debug|release) {
    DESTDIR = debug
   # QMAKE_POST_LINK += $$quote(cp $$PWD/debug/libwidget* $$MYAPPDEST/)
    OBJECTS_DIR = debug/obj
    MOC_DIR = debug/moc
} else {
    DESTDIR = release
 #   QMAKE_POST_LINK += $$quote(cp $$PWD/release/libwidget* $$MYAPPDEST/)
    OBJECTS_DIR = release/obj
    MOC_DIR = release/moc
}


QMAKE_CLEAN += *.o *.dll *.so *.a *.dylib

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE121D869
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = widget.dll
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
