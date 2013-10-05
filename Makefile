#############################################################################
# Makefile for building: mastertileconverter
# Generated by qmake (2.01a) (Qt 4.8.1) on: Sat Oct 5 18:26:50 2013
# Project:  mastertileconverter.pro
# Template: app
# Command: /usr/bin/qmake -spec /usr/share/qt4/mkspecs/linux-g++ -o Makefile mastertileconverter.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_NO_DEBUG -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4 -I. -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/x86_64-linux-gnu -lQtXml -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		mainwindow.cpp \
		tile.cpp \
		palette.cpp \
		tileswidget.cpp \
		palettewidget.cpp \
		zoomwidget.cpp \
		rom.cpp \
		romheader.cpp \
		dialog_about.cpp \
		mtcproject.cpp moc_mainwindow.cpp \
		moc_tileswidget.cpp \
		moc_dialog_about.cpp \
		qrc_data.cpp
OBJECTS       = main.o \
		mainwindow.o \
		tile.o \
		palette.o \
		tileswidget.o \
		palettewidget.o \
		zoomwidget.o \
		rom.o \
		romheader.o \
		dialog_about.o \
		mtcproject.o \
		moc_mainwindow.o \
		moc_tileswidget.o \
		moc_dialog_about.o \
		qrc_data.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		mastertileconverter.pro
QMAKE_TARGET  = mastertileconverter
DESTDIR       = 
TARGET        = mastertileconverter

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): ui_mainwindow.h ui_dialog_about.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: mastertileconverter.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/x86_64-linux-gnu/libQtXml.prl \
		/usr/lib/x86_64-linux-gnu/libQtGui.prl \
		/usr/lib/x86_64-linux-gnu/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -o Makefile mastertileconverter.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/x86_64-linux-gnu/libQtXml.prl:
/usr/lib/x86_64-linux-gnu/libQtGui.prl:
/usr/lib/x86_64-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -o Makefile mastertileconverter.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/mastertileconverter1.0.0 || $(MKDIR) .tmp/mastertileconverter1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/mastertileconverter1.0.0/ && $(COPY_FILE) --parents mainwindow.h tile.h palette.h tileswidget.h palettewidget.h zoomwidget.h rom.h romheader.h dialog_about.h mtcproject.h .tmp/mastertileconverter1.0.0/ && $(COPY_FILE) --parents data.qrc .tmp/mastertileconverter1.0.0/ && $(COPY_FILE) --parents main.cpp mainwindow.cpp tile.cpp palette.cpp tileswidget.cpp palettewidget.cpp zoomwidget.cpp rom.cpp romheader.cpp dialog_about.cpp mtcproject.cpp .tmp/mastertileconverter1.0.0/ && $(COPY_FILE) --parents mainwindow.ui dialog_about.ui .tmp/mastertileconverter1.0.0/ && (cd `dirname .tmp/mastertileconverter1.0.0` && $(TAR) mastertileconverter1.0.0.tar mastertileconverter1.0.0 && $(COMPRESS) mastertileconverter1.0.0.tar) && $(MOVE) `dirname .tmp/mastertileconverter1.0.0`/mastertileconverter1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/mastertileconverter1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_mainwindow.cpp moc_tileswidget.cpp moc_dialog_about.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_tileswidget.cpp moc_dialog_about.cpp
moc_mainwindow.cpp: tile.h \
		palette.h \
		rom.h \
		mtcproject.h \
		mainwindow.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

moc_tileswidget.cpp: tile.h \
		palette.h \
		tileswidget.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) tileswidget.h -o moc_tileswidget.cpp

moc_dialog_about.cpp: dialog_about.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) dialog_about.h -o moc_dialog_about.cpp

compiler_rcc_make_all: qrc_data.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_data.cpp
qrc_data.cpp: data.qrc \
		icon.png
	/usr/bin/rcc -name data data.qrc -o qrc_data.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_mainwindow.h ui_dialog_about.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mainwindow.h ui_dialog_about.h
ui_mainwindow.h: mainwindow.ui \
		tileswidget.h \
		palettewidget.h \
		zoomwidget.h \
		tile.h \
		palette.h \
		palette.h \
		tile.h \
		palette.h
	/usr/bin/uic-qt4 mainwindow.ui -o ui_mainwindow.h

ui_dialog_about.h: dialog_about.ui
	/usr/bin/uic-qt4 dialog_about.ui -o ui_dialog_about.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

main.o: main.cpp mainwindow.h \
		tile.h \
		palette.h \
		rom.h \
		mtcproject.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		tile.h \
		palette.h \
		rom.h \
		mtcproject.h \
		ui_mainwindow.h \
		palettewidget.h \
		tileswidget.h \
		zoomwidget.h \
		dialog_about.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

tile.o: tile.cpp tile.h \
		palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tile.o tile.cpp

palette.o: palette.cpp palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o palette.o palette.cpp

tileswidget.o: tileswidget.cpp tileswidget.h \
		tile.h \
		palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tileswidget.o tileswidget.cpp

palettewidget.o: palettewidget.cpp palettewidget.h \
		palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o palettewidget.o palettewidget.cpp

zoomwidget.o: zoomwidget.cpp zoomwidget.h \
		tile.h \
		palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o zoomwidget.o zoomwidget.cpp

rom.o: rom.cpp rom.h \
		tile.h \
		palette.h \
		romheader.h \
		tileswidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o rom.o rom.cpp

romheader.o: romheader.cpp romheader.h \
		rom.h \
		tile.h \
		palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o romheader.o romheader.cpp

dialog_about.o: dialog_about.cpp dialog_about.h \
		ui_dialog_about.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o dialog_about.o dialog_about.cpp

mtcproject.o: mtcproject.cpp mtcproject.h \
		palette.h \
		rom.h \
		tile.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mtcproject.o mtcproject.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_tileswidget.o: moc_tileswidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_tileswidget.o moc_tileswidget.cpp

moc_dialog_about.o: moc_dialog_about.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_dialog_about.o moc_dialog_about.cpp

qrc_data.o: qrc_data.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qrc_data.o qrc_data.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

