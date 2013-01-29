#-------------------------------------------------
#
# Project created by QtCreator 2013-01-29T13:49:39
#
#-------------------------------------------------

TARGET = WidgetStyle
TEMPLATE = lib

DEFINES += WIDGETSTYLE_LIBRARY

SOURCES += widgetstyle.cpp

HEADERS += widgetstyle.h\
        WidgetStyle_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

RESOURCES += \
    Resources/resource.qrc
