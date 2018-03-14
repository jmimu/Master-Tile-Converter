TARGET = mastertileconverter
TEMPLATE = app
QT       += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
SOURCES += main.cpp \
    mainwindow.cpp \
    tile.cpp \
    palette.cpp \
    tileswidget.cpp \
    palettewidget.cpp \
    zoomwidget.cpp \
    rom.cpp \
    dialog_about.cpp \
    mtcproject.cpp \
    system.cpp \
    system_SMS.cpp \
    system_unknown.cpp \
    dialogcompress.cpp
HEADERS += mainwindow.h \
    tile.h \
    palette.h \
    tileswidget.h \
    palettewidget.h \
    zoomwidget.h \
    rom.h \
    dialog_about.h \
    mtcproject.h \
    system.h \
    system_SMS.h \
    system_unknown.h \
    dialogcompress.h
FORMS += mainwindow.ui \
    dialog_about.ui \
    dialogcompress.ui
RESOURCES += data.qrc
CONFIG += debug
OTHER_FILES += README
