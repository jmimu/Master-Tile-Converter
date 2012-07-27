TARGET = mastertileconverter
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    tile.cpp \
    palette.cpp \
    tileswidget.cpp \
    palettewidget.cpp \
    zoomwidget.cpp \
    rom.cpp \
    romheader.cpp \
    dialog_about.cpp
HEADERS += mainwindow.h \
    tile.h \
    palette.h \
    tileswidget.h \
    palettewidget.h \
    zoomwidget.h \
    rom.h \
    romheader.h \
    dialog_about.h
FORMS += mainwindow.ui \
    dialog_about.ui
RESOURCES += data.qrc
CONFIG +=debug
