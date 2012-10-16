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
INCLUDEPATH += ../SelectRemoveCurveWindowDialog
INCLUDEPATH += ../ExportAudioFileWidget

LIBS += -F/usr/local/qwt-6.0.1/lib/ -framework qwt
LIBS += -L../ScaledSliderWidget -lscaledsliderwidget.o -lscaledslider.o -lmoc_scaledslider.o -lmoc_scaledsliderwidget.o
LIBS += -L../ComboBoxWidgetColor -lmoc_comboboxwidgetcolor.o -lcomboboxwidgetcolor.o
LIBS += -L../ComboBoxLabelWidget -lmoc_comboboxnamewidget.o -lcomboboxnamewidget.o
LIBS += -L../QwtHelpers -lzmp_handler.o -lqwthelpers.o -lmoc_zmp_handler.o
LIBS += -L../SelectCurveWindowDialog -lselectcurvewindowdialog.o -lmoc_selectcurvewindowdialog.o -lselectcurvewindowhelper.o -lmoc_selectcurvewindowhelper.o
LIBS += -L../SelectRemoveCurveWindowDialog -lselectremovecurvewindowdialog.o -lmoc_selectremovecurvewindowdialog.o

LIBS += -L../ExportAudioFileWidget -lexportaudiofiledialog.o  -lmoc_exportaudiofiledialog.o


#LIBS += -L../ComboBoxLabelWidget -lComboBoxLabelWidget
#LIBS += -L../ScaledSliderWidget -lScaledSliderWidget
#LIBS += -L../ComboBoxWidgetColor -lComboBoxWidgetColor
#LIBS += -L../QwtHelpers -lQwtHelpers
#LIBS += -L../SelectCurveWindowDialog -lSelectCurveWindowDialog

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
