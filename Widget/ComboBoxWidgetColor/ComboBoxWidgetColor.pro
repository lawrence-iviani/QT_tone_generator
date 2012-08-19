#-------------------------------------------------
#
# Project created by QtCreator 2012-07-01T13:50:17
#
#-------------------------------------------------

QT       += core gui

TARGET = ComboBoxWidgetColor
TEMPLATE = app
LIBS += -L../ComboBoxLabelWidget -lcomboboxnamewidget.o -lmoc_comboboxnamewidget.o
INCLUDEPATH += ../ComboBoxLabelWidget

SOURCES += main.cpp\
        comboboxwidgetcolor.cpp

HEADERS  += comboboxwidgetcolor.h

FORMS    += comboboxwidgetcolor.ui
