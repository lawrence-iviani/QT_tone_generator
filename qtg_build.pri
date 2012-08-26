################################################################
# QT tone generator
# Copyright (C) 2012   Lawrence Iviani
#
# This App is free software; you can redistribute it and/or
# modify it under the terms of GPL, Version 2
################################################################

######################################################################
# qmake internal options
######################################################################

CONFIG           += qt     

######################################################################
# release/debug mode
######################################################################

win32 {
    # On Windows you can't mix release and debug libraries.
    # The designer is built in release mode. If you like to use it
    # you need a release version. For your own application development you
    # might need a debug version. 
    # Enable debug_and_release + build_all if you want to build both.

    CONFIG           += debug_and_release
    CONFIG           += build_all
}
else {
    CONFIG           += release

    VER_MAJ           = $${QTG_VER_MAJ}
    VER_MIN           = $${QTG_VER_MIN}
    VER_PAT           = $${QTG_VER_PAT}
    VERSION           = $${QTG_VERSION}
}

linux-g++ {
    # CONFIG           += separate_debug_info
}



