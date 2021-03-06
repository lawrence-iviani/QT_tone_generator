################################################################
# QT tone generator
# Copyright (C) 2012   Lawrence Iviani
#
# This App is free software; you can redistribute it and/or
# modify it under the terms of GPL, Version 2
################################################################

include( ../qtg_config.pri )

TEMPLATE = subdirs
CONFIG   += ordered

INCLUDEPATH += ./WidgetStyle


SUBDIRS += WidgetStyle \
           ScaledSliderWidget \
           ComboBoxLabelWidget \
           ComboBoxWidgetColor \
           QwtHelpers \
           SelectCurveWindowDialog \
           SelectMultipleCurvesWindowDialog \
           ExportAudioFileWidget \
           WidgetLib \

           
    
