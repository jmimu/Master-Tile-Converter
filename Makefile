#############################################################################
# Makefile for building: sms_3bpp_tiles_reader
# Generated by qmake (2.01a) (Qt 4.6.3) on: Tue May 15 09:50:13 2012
# Project:  sms_3bpp_tiles_reader.pro
# Template: app
# Command: /usr/bin/qmake-qt4 -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile sms_3bpp_tiles_reader.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -I.
LINK          = g++
LFLAGS        = 
LIBS          = $(SUBLIBS)  -L/usr/lib -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
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
		tile3bpp.cpp \
		tileswidget.cpp \
		palettewidget.cpp \
		zoomwidget.cpp moc_mainwindow.cpp \
		moc_tileswidget.cpp
OBJECTS       = main.o \
		mainwindow.o \
		tile.o \
		palette.o \
		tile3bpp.o \
		tileswidget.o \
		palettewidget.o \
		zoomwidget.o \
		moc_mainwindow.o \
		moc_tileswidget.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		sms_3bpp_tiles_reader.pro
QMAKE_TARGET  = sms_3bpp_tiles_reader
DESTDIR       = 
TARGET        = sms_3bpp_tiles_reader

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

$(TARGET): ui_mainwindow.h $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: sms_3bpp_tiles_reader.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/debug.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/libQtGui.prl \
		/usr/lib/libQtCore.prl
	$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile sms_3bpp_tiles_reader.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/debug.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/libQtGui.prl:
/usr/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/share/qt4/mkspecs/linux-g++ -unix CONFIG+=debug -o Makefile sms_3bpp_tiles_reader.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/sms_3bpp_tiles_reader1.0.0 || $(MKDIR) .tmp/sms_3bpp_tiles_reader1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/sms_3bpp_tiles_reader1.0.0/ && $(COPY_FILE) --parents mainwindow.h tile.h palette.h tile3bpp.h tileswidget.h palettewidget.h zoomwidget.h .tmp/sms_3bpp_tiles_reader1.0.0/ && $(COPY_FILE) --parents main.cpp mainwindow.cpp tile.cpp palette.cpp tile3bpp.cpp tileswidget.cpp palettewidget.cpp zoomwidget.cpp .tmp/sms_3bpp_tiles_reader1.0.0/ && $(COPY_FILE) --parents mainwindow.ui .tmp/sms_3bpp_tiles_reader1.0.0/ && (cd `dirname .tmp/sms_3bpp_tiles_reader1.0.0` && $(TAR) sms_3bpp_tiles_reader1.0.0.tar sms_3bpp_tiles_reader1.0.0 && $(COMPRESS) sms_3bpp_tiles_reader1.0.0.tar) && $(MOVE) `dirname .tmp/sms_3bpp_tiles_reader1.0.0`/sms_3bpp_tiles_reader1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/sms_3bpp_tiles_reader1.0.0


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

compiler_moc_header_make_all: moc_mainwindow.cpp moc_tileswidget.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mainwindow.cpp moc_tileswidget.cpp
moc_mainwindow.cpp: tile3bpp.h \
		tile.h \
		palette.h \
		mainwindow.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

moc_tileswidget.cpp: tile.h \
		palette.h \
		tileswidget.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) tileswidget.h -o moc_tileswidget.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_mainwindow.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mainwindow.h
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

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

main.o: main.cpp mainwindow.h \
		tile3bpp.h \
		tile.h \
		palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		tile3bpp.h \
		tile.h \
		palette.h \
		ui_mainwindow.h \
		palettewidget.h \
		tileswidget.h \
		zoomwidget.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

tile.o: tile.cpp tile.h \
		palette.h \
		tile3bpp.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tile.o tile.cpp

palette.o: palette.cpp palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o palette.o palette.cpp

tile3bpp.o: tile3bpp.cpp tile3bpp.h \
		tile.h \
		palette.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o tile3bpp.o tile3bpp.cpp

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

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_tileswidget.o: moc_tileswidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_tileswidget.o moc_tileswidget.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

